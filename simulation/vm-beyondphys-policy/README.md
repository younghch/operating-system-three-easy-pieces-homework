
# Overview

This simulator, paging-policy.py, allows you to play around with different
page-replacement policies. For example, let's examine how LRU performs with a
series of page references with a cache of size 3:

```sh
  0 1 2 0 1 3 0 3 1 2 1
```

To do so, run the simulator as follows:

```sh
prompt> ./paging-policy.py --addresses=0,1,2,0,1,3,0,3,1,2,1 
                           --policy=LRU --cachesize=3 -c

And what you would see is:

ARG addresses 0,1,2,0,1,3,0,3,1,2,1
ARG numaddrs 10
ARG policy LRU
ARG cachesize 3
ARG maxpage 10
ARG seed 0

Solving...

Access: 0 MISS LRU->      [br 0]<-MRU Replace:- [br Hits:0 Misses:1]
Access: 1 MISS LRU->   [br 0, 1]<-MRU Replace:- [br Hits:0 Misses:2]
Access: 2 MISS LRU->[br 0, 1, 2]<-MRU Replace:- [br Hits:0 Misses:3]
Access: 0 HIT  LRU->[br 1, 2, 0]<-MRU Replace:- [br Hits:1 Misses:3]
Access: 1 HIT  LRU->[br 2, 0, 1]<-MRU Replace:- [br Hits:2 Misses:3]
Access: 3 MISS LRU->[br 0, 1, 3]<-MRU Replace:2 [br Hits:2 Misses:4]
Access: 0 HIT  LRU->[br 1, 3, 0]<-MRU Replace:2 [br Hits:3 Misses:4]
Access: 3 HIT  LRU->[br 1, 0, 3]<-MRU Replace:2 [br Hits:4 Misses:4]
Access: 1 HIT  LRU->[br 0, 3, 1]<-MRU Replace:2 [br Hits:5 Misses:4]
Access: 2 MISS LRU->[br 3, 1, 2]<-MRU Replace:0 [br Hits:5 Misses:5]
Access: 1 HIT  LRU->[br 3, 2, 1]<-MRU Replace:0 [br Hits:6 Misses:5]
```
  
The complete set of possible arguments for paging-policy is listed on the
following page, and includes a number of options for varying the policy, how
addresses are specified/generated, and other important parameters such as the
size of the cache. 

```sh
prompt> ./paging-policy.py --help
Usage: paging-policy.py [options]

Options:
-h, --help      show this help message and exit
-a ADDRESSES, --addresses=ADDRESSES
                a set of comma-separated pages to access; 
                -1 means randomly generate
-f ADDRESSFILE, --addressfile=ADDRESSFILE
                a file with a bunch of addresses in it
-n NUMADDRS, --numaddrs=NUMADDRS
                if -a (--addresses) is -1, this is the 
                number of addrs to generate
-p POLICY, --policy=POLICY
                replacement policy: FIFO, LRU, LFU, OPT, 
                                    UNOPT, RAND, CLOCK
-b CLOCKBITS, --clockbits=CLOCKBITS
                for CLOCK policy, how many clock bits to use
-C CACHESIZE, --cachesize=CACHESIZE
                size of the page cache, in pages
-m MAXPAGE, --maxpage=MAXPAGE
                if randomly generating page accesses, 
                this is the max page number
-s SEED, --seed=SEED  random number seed
-N, --notrace   do not print out a detailed trace
-c, --compute   compute answers for me
```
  
As usual, "-c" is used to solve a particular problem, whereas without it, the
accesses are just listed (and the program does not tell you whether or not a
particular access is a hit or miss).

To generate a random problem, instead of using "-a/--addresses" to pass in
some page references, you can instead pass in "-n/--numaddrs" as the number of
addresses the program should randomly generate, with "-s/--seed" used to
specify a different random seed. For example:

```sh
prompt> ./paging-policy.py -s 10 -n 3
.. .
Assuming a replacement policy of FIFO, and a cache of size 3 pages,
figure out whether each of the following page references hit or miss
in the page cache.
  
Access: 5  Hit/Miss?  State of Memory?
Access: 4  Hit/Miss?  State of Memory?
Access: 5  Hit/Miss?  State of Memory?
```
  
As you can see, in this example, we specify "-n 3" which means the program
should generate 3 random page references, which it does: 5, 7, and 5. The
random seed is also specified (10), which is what gets us those particular
numbers. After working this out yourself, have the program solve the problem
for you by passing in the same arguments but with "-c" (showing just the
relevant part here):

```sh
prompt> ./paging-policy.py -s 10 -n 3 -c
...
Solving...

Access: 5 MISS FirstIn->   [br 5] <-Lastin Replace:- [br Hits:0 Misses:1]
Access: 4 MISS FirstIn->[br 5, 4] <-Lastin Replace:- [br Hits:0 Misses:2]
Access: 5 HIT  FirstIn->[br 5, 4] <-Lastin Replace:- [br Hits:1 Misses:2]
```

The default policy is FIFO, though others are available, including LRU, MRU,
OPT (the optimal replacement policy, which peeks into the future to see what
is best to replace), UNOPT (which is the pessimal replacement), RAND (which
does random replacement), and CLOCK (which does the clock algorithm). The
CLOCK algorithm also takes another argument (-b), which states how many bits
should be kept per page; the more clock bits there are, the better the
algorithm should be at determining which pages to keep in memory.

Other options include: "-C/--cachesize" which changes the size of the page
cache; "-m/--maxpage" which is the largest page number that will be used if
the simulator is generating references for you; and "-f/--addressfile" which
lets you specify a file with addresses in them, in case you wish to get traces
from a real application or otherwise use a long trace as input.

One last piece of fun: why are these two examples interesting?

