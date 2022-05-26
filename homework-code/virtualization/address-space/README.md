# Questions

1. The first Linux tool you should check out is the very simple tool free. First, type man free and read its entire manual page; it’s short, don’t worry!

2. Now, run free, perhaps using some of the arguments that might be useful (e.g., -m, to display memory totals in megabytes). How much memory is in your system? How much is free? Do these numbers match your intuition?

| Type	|total  |       used   |     free   |   shared  | buff/cache |  available |
|-------|-------|--------------|------------|-----------|------------|------------|
|Mem    | 7.7Gi |      1.5Gi   |    3.7Gi   |    313Mi  |     2.5Gi  |     5.7Gi  |
|Swap   | 1.9Gi |         0B   |    1.9Gi   |           |            |            |


There is 7.7GiB of memory in my system, 3.7GiB of free memory.

3. Next, create a little program that uses a certain amount of memory, called memory-user.c. This program should take one command-line argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.

4. Now, while running your memory-user program, also (in a different terminal window, but on the same machine) run the free tool. How do the memory usage totals change when your program is running? How about when you kill the memory-user program? Do the numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?

	For a program with small memory allocation(eg. 100 MiB), used memory is increased and decreased by almost the same amount of the memory I allocated. But when I try to allocate large number(eg. 200GiB) larger than the memory size, used memory is increased slowly and stops increasing at certain size, And there still is free space. 

5. Let’s try one more tool, known as pmap. Spend some time, and read the pmap manual page in detail.

6. To use pmap, you have to know the process ID of the process you’re interested in. Thus, first run ```ps auxw``` to see a list of all processes; then, pick an interesting one, such as a browser. You can also use your memory-user program in this case (indeed, you can even have that program call getpid() and print out its PID for your convenience).

