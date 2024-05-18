#include "riscv.h"
#include "types.h"

extern char trampline[], uservec[], userret[];

// in kernelvec.S, calls kerneltrap()
void kernelvec();

//
// handle an interrupt, exception, or system call from user space.
// called from trampline.S
//
void usertrap(void)
{
    int which_dev = 0;

    if ((r_status() & SSTATUS_SPP) != 0)
    {
        panic("usertrap: not from user mode");
    }

    // send interrupts and exceptions to kerneltrap()
    // since we're now in the kernel
    w_stvec((u64)kernelvec);

    struct proc *p = myproc();

    // save user program counter.
    p->trapframe->epc = r_sepc();

    if (r_scause() == 8) // system call
    {
        if(killed(p)) {
            exit(-1); // will not return
        }

        // sepc points to the ecall instruction,
        //but we want to return to the next instruction.
        p->trapframe->epc += 4;

        // an interrupt will change sepc, scause, and sstatus,
        // so enable only now that we're done with those registers.
        intr_on();

        syscall();
    } else if((which_dev = devintr()) != 0) {
        // ok
    } else {
        printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
        printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
        setkilled(p);
    }

    if (killed(p)) {
        exit(-1);
    }

    // give up the CPU if this is a timer interrupt.
    if(which_dev == 2) {
        yield();
    }

    usertrapret();
}

 