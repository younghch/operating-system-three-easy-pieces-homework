
# Overview

In this homework, you will use a simple program, which is known as
paging-linear-translate.py, to see if you understand how simple
virtual-to-physical address translation works with linear page tables. To run
the program, remember to either type just the name of the program
(./paging-linear-translate.py) or possibly this (python
paging-linear-translate.py). When you run it with the -h (help) flag, you 
see:

```sh
prompt> ./paging-linear-translate.py -h
Usage: paging-linear-translate.py [options]

Options:
-h, --help              show this help message and exit
-s SEED, --seed=SEED    the random seed
-a ASIZE, --asize=ASIZE 
                        address space size (e.g., 16, 64k, ...)
-p PSIZE, --physmem=PSIZE
                        physical memory size (e.g., 16, 64k, ...)
-P PAGESIZE, --pagesize=PAGESIZE
                        page size (e.g., 4k, 8k, ...)
-n NUM, --addresses=NUM number of virtual addresses to generate
-u USED, --used=USED    percent of address space that is used
-v                      verbose mode
-c                      compute answers for me
```

First, run the program without any arguments:

```sh
prompt> ./paging-linear-translate.py 
ARG seed 0
ARG address space size 16k
ARG phys mem size 64k
ARG page size 4k
ARG verbose False

The format of the page table is simple:
The high-order (left-most) bit is the VALID bit.
  If the bit is 1, the rest of the entry is the PFN.
  If the bit is 0, the page is not valid.
Use verbose mode (-v) if you want to print the VPN # by
each entry of the page table.

Page Table (from entry 0 down to the max size)
   0x8000000c
   0x00000000
   0x00000000
   0x80000006

Virtual Address Trace
  VA  0: 0x00003229 (decimal:    12841) --> PA or invalid?
  VA  1: 0x00001369 (decimal:     4969) --> PA or invalid?
  VA  2: 0x00001e80 (decimal:     7808) --> PA or invalid?
  VA  3: 0x00002556 (decimal:     9558) --> PA or invalid?
  VA  4: 0x00003a1e (decimal:    14878) --> PA or invalid?
```

For each virtual address, write down the physical address it 
translates to OR write down that it is an out-of-bounds 
address (e.g., a segmentation fault).

As you can see, what the program provides for you is a page table for a
particular process (remember, in a real system with linear page tables, there
is one page table per process; here we just focus on one process, its address
space, and thus a single page table). The page table tells you, for each
virtual page number (VPN) of the address space, that the virtual page is
mapped to a particular physical frame number (PFN) and thus valid, or not
valid.

The format of the page-table entry is simple: the left-most (high-order) bit
is the valid bit; the remaining bits, if valid is 1, is the PFN. 

In the example above, the page table maps VPN 0 to PFN 0xc (decimal 12), VPN 3
to PFN 0x6 (decimal 6), and leaves the other two virtual pages, 1 and 2, as
not valid. 

Because the page table is a linear array, what is printed above is a replica
of what you would see in memory if you looked at the bits yourself. However,
it is sometimes easier to use this simulator if you run with the verbose flag
(-v); this flag also prints out the VPN (index) into the page table. From the
example above, run with the -v flag:

```sh
Page Table (from entry 0 down to the max size)
  [       0]   0x8000000c
  [       1]   0x00000000
  [       2]   0x00000000
  [       3]   0x80000006
```

Your job, then, is to use this page table to translate the virtual addresses
given to you in the trace to physical addresses. Let's look at the first one:
VA 0x3229. To translate this virtual address into a physical address, we first
have to break it up into its constituent components: a virtual page number and
an offset. We do this by noting down the size of the address space and the
page size. In this example, the address space is set to 16KB (a very small
address space) and the page size is 4KB. Thus, we know that there are 14 bits
in the virtual address, and that the offset is 12 bits, leaving 2 bits for the
VPN. Thus, with our address 0x3229, which is binary 11 0010 0010 1001, we know
the top two bits specify the VPN. Thus, 0x3229 is on virtual page 3 with an
offset of 0x229.

