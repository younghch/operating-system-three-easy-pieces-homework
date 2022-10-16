
# Overview

This section introduces `raid.py`, a simple RAID simulator you can use to shore
up your knowledge of how RAID systems work. It has a number of options, as we
see below:

```sh
prompt> ./raid.py -h
Usage: raid.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -D NUMDISKS, --numDisks=NUMDISKS
                        number of disks in RAID
  -C CHUNKSIZE, --chunkSize=CHUNKSIZE
                        chunk size of the RAID
  -n NUMREQUESTS, --numRequests=NUMREQUESTS
                        number of requests to simulate
  -S SIZE, --reqSize=SIZE
                        size of requests
  -W WORKLOAD, --workload=WORKLOAD
                        either "rand" or "seq" workloads
  -w WRITEFRAC, --writeFrac=WRITEFRAC
                        write fraction (100->all writes, 0->all reads)
  -R RANGE, --randRange=RANGE
                        range of requests (when using "rand" workload)
  -L LEVEL, --level=LEVEL
                        RAID level (0, 1, 4, 5)
  -5 RAID5TYPE, --raid5=RAID5TYPE
                        RAID-5 left-symmetric "LS" or left-asym "LA"
  -r, --reverse         instead of showing logical ops, show physical
  -t, --timing          use timing mode, instead of mapping mode
  -c, --compute         compute answers for me
```

In its basic mode, you can use it to understand how the different RAID levels
map logical blocks to underlying disks and offsets. For example, let's say we
wish to see how a simple striping RAID (RAID-0) with four disks does this
mapping.

```sh
prompt> ./raid.py -n 5 -L 0 -R 20 
...
LOGICAL READ from addr:16 size:4096
  Physical reads/writes?

LOGICAL READ from addr:8 size:4096
  Physical reads/writes?

LOGICAL READ from addr:10 size:4096
  Physical reads/writes?

LOGICAL READ from addr:15 size:4096
  Physical reads/writes?

LOGICAL READ from addr:9 size:4096
  Physical reads/writes?
```

In this example, we simulate five requests (-n 5), specifying RAID level zero
(-L 0), and restrict the range of random requests to just the first twenty
blocks of the RAID (-R 20). The result is a series of random reads to the
first twenty blocks of the RAID; the simulator then asks you to guess which
underlying disks/offsets were accessed to service the request, for each
logical read.

In this case, calculating the answers is easy: in RAID-0, recall that the
underlying disk and offset that services a request is calculated via modulo
arithmetic: 

```sh
disk   = address % number_of_disks
offset = address / number_of_disks
```

