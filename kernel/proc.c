#include "param.h"
#include "proc.h"
#include "riscv.h"

struct cpu cpus[NCPU];

// Must be called with interrupts disabled,
// to prevent race with process being moved
// to a different CPU.
int cpuid()
{
    int id = r_tp();
    return id;
}

// Return this CPU's cpu struct.
// Interupts must be disabled.
struct cpu *mycpu(void)
{
    int id = cpuid();
    struct cpu *c = &cpus[id];
    return c;
}

// Return the current struct proc *, or zero if none.
struct proc *myproc(void)
{
    push_off(); // not always needed because interupts often are disabled already
    struct cpu *c = mycpu();
    struct proc *p = c->proc;
    pop_off();
    return p;
}

void setkilled(struct proc *p)
{
    acquire(&p->lock);
    p->killed = 1;
    release(&p->lock);
}