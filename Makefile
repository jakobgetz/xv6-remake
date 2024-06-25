K = kernel
U = user

OBJS = \
	$K/entry.o \
	$K/start.o \
	$K/console.o \
	$K/uart.o \
	$K/kalloc.o \
	$K/spinlock.o \
	$K/main.o \
	$K/vm.o \
	$K/proc.o \
	$K/swtch.o \
	$K/trampoline.o \
	$K/trap.o \
	$K/sysproc.o \
	$K/kernelvec.o

TOOLPREFIX = riscv64-elf-
CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy

CFLAGS = -mcmodel=medany
LDFLAGS = 
OCFLAGS = \
	--only-section=.text \
	--only-section=.rodata \
	--only-section=.data \
	--only-section=.bss

$K/kernel: $(OBJS) $K/kernel.ld
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $K/kernel.elf $(OBJS)
# just for me, because I wanna try objcopy on elf
	$(OBJCOPY) $(OCFLAGS) $K/kernel.elf $K/kernel
# only for debug
	$(OBJDUMP) -S $K/kernel > $K/kernel.asm 
	$(OBJDUMP) -t $K/kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $K/kernel.sym

ULIB = $U/usys.o

# _% gets substituted with UPROGS, because UPROGS is required by some rule
# so it needs to get built by some rule, and this is the only rule matching
_%: %.o $(ULIB)
	$(LD) $(LDFLAGS) -T $U/user.ld -o $@ $^
# only for debug
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym


$U/usys.S: $U/usys.pl
	perl $U/usys.pl > $U/usys.S

$U/usys.o: $U/usys.S
	$(CC) $(CFLAGS) -c -o $U/usys.o $U/usys.S

UPROGS = \
	$U/_cat \
	$U/_echo \
	$U/_forktest \
	$U/_grep \
	$U/_init \
	$U/_kill \
	$U/_ln \
	$U/_ls \
	$U/_mkdir \
	$U/_rm \
	$U/_sh \
	$U/_stressfs \
	$U/_usertests \
	$U/_grind \
	$U/_wc \
	$U/_zombie \
	$U/_ps

QEMU = qemu-system-riscv64
QFLAGS = -machine virt -bios none -m 128 -smp 8 -nographic -serial mon:stdio

qemu: $K/kernel
	$(QEMU) $(QFLAGS) -kernel $K/kernel