We next look in the page table to see if VPN 3 is valid and mapped to some
physical frame or invalid, and we see that it is indeed valid (the high bit is
1) and mapped to physical page 6. Thus, we can form our final physical address
by taking the physical page 6 and adding it onto the offset, as follows:
0x6000 (the physical page, shifted into the proper spot) OR 0x0229 (the
offset), yielding the final physical address: 0x6229. Thus, we can see that
virtual address 0x3229 translates to physical address 0x6229 in this example.

To see the rest of the solutions (after you have computed them yourself!),
just run with the -c flag (as always):

```sh
...
VA  0: 00003229 (decimal: 12841) --> 00006229 (25129) [VPN 3]
VA  1: 00001369 (decimal:  4969) --> Invalid (VPN 1 not valid)
VA  2: 00001e80 (decimal:  7808) --> Invalid (VPN 1 not valid)
VA  3: 00002556 (decimal:  9558) --> Invalid (VPN 2 not valid)
VA  4: 00003a1e (decimal: 14878) --> 00006a1e (27166) [VPN 3]
```

Of course, you can change many of these parameters to make more interesting
problems. Run the program with the -h flag to see what options there are:

* The -s flag changes the random seed and thus generates different page table values as well as different virtual addresses to translate.
* The -a flag changes the size of the address space.
* The -p flag changes the size of physical memory.
* The -P flag changes the size of a page.
* The -n flag can be used to generate more addresses to translate (instead of the default 5).
* The -u flag changes the fraction of mappings that are valid, from 0% (-u 0) up to 100% (-u 100). The default is 50, which means that roughly 1/2 of the pages in the virtual address space will be valid.
* The -v flag prints out the VPN numbers to make your life easier.

# Questions

1. Before doing any translations, let’s use the simulator to study how linear page tables change size given different parameters. Compute the size of linear page tables as different parameters change. Some suggested inputs are below; by using the -v flag, you can see how many page-table entries are filled. First, to understand how linear page table size changes as the address space grows, run with these flags:
    ```
    -P 1k -a 1m -p 512m -v -n 0
    -P 1k -a 2m -p 512m -v -n 0
    -P 1k -a 4m -p 512m -v -n 0
    ```
    Then, to understand how linear page table size changes as page size grows:
    ```
    -P 1k -a 1m -p 512m -v -n 0
    -P 2k -a 1m -p 512m -v -n 0
    -P 4k -a 1m -p 512m -v -n 0
    ```
    Before running any of these, try to think about the expected trends. How should page-table size change as the address space grows? As the page size grows? Why not use big pages in general?
  
    Page table size increase as address space size grows and decrease as page size grows because ```page-table size = (address space size) / (page size)```. Using big pages can decrease page table size, but It makes page frame large. Large page frames makes internal fragmentation and makes it hard to share memory between many processes.


2. Now let’s do some translations. Start with some small examples, and change the number of pages that are allocated to the address space with the -u flag. For example:
    ```
    -P 1k -a 16k -p 32k -v -u 0
    -P 1k -a 16k -p 32k -v -u 25
    -P 1k -a 16k -p 32k -v -u 50
    -P 1k -a 16k -p 32k -v -u 75
    -P 1k -a 16k -p 32k -v -u 100
    ```
    What happens as you increase the percentage of pages that are allocated in each address space?

    The number of pages valid increase accordingly.

3. Now let’s try some different random seeds, and some different(and sometimes quite crazy) address-space parameters, for variety:
    ```
    -P 8 -a 32 -p 1024 -v -s 1 
    -P 8k -a 32k -p 1m -v -s 2
    -P 1m -a 256m -p 512m -v -s 3
    ```
    Which of these parameter combinations are unrealistic? Why?

    First one. Size too small.

4. Use the program to try out some other problems. Can you find the limits of where the program doesn’t work anymore? For example, what happens if the address-space size is bigger than physical memory?

    The simulator returns error if the address-space size is bigger than physical memory.
    ```
    Error: physical memory size must be GREATER than address space size (for this simulation)
    ```

    Other failing conditions:
    - page size is larger than address space size.
    - physical memory size is not multiple of page size.
    - address space is not multiple of page size.
    