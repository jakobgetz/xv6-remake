#include "types.h"
#include "riscv.h"
#include "defs.h"

/*
 * the kernel's page table.
 */
pagetable_t kernel_pagetable;

// Copy from user to kernel
// Copy len bytes to dst from virtual address srcva in agiven page table.
// Return 0 on success, -1 on error
int copyin(pagetable_t pagetable, char *dst, u64 srcva, u64 len)
{
    u64 n, va0, pa0;

    while (len > 0)
    {
        va0 = PGROUNDDOWN(srcva);
        pa0 = walkaddr(pagetable, va0);
        // continue here soon
    }
}

// Make a direct-map page table for the kernel.
pagetable_t kvmake(void)
{
    pagetable_t kgbtbl;

    kgbtbl = (pagetable_t)kalloc();
}

// Initialize the one kernel_pagetable
void kvminit(void)
{
    kernel_pagetable = kvmmake();
}