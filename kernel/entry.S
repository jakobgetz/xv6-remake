.section .text
.global _entry
_entry:
    # set up a stack for C.
    # stack0 is declared in start.c,
    # with a 4096-byte stack per CPU.
    # sp = stack0 + (hartid * 4096)
    la  sp, stack0
    li a0, 4096
    csrr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
    add sp, sp, a0
    # jump to start() in start.c
    call start # we dont assume start to return

# write something to UART
li t0, 0x10000000
la t1, 65
sb t1, 0(t0)
wfi

spin:
    j spin
