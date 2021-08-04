// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log
//                    |  [inode blocks | free bit map | data blocks] 
//                    | ... ]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of group image (blocks)

  uint nlog;         // Number of log blocks
  uint ngroup;       // Number of groups blocks
  uint groupnum;     // Number of groups
  uint logstart;     // Block number of first log block
  uint groupstart;   // Block number of first group block

  // Group offset
  uint inodenum;     // Number of inodes;
  uint ninodes;       // Number of inode blocks
  uint nbmap;          // Number of bitmap blocks
  uint ndata;        // Number of data blocks
  uint bmapstart;      // Block offset of first bitmap block in group
  uint datastart;    // Block offset of first data block in group
};

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure (64B)
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Group containing inode i
#define IGROUP(i, sb)     (i/sb.inodenum)

// Block containing inode i
#define IBLOCK(i, sb)     (((i%sb.inodenum) / IPB) + sb.groupstart + IGROUP(i, sb)*BGSIZE)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Group containing bit for block b
#define BGROUP(b, sb)     ((b-sb.groupstart) / BGSIZE)

// Block of free map containing bit for block b
#define BBLOCK(b, sb)     ((((b-sb.groupstart) % BGSIZE - sb.datastart) / BPB) + sb.groupstart + sb.bmapstart + BGROUP(b, sb)*BGSIZE)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};


