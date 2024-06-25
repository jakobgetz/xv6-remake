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

// bio.c
void binit(void);
struct buf *bread(u32, u32);
void brelse(struct buf *);
void bwrite(struct buf *);
void bpin(struct buf *);
void bunpin(struct buf *);

// console.c
void consoleinit(void);
void consoleintr(int);
void consputc(int);

// fs.c
void fsinit(int);
int dirlink(struct inode *, char *, u32);
struct inode *dirlookup(struct inode *, char *, u32 *);
struct inode *ialloc(u32, short);
struct inode *idup(struct inode *);
void iinit();
void ilock(struct inode *);
void iput(struct inode *);
void iunlock(struct inode *);
void iunlockput(struct inode *);
void iupdate(struct inode *);
int namecmp(const char *, const char *);
struct inode *namei(char *);
struct inode *nameiparent(char *, char *);
int readi(struct inode *, int, u64, u32, u32);
void stati(struct inode *, struct stat *);
int writei(struct inode *, int, u64, u32, u32);
void itrunc(struct inode *);

// kalloc.c
void *kalloc(void);
void kfree(void *);
void kinit(void);

// log.c
void initlog(int, struct superblock *);
void log_write(struct buf *);
void begin_op(void);
void end_op(void);

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

// spinlock.c
void acquire(struct spinlock *);
int holding(struct spinlock *);
void initlock(struct spinlock *, char *);
void release(struct spinlock *);
void push_off(void);
void pop_off(void);

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
void uvmfirst(pagetable_t, u8 *, u64);
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

// plic.c
void plicinit(void);
void plicinithart(void);
int plic_claim(void);
void plic_complete(int);

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x) / sizeof((x)[0]))