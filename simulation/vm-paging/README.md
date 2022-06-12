
# Overview

In this homework, you will use a simple program, which is known as
paging-linear-translate.py, to see if you understand how simple virtual-to-physical address translation works with linear page tables. [detail](https://github.com/remzi-arpacidusseau/ostep-homework/tree/master/vm-paging)

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
