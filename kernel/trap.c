#include "riscv.h"
#include "types.h"
#include "memlayout.h"

struct spinlock tickslock;
u32 ticks;

extern char trampoline[], uservec[], userret[];

// in kernelvec.S, calls kerneltrap()
void kernelvec();

//
// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
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
        if (killed(p))
        {
            exit(-1); // will not return
        }

        // sepc points to the ecall instruction,
        // but we want to return to the next instruction.
        p->trapframe->epc += 4;

        // an interrupt will change sepc, scause, and sstatus,
        // so enable only now that we're done with those registers.
        intr_on();

        syscall();
    }
    else if ((which_dev = devintr()) != 0)
    {
        // ok
    }
    else
    {
        printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
        printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
        setkilled(p);
    }

    if (killed(p))
    {
        exit(-1);
    }

    // give up the CPU if this is a timer interrupt.
    if (which_dev == 2)
    {
        yield();
    }

    usertrapret();
}

//
// return to user space
//

void usertrapret(void)
{
    struct proc *p = myproc();

    // we're about to switch the destination of traps from
    // kerneltrap() to usertrap(), so turn off interrupts until
    // we're back in user space, where usertrap() is correct.
    intr_off();

    // send syscalls, interrupts, and exception to trampoline.S
    u64 trampoline_uservec = TRAMPOLINE + (uservec - trampoline);
    w_stvec(trampoline_uservec);

    // set up trapframe values that uservec will need when
    // the process next traps into the kernel.
    p->trapframe->kernel_satp = r_satp();         // kernel page table
    p->trapframe->kernel_sp = p->kstack + PGSIZE; // process's kernel stack
    p->trapframe->kernel_trap = (u64)usertrap;
    p->trapframe->kernel_hartid = r_tp(); // hartid for cpuid()

    // set up the registers that trampoline.S's sret will use
    // to get to user space.

    // set S Previous Privilege mode to User.
    u64 x = r_sstatus();
    x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
    x |= SSTATUS_SPIE; // enable interrupts in user mode
    w_sstatus(x);

    // set S Exception Program Counter to the saved user pc.
    w_sepc(p->trapframe->epc);

    // tell trampoline.S the user page table to switch to.
    u64 satp = MAKE_SATP(p->pagetable);

    // jump to trampoline.S at the top of memory, which
    // switches to the user page table, restores user registers,
    // and switches to user mode with sret.
    u64 fn = TRAMPOLINE + (userret - trampoline);
    ((void (*)(u64))fn)(satp);
}