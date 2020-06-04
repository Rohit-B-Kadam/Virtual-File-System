# VIRTUAL FILE SYSTEM (VFS)
This is Research Oriented Project to simulate the existing Unix File system.

## Implement following Filesystem datastructure to build VFS
1. Superblock
1. UArea (1)	
1. UFDT (array) 
1. File Table (LinkList)
1. Incore Inode table (limited linklist)
1. Buffer Cache	(limited linklist)

we have create a shell which accept the command from the user and Also we have implement our own system call.
```
1. open
2. create
3. read
4. write
5. lseek
6. rm
7. stat
8. ls
9. man command for help
```
___

## Working of system class

### create

- Allocating free Inode.
- Get free index in UFDT.
- Create new node in Filetable and set address in UFDT.
- Fill information in __Filetable__ (count = 1, mode, offset = 0)
- Get free block of data.
- Fill file information in Inode.

### rm

- Get inode number for File Name.
- linkcount--

### open

- Get Inode Number. 
- Check the permission satisfy.
- Get free UFDT index.
- Create entry File Table index.
- Get Iode Address.
- Fill FileTable Info

### close

- reference_count--
- remove the entry from FileTable.
- set null in UFDT

### read

- check if the open mode satisfy "READ" or not
- read the data from the offset(file table) position till the size mention. 
- copy to provide the char pointer

### write

- check if the open mode satisfy "WRITE" or not
- write the data from the offset(file table) position. 

### lseek

- change the offset to the given offset.

### stat

- Set Inode Number.
- Display information of that inode.

### ls

- Traverse all Inode List

___
