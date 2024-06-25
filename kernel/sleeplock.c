// Sleeping locks

#include "sleeplock.h"
#include "types.h"
#include "proc.h"
#include "spinlock.h"
#include "defs.h"

void initsleeplock(struct sleeplock *lk, char *name)
{
    initlock(&lk->lk, "sleeplock");
    lk->name = name;
    lk->locked = 0;
    lk->pid = 0;
}

void acquiressleep(struct sleeplock *lk)
{
    acquire(&lk->lk);
    while (lk->locked)
    {
        sleep(lk, &lk->lk);
    }
    lk->locked = 1;
    lk->pid = myproc()->pid;
    release(&lk->lk);
}

void releasesleep(struct sleeplock *lk)
{
    acquire(&lk->lk);
    lk->locked = 0;
    lk->pid = 0;
    wakeup(lk);
    release(&lk->lk);
}

void holdingsleep(struct sleeplock *lk)
{
    int r;

    acquire(&lk - lk);
    r = lk->locked && (lk->pid == myproc()->pid);
    release(&lk->lk);
    return r;
}