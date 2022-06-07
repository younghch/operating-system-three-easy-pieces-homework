
# Overview

This program, malloc.py, allows you to see how a simple memory allocator
works. Here are the options that you have at your disposal:

```sh
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -S HEAPSIZE, --size=HEAPSIZE
                        size of the heap
  -b BASEADDR, --baseAddr=BASEADDR
                        base address of heap
  -H HEADERSIZE, --headerSize=HEADERSIZE
                        size of the header
  -a ALIGNMENT, --alignment=ALIGNMENT
                        align allocated units to size; -1->no align
  -p POLICY, --policy=POLICY
                        list search (BEST, WORST, FIRST)
  -l ORDER, --listOrder=ORDER
                        list order (ADDRSORT, SIZESORT+, SIZESORT-, INSERT-FRONT, INSERT-BACK)
  -C, --coalesce        coalesce the free list?
  -n OPSNUM, --numOps=OPSNUM
                        number of random ops to generate
  -r OPSRANGE, --range=OPSRANGE
                        max alloc size
  -P OPSPALLOC, --percentAlloc=OPSPALLOC
                        percent of ops that are allocs
  -A OPSLIST, --allocList=OPSLIST
                        instead of random, list of ops (+10,-0,etc)
  -c, --compute         compute answers for me
```

One way to use it is to have the program generate some random allocation/free
operations and for you to see if you can figure out what the free list would
look like, as well as the success or failure of each operation. 

Here is a simple example:

```sh
prompt> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT -p BEST -n 5 

ptr[0] = Alloc(3)  returned ?
List?

Free(ptr[0]) returned ?
List?

ptr[1] = Alloc(5)  returned ?
List?

Free(ptr[1]) returned ?
List?

ptr[2] = Alloc(8)  returned ?
List?
```

In this example, we specify a heap of size 100 bytes (-S 100), starting at
address 1000 (-b 1000). We specify an additional 4 bytes of header per
allocated block (-H 4), and make sure each allocated space rounds up to the
nearest 4-byte free chunk in size (-a 4). We specify that the free list be
kept ordered by address (increasing). Finally, we specify a "best fit"
free-list searching policy (-p BEST), and ask for 5 random operations to be
generated (-n 5). The results of running this are above; your job is to figure
out what each allocation/free operation returns, as well as the state of the
free list after each operation.

Here we look at the results by using the -c option.

```sh
prompt> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT -p BEST -n 5 -c

ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:92 ]

ptr[1] = Alloc(5)  returned 1012 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1020 sz:80 ]

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:12 ] [ addr:1020 sz:80 ]

ptr[2] = Alloc(8)  returned 1012 (searched 3 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1020 sz:80 ]

As you can see, the first allocation operation (an allocation) returns the
following information:

ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]
```

Because the initial state of the free list is just one large element, it is
easy to guess that the Alloc(3) request will succeed. Further, it will just
return the first chunk of memory and make the remainder into a free list. The
pointer returned will be just beyond the header (address:1004), and the
allocated space is rounded up to 4 bytes, leaving the free list with 92 bytes
starting at 1008. 

The next operation is a Free, of "ptr[0]" which is what stores the results of
the previous allocation request. As you can expect, this free will succeed
(thus returning "0"), and the free list now looks a little more complicated:

```sh
Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:92 ]
```

Indeed, because we are NOT coalescing the free list, we now have two elements
on it, the first being 8 bytes large and holding the just-returned space, and
the second being the 92-byte chunk. 

We can indeed turn on coalescing via the -C flag, and the result is:

```sh
prompt> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT -p BEST -n 5 -c -C
ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]

Free(ptr[0]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ]

ptr[1] = Alloc(5)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1012 sz:88 ]

Free(ptr[1]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ]

ptr[2] = Alloc(8)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1012 sz:88 ]
```

You can see that when the Free operations take place, the free list is
coalesced as expected.

There are some other interesting options to explore:

* `-p BEST` or `-p WORST` or `-p FIRST`: This option lets you use these three different strategies to look for a chunk of memory to use during an allocation request 
* `-l ADDRSORT` or `-l SIZESORT+` or `-l SIZESORT-` or `-l INSERT-FRONT` or `-l INSERT-BACK`: This option lets you keep the free list in a particular order, say sorted by address of the free chunk, size of free chunk (either increasing with a + or decreasing with a -), or simply returning free chunks to the front (INSERT-FRONT) or back (INSERT-BACK) of the free list.
* `-A list_of_ops`: This option lets you specify an exact series of requests instead of randomly-generated ones. For example, running with the flag "-A +10,+10,+10,-0,-2" will allocate three chunks of size 10 bytes (plus header), and then free the first one ("-0") and then free the third one ("-2"). What will the free list look like then?

Those are the basics. Use the questions from the book chapter to explore more,
or create new and interesting questions yourself to better understand how
allocators function.

# Questions

1. First run with the flags ```-n 10 -H 0 -p BEST -s 0``` to generate a few random allocations and frees. Can you predict what alloc()/free() will return? Can you guess the state of the free list after each request? What do you notice about the free list over time?

    ```
    size 100
    baseAddr 1000
    headerSize 0
    alignment -1
    policy BEST => number of searched elment == length of free list before starts searching
    listOrder ADDRSORT
    coalesce False
    numOps 10
    range 10
    percentAlloc 50
    allocList 
    compute False

    ptr[0] = Alloc(3) returned 1000
    Free List [Size 1]: [address: 1003, size: 97]

    Free(ptr[0])
    returned 0
    Free List [Size 2]: [address: 1000, size: 3], [address: 1003, size: 97]

    ptr[1] = Alloc(5) returned 1003
    Free List [Size 2]: [address: 1000, size: 3], [address: 1008, size: 92]

    Free(ptr[1])
    returned 0
    Free List [Size 3]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 92]

    ptr[2] = Alloc(8) returned 1008
    Free List [Size 3]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1016, size: 84]

    Free(ptr[2])
    returned 0
    Free List [Size 4]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 84]

    ptr[3] = Alloc(8) returned 1008
    Free List [Size 3]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1016, size: 84]

    Free(ptr[3])
    returned 0
    Free List [Size 4]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 84]

    ptr[4] = Alloc(2) returned 1000
    Free List [Size 4]: [address: 1002, size: 1], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 84]

    ptr[5] = Alloc(7) returned 1008
    Free List [Size 4]: [address: 1002, size: 1], [address: 1003, size: 5], [address: 1015, size: 1], [address: 1016, size: 84]
    ```

    Best fit makes small size of free spaces in the free list, which are hard to allocate new items.

