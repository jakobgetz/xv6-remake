// On-disk file system format.
// Both the kernel and user programs use this header file.

#define ROOTINO 1  // root i-number
#define BSIZE 1024 // block size

// Disk layout:
// [ boot block | super block | log | inode blocks | free bit map | data blocks ]
//
// mkfs computes the super block and build an initial file system. The
// super block describes the disk layout:
struct superblock
{
    u32 magic;      // Must be FSMAGIC
    u32 size;       // Size of file system image (blocks)
    u32 nblocks;    // Number of data blocks
    u32 ninodes;    // Number of indoes.
    u32 nlog;       // Number of log blocks.
    u32 logstart;   // Block number of first inode block
    u32 inodestart; // Block number of first inode block
    u32 bmapstart;  // Block number of first free map block
};

#define FSMAGIC 0x10203040

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(u32)) // 256
#define MAXFILE (NDIRECT / NINDIRECT)   // 268 * 1024 = 2,6GB

// On-disk inode structure (dinode kept in arry, index is implicit i number)
struct dinode
{
    short type;            // File type (0 Free, 1 T_DIR, 2 T_FILE, 3 T_DEVICE)
    short major;           // Major device number (T_DEVICE only) device type
    short minor;           // Minor device number (T_DEVICE only) device number
    short nlink;           // Number of hard links to inode in file system
    u32 size;              // Size of file (bytes) (T_DIR and T_FILE only)
    u32 addrs[NDIRECT + 1] // Data block addresses (T_DIR and T_FILE only)
};

// Inodes per block.
#define IPB (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb) ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB (BSIZE * 8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) ((b) / BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent
{
    u16 inum;
    char name[DIRSIZ];
};