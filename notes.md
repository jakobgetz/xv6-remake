# Virtual Memory

**On Startup (main.c):** 
1. Jump to supervisor mode (satp = 0)
2. kinit(): add all 64k datapages to freelist from. Address after kernal to PHYSTOP
3. kvminit(): create kernel page table


# What happens on load
Risc-V processor follows a specific algorithm to find address. (Sv39)
1. Read satp register to get physical address to top level page table
2. Extract index bit virtual adress and index into page table and get page table entry
3. Transform page table entry to physical address to get next level page table
4. Repeat until last level page table enty. That one is the actual physical address.
=> This means page table needs to be set up and layed out in memory before that.
To disable virtual memory set satp to 0