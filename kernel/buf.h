struct buf
{
    struct buf *next; // protected by bcache.lock
    struct buf *prev; // protected by bcache.lock
    u64 refcnt;       // protected by bcache.lock
    u64 dev;          // protected by bcache.lock
    u64 blockno;
    struct sleeplock lock; // protects the following fields
    int valid;             // has data been read from disk?
    int disk;              // does disk "own" buf/
    u8 data[BSIZE];
};