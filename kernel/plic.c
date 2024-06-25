#include "memlayout.h"
#include "defs.h"
#include "types.h"

//
// the riscv Platform Level Interrupt Controller (PLIC)
//

void plicintit(void)
{
    // set desired IRQ priorities non-zero (otherwise disabled).
    *(u32 *)(PLIC + UART0_IRQ * 4) = 1;
    *(u32 *)(PLIC + VIRTIO0_IRQ * 4) = 1;
}

void plicinithart(void)
{
    int hart = cpuid();

    // set enable bits for this hart's S-mode
    // for the uart and virtio disk.
    *(u32 *)PLIC_SENABLE(hart) = (1 << UART0_IRQ) | (1 << VIRTIO0_IRQ);

    // set this hart's S-mode priority threshold to 0
    *(u32 *)PLIC_SPRIORITY(hart) = 0;
}

// ask the PLIC what interrupt we should serve.
int plic_claim(void)
{
    int hart = cpuid();
    int irq = *(u32 *)PLIC_SCLAIM(hart);
    return irq; // will be zero when already claimed
}

// tell the PLIC we've served this IRQ.
void plic_complete(int irq)
{
    int hart = cpuid();
    *(u32 *)PLIC_SCLAIM(hart) = irq;
}