# Overview
 In this homework, you will gain some familiarity with memory allocation. First, you’ll write some buggy programs (fun!). Then, you’ll use some tools to help you find the bugs you inserted. Then, you will realize how awesome these tools are and use them in the future, thus making yourself more happy and productive. The tools are the debugger (e.g., gdb) and a memory-bug detector called valgrind [SN05].

# Questions
1. First, write a simple program called null.c that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Compile this into an executable called null. What happens when you run this program?
   
    Nothing happens. According to man 3 free, ```The free() function deallocates the memory allocation pointed to by ptr. If ptr is a NULL pointer, no operation is performed.```

2. Next,compile this program with symbol information included(with the -g flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing gdb null and then, once gdb is running, typing run. What does gdb show you?
    
    Process exited normally.
    
3. Finally, use the valgrind tool on this program. We’ll use the memcheck tool that is a part of valgrind to analyze what happens. Run this by typing in the following: valgrind --leak-check=yes null. What happens when you run this? Can you interpret the output from the tool?
    
    No memory leaks exists.
    output:
    ```
    ==9715== Command: ./test
    ==9715== 
    ==9715== 
    ==9715== HEAP SUMMARY:
    ==9715==     in use at exit: 0 bytes in 0 blocks
    ==9715==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
    ==9715== 
    ==9715== All heap blocks were freed -- no leaks are possible
    ==9715== 
    ==9715== For lists of detected and suppressed errors, rerun with: -s
    ==9715== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ```
    

4. Write a simple program that allocates memory using malloc() but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the --leak-check=yes flag)?

    The allocated memory is freed by OS when terminating process, not by a process control. Gdb can't detect problem, but valgrind can.
    ```
    ==9336== Command: ./q4_test
    ==9336== 
    ==9336== 
    ==9336== HEAP SUMMARY:
    ==9336==     in use at exit: 4 bytes in 1 blocks
    ==9336==   total heap usage: 1 allocs, 0 frees, 4 bytes allocated
    ==9336== 
    ==9336== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
    ==9336==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==9336==    by 0x109165: main (q4.c:7)
    ==9336== 
    ==9336== LEAK SUMMARY:
    ==9336==    definitely lost: 4 bytes in 1 blocks
    ==9336==    indirectly lost: 0 bytes in 0 blocks
    ==9336==      possibly lost: 0 bytes in 0 blocks
    ==9336==    still reachable: 0 bytes in 0 blocks
    ==9336==         suppressed: 0 bytes in 0 blocks
    ==9336== 
    ==9336== For lists of detected and suppressed errors, rerun with: -s
    ==9336== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
    ```
5. Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?

    Nothing happens when run this program, but valgrind detects error. data is pointer to integer so moves by 4 bytes. Correct expression would be ```data[25] = 0```.

    ```
    ==9530== Command: ./q5_test
    ==9530== 
    ==9530== Invalid write of size 4
    ==9530==    at 0x109194: main (q5.c:8)
    ==9530==  Address 0x4a501d0 is 224 bytes inside an unallocated block of size 4,194,032 in arena "client"
    ==9530== 
    ==9530== 
    ==9530== HEAP SUMMARY:
    ==9530==     in use at exit: 0 bytes in 0 blocks
    ==9530==   total heap usage: 1 allocs, 1 frees, 100 bytes allocated
    ==9530== 
    ==9530== All heap blocks were freed -- no leaks are possible
    ==9530== 
    ==9530== For lists of detected and suppressed errors, rerun with: -s
    ==9530== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
    ```

6. Create a program that allocates an array of integers(as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?

    Yes, program runs normally. But valgrinds detects invalid read error.
    ```
    ==9839== Command: ./q6.test
    ==9839== 
    ==9839== Invalid read of size 4
    ==9839==    at 0x1091CC: main (q6.c:11)
    ==9839==  Address 0x4a50068 is 40 bytes inside a block of size 100 free'd
    ==9839==    at 0x483CA3F: free (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==9839==    by 0x1091C3: main (q6.c:10)
    ==9839==  Block was alloc'd at
    ==9839==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==9839==    by 0x1091A5: main (q6.c:8)
    ==9839== 
    value inside the pointer : 42
    ==9839== 
    ==9839== HEAP SUMMARY:
    ==9839==     in use at exit: 0 bytes in 0 blocks
    ==9839==   total heap usage: 2 allocs, 2 frees, 1,124 bytes allocated
    ==9839== 
    ==9839== All heap blocks were freed -- no leaks are possible
    ==9839== 
    ==9839== For lists of detected and suppressed errors, rerun with: -s
    ==9839== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
    ```

7. Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?
    
    Error ocuurs. Do not need additional tools.
    ```
    free(): invalid pointer
    Aborted (core dumped)
    ```
8. Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use realloc() to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs.


9. Spend more time and read about using gdb and valgrind. Knowing your tools is critical; spend the time and learn how to become an expert debugger in the UNIX and C environment.