```sh
./paging-policy.py -C 3 -a 1,2,3,4,1,2,5,1,2,3,4,5
```
and
```sh
./paging-policy.py -C 4 -a 1,2,3,4,1,2,5,1,2,3,4,5
```

# Questions


1. Generate random addresses with the following arguments: -s 0 -n 10, -s 1 -n 10, and -s 2 -n 10. Change the policy from FIFO, to LRU, to OPT. Compute whether each access in said address traces are hits or misses.
    
    cache size: 3

    - FIFO

    access |  hit or miss  | replaced page | cache    | hit/miss count
    -------|---------------|---------------|----------|----------------
    8      |  miss         |  -            | [8]      | 0/1
    7      |  miss         |  -            | [7, 8]   | 0/2  
    4      |  miss         |  -            | [4, 7, 8]| 0/3  
    2      |  miss         |  8            | [2, 4, 7]| 0/4   
    5      |  miss         |  7            | [5, 2, 4]| 0/5   
    4      |  hit          |  -            | [5, 2, 4]| 1/5   
    7      |  miss         |  4            | [7, 5, 2]| 1/6   
    3      |  miss         |  2            | [3, 7, 5]| 1/7   
    4      |  miss         |  5            | [4, 3, 7]| 1/8   
    5      |  miss         |  7            | [5, 4, 3]| 1/9

    - LRU

    access |  hit or miss  | replaced page | cache    | hit/miss count
    -------|---------------|---------------|----------|----------------
    8      |  miss         |  -            | [8]      | 0/1
    7      |  miss         |  -            | [7, 8]   | 0/2  
    4      |  miss         |  -            | [4, 7, 8]| 0/3  
    2      |  miss         |  8            | [2, 4, 7]| 0/4   
    5      |  miss         |  7            | [5, 2, 4]| 0/5   
    4      |  hit          |  -            | [4, 5, 2]| 1/5   
    7      |  miss         |  2            | [7, 4, 5]| 1/6   
    3      |  miss         |  5            | [3, 7, 4]| 1/7   
    4      |  hit          |  -            | [4, 3, 7]| 2/7   
    5      |  miss         |  7            | [5, 4, 3]| 2/8   

    - OPT

    access |  hit or miss  | replaced page | cache    | hit/miss count
    -------|---------------|---------------|----------|----------------
    8      |  miss         |  -            | [8]      | 0/1
    7      |  miss         |  -            | [7, 8]   | 0/2  
    4      |  miss         |  -            | [4, 7, 8]| 0/3  
    2      |  miss         |  8            | [2, 4, 7]| 0/4   
    5      |  miss         |  2            | [5, 4, 7]| 0/5   
    4      |  hit          |  -            | [5, 4, 7]| 1/5   
    7      |  hit          |  -            | [5, 4, 7]| 2/5   
    3      |  miss         |  7            | [5, 4, 3]| 2/6   
    4      |  hit          |  -            | [5, 4, 3]| 3/6   
    5      |  hit          |  -            | [5, 4, 3]| 4/6  

2. For a cache of size 5, generate worst-case address reference streams for each of the following policies: FIFO, LRU, and MRU (worst-case reference streams cause the most misses possible. For the worst case reference streams, how much bigger of a cache is needed to improve performance dramatically and approach OPT?

    - FIFO, LRU: ```paging-policy.py -C 5 -p FFIO/LRU -a 1,2,3,4,5,6,1,2,3,4,5,6``` 

        Worst case when looping over the data larger than cache. To improve performance, cache size should be the same with the number of pages in a loop. 
    - MRU:  ```paging-policy.py -C 5 -p MRU -a 1,2,3,4,5,6,5,6,5,6,5```

        Worst case when evicted recent page is accessed right after replaced. To improve performance, one more cache is needed for above case.
        
3. Generate a random trace (use python or perl). How would you expect the different policies to perform on such a trace?
    
    run ```./generate-trace.py POLICIES_SEPERATED_BY_COMMA NUMBER_OF_ACCESS```

4. Now generate a trace with some locality. How can you generate such a trace? How does LRU perform on it? How much better than RAND is LRU? How does CLOCK do? How about CLOCK with different numbers of clock bits?

   ```
   python3 ./generate-trace.py OPT,LRU,CLOCK,RAND 1000 0.8

    try 1:
    OPT
    hits 796   misses 204   hitrate 79.60
    LRU
    hits 676   misses 324   hitrate 67.60
    CLOCK
    hits 663   misses 337   hitrate 66.30
    RAND
    hits 626   misses 374   hitrate 62.60

    try 2:
    OPT
    hits 778   misses 222   hitrate 77.80
    LRU
    hits 648   misses 352   hitrate 64.80
    CLOCK
    hits 635   misses 365   hitrate 63.50
    RAND
    hits 602   misses 398   hitrate 60.20    

    try 3:
    OPT
    hits 781   misses 219   hitrate 78.10
    LRU
    hits 668   misses 332   hitrate 66.80
    CLOCK
    hits 628   misses 372   hitrate 62.80
    RAND
    hits 638   misses 362   hitrate 63.80

   ```
   LRU shows about 6~7% better performance than random on high locality tasks. Clock is in the middle, and as we increase the numbers of clock bits it's performance get closer to the LRU.


5. Use a program like valgrind to instrument a real application and generate a virtual page reference stream. For example, running valgrind --tool=lackey --trace-mem=yes ls will output a nearly-complete reference trace of every instruction and data reference made by the program ls. To make this useful for the simulator above, you’ll have to first transform each virtual memory reference into a virtual page-number reference (done by masking off the offset and shifting the resulting bits downward). How big of a cache is needed for your application trace in order to satisfy a large fraction of requests? Plot a graph of its working set as the size of the cache increases.