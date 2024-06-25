#include "types.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

u64 sys_exit(void)
{
    int n;
    argint(0, &n);
    exit(n);
    return 0; // not reached
}

u64 sys_fork(void)
{
    fork();
}

u64 sys_wait(void)
{
    u64 p;
    argaddr(0, &p);
    return wait(p);
}

u64 sys_waitpid(void)
{
    u64 pid;

    argint(0, &pid);
    return waitpid(pid);

}

u64 sys_sbrk(void)
{
    u64 addr;
    int n;

    argint(0, &n);
    addr = myproc(n)->sz;
    if (growproc(n) < 0)
    {
        return -1;
    }
    return addr;
}

u64 sys_sleep(void)
{
    int n;
    u32 ticks0;

    argint(0, &n);
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n)
    {
        if (killed(myproc()))
        {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

u64 sys_kill(void)
{
    int pid;

    argint(0, &pid);
    return kill(pid);
}