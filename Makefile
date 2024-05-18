K = kernel
U = user

OBJS = \
	$K/entry.o \
	$K/start.o \
	$K/main.o \
	$K/timervec.o

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
	$(OBJCOPY) $(OCFLAGS) $K/kernel.elf $K/kernel

QEMU = qemu-system-riscv64
QFLAGS = -machine virt -bios none -m 128 -smp 8 -nographic -serial mon:stdio

qemu: $K/kernel
	$(QEMU) $(QFLAGS) -kernel $K/kernel