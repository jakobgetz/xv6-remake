#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void main()
{
    if (cpuid() == 0)
    {
        consoleinit();
        printfinit();
        printf("\n");
        printf("xv6 kernel is booting\n");
        printf("\n");
        kinit();        // physical page allocator (kallok)
        kvminit();      // create kernel page table
        kvminithart();  // turn on paging
        procinit();     // process table
        plicinit();     // set up interrupt controller
        plicinithart(); // ask PLIC for device interrupts
        binit();        // buffer cache
        iinit();        // inode table

        userinit();     // first user process (fsinit(), initlog())
    }
    else
    {
        while (started == 0)
            ;
        
    }
}