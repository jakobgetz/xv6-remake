// Long-term locks for processes
struct sleeplock
{
    u64 locked;         // Is the lock held?
    struct spinlock lk; // spinlock protecting this sleep lock

    // For debugging:
    char *name;         // Name of lock.
    int pid;            // Process holding lock
}