7. Now run pmap on some of these processes, using various flags (like -X) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of code/stack/heap?

	```Rss Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping```
	- Rss: resident set size, the portion of memory occupied by a process that is held in main memory (RAM)
	- Pss: [proportional set size]([)https://en.wikipedia.org/wiki/Proportional_set_size)
	- Others: [man 5 proc](https://man7.org/linux/man-pages/man5/proc.5.html)

8. Finally, let’s run pmap on your memory-user program, with different amounts of used memory. What do you see here? Does the output from pmap match your expectations?
	```
	152241:   ./a.out 1

		Address  Perm   Offset Device   Inode Size  Rss  Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
    55d6e9e4e000 r--p 00000000  08:15 1053527    4    4    4          4         0        0              0             0              0               0    0       0      0           0 a.out
    55d6e9e4f000 r-xp 00001000  08:15 1053527    4    4    4          4         0        0              0             0              0               0    0       0      0           0 a.out
    55d6e9e50000 r--p 00002000  08:15 1053527    4    0    0          0         0        0              0             0              0               0    0       0      0           0 a.out
    55d6e9e51000 r--p 00002000  08:15 1053527    4    4    4          4         4        0              0             0              0               0    0       0      0           0 a.out
    55d6e9e52000 rw-p 00003000  08:15 1053527    4    4    4          4         4        0              0             0              0               0    0       0      0           0 a.out
    55d6eb4fc000 rw-p 00000000  00:00       0  132    4    4          4         4        0              0             0              0               0    0       0      0           0 [heap]
    7ff0f6b4e000 rw-p 00000000  00:00       0 1028 1028 1028       1028      1028        0              0             0              0               0    0       0      0           0 
    7ff0f6c4f000 r--p 00000000  08:15 1975427  136  136    6        136         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7ff0f6c71000 r-xp 00022000  08:15 1975427 1504  636   33        636         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7ff0f6de9000 r--p 0019a000  08:15 1975427  312  156    8        156         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7ff0f6e37000 r--p 001e7000  08:15 1975427   16   16   16         16        16        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7ff0f6e3b000 rw-p 001eb000  08:15 1975427    8    8    8          8         8        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7ff0f6e3d000 rw-p 00000000  00:00       0   24   20   20         20        20        0              0             0              0               0    0       0      0           0 
    7ff0f6e54000 r--p 00000000  08:15 1975423    4    4    0          4         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7ff0f6e55000 r-xp 00001000  08:15 1975423  140  140    6        140         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7ff0f6e78000 r--p 00024000  08:15 1975423   32   32    1         32         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7ff0f6e81000 r--p 0002c000  08:15 1975423    4    4    4          4         4        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7ff0f6e82000 rw-p 0002d000  08:15 1975423    4    4    4          4         4        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7ff0f6e83000 rw-p 00000000  00:00       0    4    4    4          4         4        0              0             0              0               0    0       0      0           0 
    7fffa36d8000 rw-p 00000000  00:00       0  132   16   16         16        16        0              0             0              0               0    0       0      0           0 [stack]
    7fffa37c5000 r--p 00000000  00:00       0   16    0    0          0         0        0              0             0              0               0    0       0      0           0 [vvar]
    7fffa37c9000 r-xp 00000000  00:00       0    8    4    0          4         0        0              0             0              0               0    0       0      0           0 [vdso]
	ffffffffff600000 --xp 00000000  00:00       0    4    0    0          0         0        0              0             0              0               0    0       0      0           0 [vsyscall]
                                              ==== ==== ==== ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== 
                                              3528 2228 1174       2228      1112        0              0             0              0               0    0       0      0           0 KB 
	```
	```
	152192:   ./a.out 10

         Address Perm   Offset Device   Inode  Size   Rss   Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
    55fa429bb000 r--p 00000000  08:15 1053527     4     4     4          4         0        0              0             0              0               0    0       0      0           0 a.out
    55fa429bc000 r-xp 00001000  08:15 1053527     4     4     4          4         0        0              0             0              0               0    0       0      0           0 a.out
    55fa429bd000 r--p 00002000  08:15 1053527     4     0     0          0         0        0              0             0              0               0    0       0      0           0 a.out
    55fa429be000 r--p 00002000  08:15 1053527     4     4     4          4         4        0              0             0              0               0    0       0      0           0 a.out
    55fa429bf000 rw-p 00003000  08:15 1053527     4     4     4          4         4        0              0             0              0               0    0       0      0           0 a.out
    55fa4301e000 rw-p 00000000  00:00       0   132     4     4          4         4        0              0             0              0               0    0       0      0           0 [heap]
    7f0457ff8000 rw-p 00000000  00:00       0 10244 10244 10244      10244     10244        0              0             0              0               0    0       0      0           0 
    7f04589f9000 r--p 00000000  08:15 1975427   136   136     6        136         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7f0458a1b000 r-xp 00022000  08:15 1975427  1504   724    40        724         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7f0458b93000 r--p 0019a000  08:15 1975427   312   116     5        116         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7f0458be1000 r--p 001e7000  08:15 1975427    16    16    16         16        16        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7f0458be5000 rw-p 001eb000  08:15 1975427     8     8     8          8         8        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7f0458be7000 rw-p 00000000  00:00       0    24    20    20         20        20        0              0             0              0               0    0       0      0           0 
    7f0458bfe000 r--p 00000000  08:15 1975423     4     4     0          4         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7f0458bff000 r-xp 00001000  08:15 1975423   140   140     6        140         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7f0458c22000 r--p 00024000  08:15 1975423    32    32     1         32         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7f0458c2b000 r--p 0002c000  08:15 1975423     4     4     4          4         4        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7f0458c2c000 rw-p 0002d000  08:15 1975423     4     4     4          4         4        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7f0458c2d000 rw-p 00000000  00:00       0     4     4     4          4         4        0              0             0              0               0    0       0      0           0 
    7ffc013ea000 rw-p 00000000  00:00       0   132    12    12         12        12        0              0             0              0               0    0       0      0           0 [stack]
    7ffc014fb000 r--p 00000000  00:00       0    16     0     0          0         0        0              0             0              0               0    0       0      0           0 [vvar]
    7ffc014ff000 r-xp 00000000  00:00       0     8     4     0          4         0        0              0             0              0               0    0       0      0           0 [vdso]
	ffffffffff600000 --xp 00000000  00:00       0     4     0     0          0         0        0              0             0              0               0    0       0      0           0 [vsyscall]
                                              ===== ===== ===== ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== 
                                              12744 11488 10390      11488     10324        0              0             0              0               0    0       0      0           0 KB 	
	```

	```
	152332:   ./a.out 80000
         Address Perm   Offset Device   Inode    Size     Rss     Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
    55e8e3a11000 r--p 00000000  08:15 1053527       4       4       4          4         0        0              0             0              0               0    0       0      0           0 a.out
    55e8e3a12000 r-xp 00001000  08:15 1053527       4       4       4          4         0        0              0             0              0               0    0       0      0           0 a.out
    55e8e3a13000 r--p 00002000  08:15 1053527       4       0       0          0         0        0              0             0              0               0    0       0      0           0 a.out
    55e8e3a14000 r--p 00002000  08:15 1053527       4       4       4          4         4        0              0             0              0               0    0       0      0           0 a.out
    55e8e3a15000 rw-p 00003000  08:15 1053527       4       4       4          4         4        0              0             0              0               0    0       0      0           0 a.out
    55e8e3a19000 rw-p 00000000  00:00       0     132       4       4          4         4        0              0             0              0               0    0       0      0           0 [heap]
    7fd3da18f000 rw-p 00000000  00:00       0 2228228 2228228 2228228    2228228   2228228        0              0             0              0               0    0       0      0           0 
    7fd462190000 r--p 00000000  08:15 1975427     136     136       6        136         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7fd4621b2000 r-xp 00022000  08:15 1975427    1504     632      33        632         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7fd46232a000 r--p 0019a000  08:15 1975427     312     152       7        152         0        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7fd462378000 r--p 001e7000  08:15 1975427      16      16      16         16        16        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7fd46237c000 rw-p 001eb000  08:15 1975427       8       8       8          8         8        0              0             0              0               0    0       0      0           0 libc-2.31.so
    7fd46237e000 rw-p 00000000  00:00       0      24      20      20         20        20        0              0             0              0               0    0       0      0           0 
    7fd462395000 r--p 00000000  08:15 1975423       4       4       0          4         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7fd462396000 r-xp 00001000  08:15 1975423     140     140       6        140         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7fd4623b9000 r--p 00024000  08:15 1975423      32      32       1         32         0        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7fd4623c2000 r--p 0002c000  08:15 1975423       4       4       4          4         4        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7fd4623c3000 rw-p 0002d000  08:15 1975423       4       4       4          4         4        0              0             0              0               0    0       0      0           0 ld-2.31.so
    7fd4623c4000 rw-p 00000000  00:00       0       4       4       4          4         4        0              0             0              0               0    0       0      0           0 
    7fffbeea6000 rw-p 00000000  00:00       0     132      16      16         16        16        0              0             0              0               0    0       0      0           0 [stack]
    7fffbef6e000 r--p 00000000  00:00       0      16       0       0          0         0        0              0             0              0               0    0       0      0           0 [vvar]
    7fffbef72000 r-xp 00000000  00:00       0       8       4       0          4         0        0              0             0              0               0    0       0      0           0 [vdso]
	ffffffffff600000 --xp 00000000  00:00       0       4       0       0          0         0        0              0             0              0               0    0       0      0           0 [vsyscall]
                                              ======= ======= ======= ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== 
                                              2230728 2229420 2228373    2229420   2228312        0              0             0              0               0    0       0      0           0 KB 
	```