Thus, the first request to 16 should be serviced by disk 0, at offset 4. And
so forth.  You can, as usual see the answers (once you've computed them!), by
using the handy "-c" flag to compute the results.

```sh
prompt> ./raid.py -R 20 -n 5 -L 0 -c
...
LOGICAL READ from addr:16 size:4096
  read  [disk 0, offset 4]   

LOGICAL READ from addr:8 size:4096
  read  [disk 0, offset 2]   

LOGICAL READ from addr:10 size:4096
  read  [disk 2, offset 2]   

LOGICAL READ from addr:15 size:4096
  read  [disk 3, offset 3]   

LOGICAL READ from addr:9 size:4096
  read  [disk 1, offset 2]   
```

Because we like to have fun, you can also do this problem in reverse, with the
"-r" flag. Running the simulator this way shows you the low-level disk reads
and writes, and asks you to reverse engineer which logical request must have
been given to the RAID:

```sh
prompt> ./raid.py -R 20 -n 5 -L 0 -r
...
LOGICAL OPERATION is ?
  read  [disk 0, offset 4]   

LOGICAL OPERATION is ?
  read  [disk 0, offset 2]   

LOGICAL OPERATION is ?
  read  [disk 2, offset 2]   

LOGICAL OPERATION is ?
  read  [disk 3, offset 3]   

LOGICAL OPERATION is ?
  read  [disk 1, offset 2]   
```

You can again use -c to show the answers. To get more variety, a
different random seed (-s) can be given. 

Even further variety is available by examining different RAID levels.
In the simulator, RAID-0 (block striping), RAID-1 (mirroring), RAID-4
(block-striping plus a single parity disk), and RAID-5 (block-striping with
rotating parity) are supported.

In this next example, we show how to run the simulator in mirrored mode. We
show the answers to save space:

```sh
prompt> ./raid.py -R 20 -n 5 -L 1 -c
...
LOGICAL READ from addr:16 size:4096
  read  [disk 0, offset 8]   
 
LOGICAL READ from addr:8 size:4096
  read  [disk 0, offset 4]   

LOGICAL READ from addr:10 size:4096
  read  [disk 1, offset 5]   

LOGICAL READ from addr:15 size:4096
  read  [disk 3, offset 7]   

LOGICAL READ from addr:9 size:4096
  read  [disk 2, offset 4]   
```

You might notice a few things about this example. First, the mirrored
RAID-1 assumes a striped layout (which some might call RAID-10, or
stripe of mirrors), where logical block 0 is mapped to the 0th block
of disks 0 and 1, logical block 1 is mapped to the 0th blocks of disks
2 and 3, and so forth (in this four-disk example).  Second, when
reading a single block from a mirrored RAID system, the RAID has a
choice of which of two blocks to read. In this simulator, we use a
relatively silly way: for even-numbered logical blocks, the RAID
chooses the even-numbered disk in the pair; the odd disk is used for
odd-numbered logical blocks. This is done to make the results of each
run easy to guess for you (instead of, for example, a random choice).

We can also explore how writes behave (instead of just reads) with the -w
flag, which specifies the "write fraction" of a workload, i.e., the fraction
of requests that are writes. By default, it is set to zero, and thus the
examples so far were 100\% reads. Let's see what happens to our mirrored RAID
when some writes are introduced:

```sh
prompt> ./raid.py -R 20 -n 5 -L 1 -w 100 -c
... 
LOGICAL WRITE to  addr:16 size:4096
  write [disk 0, offset 8]     write [disk 1, offset 8]   

LOGICAL WRITE to  addr:8 size:4096
  write [disk 0, offset 4]     write [disk 1, offset 4]   

LOGICAL WRITE to  addr:10 size:4096
  write [disk 0, offset 5]     write [disk 1, offset 5]   

LOGICAL WRITE to  addr:15 size:4096
  write [disk 2, offset 7]     write [disk 3, offset 7]   

LOGICAL WRITE to  addr:9 size:4096
  write [disk 2, offset 4]     write [disk 3, offset 4]   
```

With writes, instead of generating just a single low-level disk operation, the
RAID must of course update both disks, and hence two writes are issued. 
Even more interesting things happen with RAID-4 and RAID-5, as you might
guess; we'll leave the exploration of such things to you in the questions
below.

The remaining options are discovered via the help flag. They are:

```sh
Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -D NUMDISKS, --numDisks=NUMDISKS
                        number of disks in RAID
  -C CHUNKSIZE, --chunkSize=CHUNKSIZE
                        chunk size of the RAID
  -n NUMREQUESTS, --numRequests=NUMREQUESTS
                        number of requests to simulate
  -S SIZE, --reqSize=SIZE
                        size of requests
  -W WORKLOAD, --workload=WORKLOAD
                        either "rand" or "seq" workloads
  -w WRITEFRAC, --writeFrac=WRITEFRAC
                        write fraction (100->all writes, 0->all reads)
  -R RANGE, --randRange=RANGE
                        range of requests (when using "rand" workload)
  -L LEVEL, --level=LEVEL
                        RAID level (0, 1, 4, 5)
  -5 RAID5TYPE, --raid5=RAID5TYPE
                        RAID-5 left-symmetric "LS" or left-asym "LA"
  -r, --reverse         instead of showing logical ops, show physical
  -t, --timing          use timing mode, instead of mapping mode
  -c, --compute         compute answers for me
```

The -C flag allows you to set the chunk size of the RAID, instead of using the
default size of one 4-KB block per chunk. The size of each request can be
similarly adjusted with the -S flag. The default workload accesses random
blocks; use -W sequential to explore the behavior of sequential accesses. With
RAID-5, two different layout schemes are available, left-symmetric and
left-asymmetric; use -5 LS or -5 LA to try those out with RAID-5 (-L 5).

Finally, in timing mode (-t), the simulator uses an incredibly simple disk
model to estimate how long a set of requests takes, instead of just focusing
on mappings. In this mode, a random request takes 10 milliseconds, whereas a
sequential request takes 0.1 milliseconds.  The disk is assumed to have a tiny
number of blocks per track (100), and a similarly small number of tracks
(100). You can thus use the simulator to estimate RAID performance under some
different workloads.

# Questions
1. Use the simulator to perform some basic RAID mapping tests. Run with different levels (0, 1, 4, 5) and see if you can figure out the mappings of a set of requests. For RAID-5, see if you can figure out the difference between left-symmetric and left-asymmetric layouts. Use some different random seeds to generate different problems than above.
  ![raid layouts](./raid5-layouts.png)
  blockSize 4096, numDisks 4, chunkSize 4k
  ```

  python3 raid.py -L 0 -R 20 -n 5 -s 42 -w 20
  python3 raid.py -L 1 -R 20 -n 5 -s 42 -w 20
  python3 raid.py -L 4 -R 20 -n 5 -s 42 -w 20
  python3 raid.py -L 5 -R 20 -n 5 -s 42 -w 20 -5 LS
  python3 raid.py -L 5 -R 20 -n 5 -s 42 -w 20 -5 LA

  LOGICAL WRITE to  addr:12 size:4096
    raid 0: write [disk 0, offset 3]  
    raid 1: write [disk 0, offset 6], [disk 1, offset 6]  
    raid 4: read and write [disk 0, offset 4], [disk 3, offset 4]  
    raid 5(LS): read and write [disk 0, offset 4], [disk 3, offset 4]
    raid 5(LA): read and write [disk 0, offset 4], [disk 3, offset 4]

  LOGICAL READ from addr:5 size:4096
    raid 0: read  [disk 1, offset 1]  
    raid 1: read  [disk 2, offset 2]  
    raid 4: read  [disk 2, offset 1]  
    raid 5(LS): read  [disk 1, offset 1]
    raid 5(LA): read  [disk 3, offset 1]

  LOGICAL READ from addr:14 size:4096
    raid 0: read  [disk 2, offset 3]  
    raid 1: read  [disk 1, offset 7]  
    raid 4: read  [disk 2, offset 4]  
    raid 5(LS): read  [disk 2, offset 4]
    raid 5(LA): read  [disk 2, offset 4]

  LOGICAL WRITE to  addr:17 size:4096
    raid 0: write [disk 1, offset 4]  
    raid 1: write [disk 2, offset 8], [disk 3, offset 8]  
    raid 4: read and write [disk 2, offset 5], [disk 3, offset 5]  
    raid 5(LS): read and write [disk 1, offset 5], [disk 2, offset 5]
    raid 5(LA): read and write [disk 3, offset 5], [disk 2, offset 5]

  LOGICAL WRITE to  addr:8 size:4096
    raid 0: write [disk 0, offset 2] 
    raid 1: write [disk 0, offset 4], [disk 1, offset 4] 
    raid 4: read and write [disk 2, offset 2], [disk 3, offset 2]  
    raid 5(LS): read and write [disk 0, offset 2], [disk 1, offset 2]
    raid 5(LA): read and write [disk 3, offset 2], [disk 1, offset 2]

  ```
2. Do the same as the first problem, but this time vary the chunk size with -C. How does chunk size change the mappings?
  
  ```
  python3 raid.py -L 0 -R 20 -n 5 -s 42 -w 20 -C 8k
  python3 raid.py -L 1 -R 20 -n 5 -s 42 -w 20 -C 8k
  python3 raid.py -L 4 -R 20 -n 5 -s 42 -w 20 -C 8k
  python3 raid.py -L 5 -R 20 -n 5 -s 42 -w 20 -5 LS -C 8k
  python3 raid.py -L 5 -R 20 -n 5 -s 42 -w 20 -5 LA -C 8k

  LOGICAL WRITE to  addr:12 size:4096
    raid 0: write [disk 2, offset 2]  
    raid 1: write [disk 0, offset 6], [disk 1, offset 6]  
    raid 4: read and write [disk 0, offset 4], [disk 3, offset 4]  
    raid 5(LS): read and write [disk 2, offset 4], [disk 1, offset 4]
    raid 5(LA): read and write [disk 0, offset 4], [disk 1, offset 4]

  LOGICAL READ from addr:5 size:4096
    raid 0: read  [disk 2, offset 1]  
    raid 1: read  [disk 1, offset 3]  
    raid 4: read  [disk 2, offset 1]  
    raid 5(LS): read  [disk 2, offset 1]
    raid 5(LA): read  [disk 2, offset 1]

  LOGICAL READ from addr:14 size:4096
    raid 0: read  [disk 3, offset 2]  
    raid 1: read  [disk 2, offset 6]  
    raid 4: read  [disk 1, offset 4]  
    raid 5(LS): read  [disk 3, offset 4]
    raid 5(LA): read  [disk 2, offset 4]

  LOGICAL WRITE to  addr:17 size:4096
    raid 0: write [disk 0, offset 5]  
    raid 1: write [disk 0, offset 9], [disk 1, offset 9]  
    raid 4: read and write [disk 2, offset 5], [disk 3, offset 5]  
    raid 5(LS): read and write [disk 0, offset 5], [disk 1, offset 5]
    raid 5(LA): read and write [disk 3, offset 5], [disk 1, offset 5]

  LOGICAL WRITE to  addr:8 size:4096
    raid 0: write [disk 0, offset 2] 
    raid 1: write [disk 0, offset 4], [disk 1, offset 4] 
    raid 4: read and write [disk 1, offset 2], [disk 3, offset 2]  
    raid 5(LS): read and write [disk 0, offset 2], [disk 2, offset 2]
    raid 5(LA): read and write [disk 1, offset 2], [disk 2, offset 2]
  ```
3. Do the same as above, but use the -r flag to reverse the nature of each problem.

4. Now use the reverse flag but increase the size of each request with the -S flag. Try specifying sizes of 8k, 12k, and 16k, while varying the RAID level. What happens to the underlying I/O pattern when the size of the request increases? Make sure to try this with the sequential workload too (-W sequential); for what request sizes are RAID-4 and RAID-5 much more I/O efficient?

  RAID-4 and RAID-5 much more I/O efficient for chunk size of 16k Sequential read/write is made on both data and parity disk.

  ```
  python3 raid.py -L 5 -R 20 -n 5 -w 100 -5 LA -C 16k -r -c -W sequential

  0 1
  LOGICAL WRITE to  addr:0 size:4096
    read  [disk 0, offset 0]    read  [disk 3, offset 0]  
    write [disk 0, offset 0]    write [disk 3, offset 0]  

  1 1
  LOGICAL WRITE to  addr:1 size:4096
    read  [disk 0, offset 1]    read  [disk 3, offset 1]  
    write [disk 0, offset 1]    write [disk 3, offset 1]  

  2 1
  LOGICAL WRITE to  addr:2 size:4096
    read  [disk 0, offset 2]    read  [disk 3, offset 2]  
    write [disk 0, offset 2]    write [disk 3, offset 2]  

  3 1
  LOGICAL WRITE to  addr:3 size:4096
    read  [disk 0, offset 3]    read  [disk 3, offset 3]  
    write [disk 0, offset 3]    write [disk 3, offset 3]  

  4 1
  LOGICAL WRITE to  addr:4 size:4096
    read  [disk 1, offset 0]    read  [disk 3, offset 0]  
    write [disk 1, offset 0]    write [disk 3, offset 0]
  ```
  
5. Use the timing mode of the simulator (-t) to estimate the performance of 100 random reads to the RAID, while varying the RAID levels, using 4 disks.

6. Do the same as above, but increase the number of disks. How does the performance of each RAID level scale as the number of disks increases?

7. Do the same as above, but use all writes (-w 100) instead of reads. How does the performance of each RAID level scale now? Can you do a rough estimate of the time it will take to complete the workload of 100 random writes?

8. Run the timing mode one last time, but this time with a sequential workload (-W sequential). How does the performance vary with RAID level, and when doing reads versus writes? How about when varying the size of each request? What size should you write to a RAID when using RAID-4 or RAID-5?