2. How are the results different when using a WORST fit policy to search the free list (-p WORST)? What changes?

    ```
    ptr[0] = Alloc(3) returned 1000
    Free List [Size 1]: [address: 1003, size: 97]

    Free(ptr[0])
    returned 0
    Free List [Size 2]: [address: 1000, size: 3], [address: 1003, size: 97]

    ptr[1] = Alloc(5) returned 1003
    Free List [Size 2]: [address: 1000, size: 3], [address: 1008, size: 92]

    Free(ptr[1])
    returned 0
    Free List [Size 3]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 92]

    ptr[2] = Alloc(8) returned 1008
    Free List [Size 3]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1016, size: 84]

    Free(ptr[2])
    returned 0
    Free List [Size 4]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 84]

    ptr[3] = Alloc(8) returned 1016
    Free List [Size 4]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1024, size: 76]

    Free(ptr[3])
    returned 0
    Free List [Size 5]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 8], [address: 1024, size: 76]

    ptr[4] = Alloc(2) returned 1024
    Free List [Size 5]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 8], [address: 1026, size: 74]

    ptr[5] = Alloc(7) returned 1026
    Free List [Size 5]: [address: 1000, size: 3], [address: 1003, size: 5], [address: 1008, size: 8], [address: 1016, size: 8], [address: 1033, size: 67]
    ```
    New items are allocated to largest free space, which is always the last element in the free list.

3. What about when using FIRST fit (-p FIRST)? What speeds up when you use first fit?

    ```
    ptr[0] = Alloc(3) returned 1000 (searched 1 elements)
    Free List [ Size 1 ]: [ addr:1003 sz:97 ]

    Free(ptr[0])
    returned 0
    Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1003 sz:97 ]

    ptr[1] = Alloc(5) returned 1003 (searched 2 elements)
    Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1008 sz:92 ]

    Free(ptr[1])
    returned 0
    Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:92 ]

    ptr[2] = Alloc(8) returned 1008 (searched 3 elements)
    Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

    Free(ptr[2])
    returned 0
    Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

    ptr[3] = Alloc(8) returned 1008 (searched 3 elements)
    Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

    Free(ptr[3])
    returned 0
    Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

    ptr[4] = Alloc(2) returned 1000 (searched 1 elements)
    Free List [ Size 4 ]: [ addr:1002 sz:1 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

    ptr[5] = Alloc(7) returned 1008 (searched 3 elements)
    Free List [ Size 4 ]: [ addr:1002 sz:1 ][ addr:1003 sz:5 ][ addr:1015 sz:1 ][ addr:1016 sz:84 ]
    ```
    When using first fit there is no need to search all the elements in the free list.

4. For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings (```-l ADDRSORT, -l SIZESORT+, -l SIZESORT-```) to see how the policies and the list orderings interact.

    For best fit using ```-l SIZESORT-```(descending order) decreased the time to search, ```-l SIZESORT+``` increase the time to search.

    Best fit and worst fit do not get affected because they compare all the elements in the list.

5. Coalescing of a free list can be quite important. Increase the number of random allocations (say to -n 1000). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the -C flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?


    Without coalescing:

    | Policy    | size of a free space <br/>(after 1000 random allocations)  | note  |
    ------------|-----------------------|-------|
    | Best Fit  | 31                    |       |
    | Worst Fit | 100                   | free lists are full of free space size 1 node |
    | First Fit (ADDRSORT)  | 51        |       |
    | First Fit (sizesort +)| 31        | Behaves exactly the same as best fit      |
    | First Fit (sizesort -)| 100       | Behaves exactly the same as worst fit      |


    With coalescing:
    | Policy    | size of a free space <br/>(after 1000 random allocations)  
    ------------|-----------------------
    | Best Fit  (ADDRSORT)  | 1        
    | Worst Fit (ADDRSORT)  | 1        
    | First Fit (ADDRSORT)  | 1        
    | First Fit (sizesort +)<br/>Best Fit (sizesort +)| 28       
    | First Fit (sizesort -)<br/>Worst Fit (sizesort -)| 98

    When coalescing, free list order matters for best and worst fit. When free node's free space size is the same, ADDRSORT free list allocate memory to the smaller address one. This makes memory allocated concentrated to lower memory address and leave large free space behind, avoiding external fragmentation.

    Without coalescing, external fragmentation occurs and frequently fails to allocate memory.

6. What happens when you change the percent allocated fraction -P to higher than 50? What happens to allocations as it nears 100? What about as the percent nears 0?

    - P higher than 50 : fails to allocate memory frequently
    - P == 100         : Free list empty at some point, no more allocation possible
    - P == 0           : No memory allocated
    
7. What kind of specific requests can you make to generate a highly fragmented free space? Use the -A flag to create fragmented free lists, and see how different policies and options change the organization of the free list.
    ```-S 20 -A +1,+2,+3,+4,+1,+2,+3,+4,-1,-3,-5,-7```
