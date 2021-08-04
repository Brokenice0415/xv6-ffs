# FFS

Fast File System at xv6

### Design target

- use **data groups** to separate SFS's file system blocks
- files in the same root directory preferred to be saved in the same data group
- a new directory preferred to be allocated in the most empty data group
- optimization in big file cases (not realized)



> Disk layout:
>
> [ boot block | super block | log
>
> ​        | [inode blocks | free bit map | data blocks] (group)
>
>  	   | [inode blocks | free bit map | data blocks] (group)
>
> ​        | ... ]



## Modified to satisfy FFS

modified files

> param.h										global definitions
>
> defs.h											global function definitions
>
> fs.h												fs definitions
>
> fs.c												 fs functions
>
> mkfs.c										   to create the initial fs
>
> sysfile.c										 file system implements



### Super block

```c
// FFS's super block layout
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
```

### Definition of bno

FFS = SFS: 

​	bno = the data block number in FS

**Modified method to find bitmap bno through data bno**



### Definition of inum

FFS = SFS: 

​	inum = the inode number in FS

**Modified method to find block containing the inode through inum**



### fs.c

```c
// functions of group
int		igno(int inum);							// make IGROUP(inum, sb) accessible to sysfile.c
int		groupnum(int dev);						// make sb.groupnum accessible to sysfile.c
int		fdgroup(int dev);						// find a most empty group, 0 means no empty group
// functions of block
static uint
	ballocg(uint dev, int gno);					// replace balloc, alloc a block in a specific group
static uint
	bmap(struct inode *ip, uint bn);			// modify the use of balloc
// functions of inode
struct inode*
	iallocg(uint dev, short type, int gno);		// replace ialloc, alloc a inode in a specific group



### mkfs.c

```c
int
	main(int argc, char *argv[]);				// modify the create and initialization of fs
void	balloc(int used);						// freeblock split into groups
void
	iappend(uint inum, void *xp, int n);		// freeblock split into groups
```



### sysfile.c

```c
// 2 cases of the method of create
// create file
// or create directory or device
static struct inode*
	create(char *path, short type, short major, short minor);
```



## Version 1.0

### Specification

- use **data groups** to separate SFS's file system blocks
- files in the same root directory preferred to be saved in the same data group
- a new directory preferred to be allocated in the most empty data group
- <u>if group have no free data block, find a data block in the next group</u>



### Limitations

- when using mkfs to create initial file system image, the size of fs must be less than that of  1 group
- using just one disk, which making the use of struct sb simple

