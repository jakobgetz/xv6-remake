#include "param.h"
#include "memlayout.h"
#include "types.h"
#include "riscv.h"

void main();
void timerinit();

// assembly code in kernelvec.S for machine-mode timer interrupt.
extern void timervec();

// entry.S needs one stack per CPU
__attribute__((aligned(16))) char stack0[PGSIZE * NCPU];

// a scratch area per CPU for machine-mode timer interrupts.
u64 timer_scratch[NCPU][5];

// entry.S jumps here in machine mode on stack0
void start()
{
    // set previous privilege mode (MPP) to Supervisor, for mret.
    // this is the mode where the trap handler that runs in machine mode
    // should jump back to when it returns
    w_mstatus(r_mstatus() & ~MSTATUS_MPP_MASK | MSTATUS_MPP_S);

    // set M Exception Program Counter to main, for mret.
    // mepc contains the previous exception pc
    // requires gcc -mcmodel=medany
    w_mepc((u64)main);

    // disable paging for now
    // if I understand correctly satp points to the page table
    // we are in supervisor mode now and there is usually a page table
    // we dont have one for now so we better set this to 0
    // in machine mode there is never a page table. There is no matp register.
    // We use the Sv39 sceme (3 level page table)
    w_satp(0);

    // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);
    // enable interupts
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

    // configure Physical Memory Protection to give supervisor mode
    // access to all of physical memory.
    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(0xf);

    // ask for clock interrupts.
    timerinit();

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = r_mhartid();
    w_tp(id);

    // switch to supervisor mode and jump to main().
    asm volatile("mret");
}

void timerinit()
{
    // each CPU has a separate source of timer interrupts.
    int id = r_mhartid();

    // ask the CLINT (Core Local Interrupter) for a timer interrupt.
    int interval = 1000000;                                      // cycles; abolut 1/10th second in qemu.
    *(u64 *)CLINT_MTIMECMP(id) = *(u64 *)CLINT_MTIME + interval; // save the time for next timer interrupt

    // prepare information in scratch[] for timervec.
    // sratch[0..2] : space for timervec to save registers.
    // scratch[3] : address of CLINT MTIMECMP register.
    // scratch[4] : desired interval (in cycles) between timer interrupts
    u64 *scratch = &timer_scratch[id][0];
    scratch[3] = CLINT_MTIMECMP(id);
    scratch[4] = interval;
    w_mscratch((u64)scratch);

    // set the machine-mode trap handler.
    w_mtvec((u64)timervec);

    // enable machine-mode interrupts.
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    // enable machine-mode timer interrupts.
    w_mie(r_mie() | MIE_MTIE);
}