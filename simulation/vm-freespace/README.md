
# Overview

This program, malloc.py, allows you to see how a simple memory allocator works. [detail](https://github.com/remzi-arpacidusseau/ostep-homework/tree/master/vm-freespace)

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
