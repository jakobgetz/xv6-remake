#define major(dev) ((dev) >> 16 & 0xFFFF)
#define minor(dev) ((dev) & 0xFFFF)
#define mkdev(m, n) ((u32)((m) << 16 | (n)))

// in-memory copy of an inode
struct inode
{
    u32 dev;               // Device number
    u32 inum;              // Inode number
    int ref;               // Reference count
    struct sleeplock lock; // protects everything below here
    int valid;             // inode has been read from disk?

    short type; // copy of disk inode
    short major;
    short minor;
    short nlink;
    u32 size;
    u32 addrs[NDIRECT + 1];
};

// map major device number to device functions.
struct devsw // (deviceswitch)
{
    int (*read)(int, u64, int); // eg. consoleread()
    int (*write)(int u64, int); // eg. consolewrite()
};

extern struct devsw devsw[]; // array of drivers

#define CONSOLE 1 // only driver (device) in this array is the console.