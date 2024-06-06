struct buf;
struct context;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;

// proc.c
int cpuid(void);
void exit(int);
int fork(void);
int growproc(int);
void proc_mapstacks(pagetable_t);
pagetable_t proc_pagetable(struct proc *);
void proc_freepagetable(pagetable_t, u64);
int kill(int);
int killed(struct proc *);
void setkilled(struct proc *);
struct cpu *mycpu(void);
struct cpu *getmycpu(void);
struct proc *myproc();
void procinit(void);
void scheduler(void) __attribute__((noreturn));
void sched(void);
void sleep(void *, struct spinlock *);
void userinit(void);
int wait(u64);
void wakeup(void *);
void yield(void);
int either_copyout(int user_dst, u64 dst, void *src, u64 len);
int either_copyin(void *dst, int user_src, u64 src, u64 len);
void procdump(void);

// swtch.S
void swtch(struct context *, struct context *);

// string.c
int memcmp(const void *, const void *, u64);
void *memmove(void *, const void *, u64);
void *memset(void *, int, u64);
char *safestrcpy(char *, const char *, int);
int strlen(const char *);
int strncmp(const char *, const char *, u64);
char *strncpy(char *, const char *, int);

// trap.c
extern u32 ticks;
extern struct spinlock tickslock;

// uart.c
void uartinit(void);
void uartintr(void);
void uartputc(int);
void uartputc_sync(int);
int uartgetc(void);

// vm.c
void kvminit(void);
void kvminithart(void);
void kvmmap(pagetable_t, u64, u64, u64, int);
int mappages(pagetable_t, u64, u64, u64, int);
pagetable_t uvmcreate(void);
void uvmfirst(pagetable_t, uchar *, u64);
u64 uvmalloc(pagetable_t, u64, u64, int);
u64 uvmdealloc(pagetable_t, u64, u64);
int uvmcopy(pagetable_t, pagetable_t, u64);
void uvmfree(pagetable_t, u64);
void uvmunmap(pagetable_t, u64, u64, int);
void uvmclear(pagetable_t, u64);
pte_t *walk(pagetable_t, u64, int);
u64 walkaddr(pagetable_t, u64);
int copyout(pagetable_t, u64, char *, u64);
int copyin(pagetable_t, char *, u64, u64);
int copyinstr(pagetable_t, char *, u64, u64);