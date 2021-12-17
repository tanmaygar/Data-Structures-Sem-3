# Data Structures Assignment 3
## Tanmay Garg CS20BTECH11063

### Build and Running the file
```bash
$ gcc *.c *.h -o main
$ ./main
```

### Implementation
- A normal BST and a 2-3-4 BTREE has been implemented
- Each memory cell has been assumed to able to accomodate space for one node of BTREE or BST
- BTREE and BST are made with separate implementations of DISK READ and DISK WRITE
- Before going into recursion, a disk write operation is being performed to write the changes first or in some cases the disk write for parent node is done in the base step of recursion
- FAST READ is assumed to be 1 unit
- DISK READ and DISK WRITE is assumed to be 10 units
- The above operations can be changed separately in each header file