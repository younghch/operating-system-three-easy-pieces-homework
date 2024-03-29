
# Overview

This homework lets you play around with a number of ways to implement
a small, deadlock-free vector object in C. The vector object is quite
limited (e.g., it only has `add()` and `init()` functions) but is just
used to illustrate different approaches to avoiding deadlock.

Some files that you should pay attention to are as follows. They, in
particular, are used by all the variants in this homework.
- `common_threads.h`: The usual wrappers around many different pthread (and other) library calls, so as to ensure they are not failing silently
- `vector-header.h`: A simple header for the vector routines, mostly defining a fixed vector size and then a struct that is used per vector (vector_t)
- `main-header.h`: A number of global variables common to each different program
- `main-common.c`: Contains the main() routine (with arg parsing) that initializes two vectors, starts some threads to access them (via a worker() routine), and then waits for the many vector_add()'s to complete

The variants of this homework are found in the following files. Each takes a
different approach to dealing with concurrency inside a "vector addition"
routine called `vector_add()`; examine the code in these files to get a sense of
what is going on. They all use the files above to make a complete runnable
program. 

The relevant files:
- `vector-deadlock.c`: This version blithely grabs the locks in a particular order (dst then src). By doing so, it creates an "invitation to deadlock", as one thread might call `vector_add(v1, v2)` while another concurrently calls `vector_add(v2, v1)`.
- `vector-global-order.c`: This version of `vector_add()` grabs the locks in a total order, based on address of the vector. 
- `vector-try-wait.c`: This version of `vector_add()` uses `pthread_mutex_trylock()` to attempt to grab locks; when the try fails, the code releases any locks it may hold and goes back to the top and tries it all over again.
- `vector-avoid-hold-and-wait.c`: This version ensures it can't get stuck in a hold and wait pattern by using a single lock around lock acquisition.
- `vector-nolock.c`: This version doesn't even use locks; rather, it uses an atomic fetch-and-add to implement the `vector_add()` routine. Its semantics (as a result) are slightly different.

Type `make` (and read the `Makefile`) to build each of five executables. 

```sh
prompt> make
```

Then you can run a program by simply typing its name:

```sh
prompt> ./vector-deadlock
```

Each program takes the same set of arguments (see main-common.c for details):
- `-d`: This flag turns on the ability for threads to deadlock. When you pass `-d` to the program, every other thread calls `vector_add()` with the vectors in a different order, e.g., with two threads, and `-d` enabled, Thread 0 calls `vector_add(v1, v2)` and Thread 1 calls `vector_add(v2, v1)`
- `-p`: This flag gives each thread a different set of vectors to call add upon, instead of just two vectors. Use this to see how things perform when there isn't contention for the same set of vectors.
- `-n num_threads`: Creates some number of threads; you need more than one to deadlock.
- `-l loops`: How many times should each thread call add?
- `-v`: Verbose flag: prints out a little more about what is going on.
- `-t`: Turns on timing and shows how long everything took.


# Questions

1. First let’s make sure you understand how the programs generally work, and some of the key options. Study the code in ```vector-deadlock.c```, as well as in ```main-common.c``` and related files. Now, run ```./vector-deadlock -n 2 -l 1 -v```, which instantiates two threads (```-n 2```), each of which does one vector add (```-l 1```), and does so in verbose mode (```-v```). Make sure you understand the output. How does the output change from run to run?

    Order of thread 1 and 2 could be changed.

2. Now add the ```-d``` flag, and change the number of loops(```-l```) from 1 to higher numbers. What happens? Does the code (always) deadlock?

    ```./vector-deadlock -n 2 -l 100000 -v -d```. Deadlock occurs but not always.

3. How does changing the number of threads (```-n```) change the outcome of the program? Are there any values of ```-n``` that ensure no deadlock occurs?

    No dead lock occurs when there is only one thread.

4. Now examine the code in ```vector-global-order.c```. First, make sure you understand what the code is trying to do; do you understand why the code avoids deadlock? Also, why is there a special case in this  ```vector_add()``` routine when the source and destination vectors are the same?

    By locking the vector with lower pointer address first, circular wait can not happen. So there is no deadlock. When two vectors are same, locking twice would make a thread wait for itself.

5. Now run the code with the following flags: ```-t -n 2 -l 100000 -d```. How long does the code take to complete? How does the total time change when you increase the number of loops, or the number of threads?

    Time increases linearly with the number of loop.
    ```
    ./vector-global-order -t -n 2 -l 100000 -d
    Time: 0.04 seconds
    ./vector-global-order -t -n 2 -l 200000 -d
    Time: 0.08 seconds
    ./vector-global-order -t -n 2 -l 300000 -d
    Time: 0.12 seconds
    ./vector-global-order -t -n 2 -l 400000 -d
    Time: 0.15 seconds
    ```

    ```
    ./vector-global-order -t -n 2 -l 100000 -d
    Time: 0.04 seconds
    ./vector-global-order -t -n 4 -l 50000 -d 
    Time: 0.11 seconds
    ./vector-global-order -t -n 8 -l 25000 -d
    Time: 0.24 seconds
    ./vector-global-order -t -n 16 -l 12500 -d
    Time: 0.41 seconds
    ```

6. What happens if you turn on the parallelism flag (-p)? How much would you expect performance to change when each thread is working on adding different vectors (which is what -p enables) versus working on the same ones?

    If all thread is working on different vectors, there is no need to wait for other thread to free the lock. Performance increase as the number of threads increase. (for large number of tasks.)

    ```
    ./vector-global-order -t -n 2 -l  10000000 -d -p
    Time: 1.46 seconds
    ./vector-global-order -t -n 4 -l  5000000 -d -p 
    Time: 1.42 seconds
    ./vector-global-order -t -n 8 -l  2500000 -d -p
    Time: 0.88 seconds
    ./vector-global-order -t -n 16 -l 1250000 -d -p
    Time: 0.52 seconds
    ```
7. Now let’s study ```vector-try-wait.c```. First make sure you understand the code. Is the first call to ```pthread_mutex_trylock()``` really needed? Now run the code. How fast does it run compared to the global order approach? How does the number of retries, as counted by the code, change as the number of threads increases?

    The first call to ```pthread_mutex_trylock()``` can be simply changed to ```pthread_mutex_lock()```. Because when it fails, it would sleep and yield control to the other thread.

    It repeatedly lock and unlock, the performance is lower than global order approach. As number of thread increases the number of retries increases accordingly.
    ```
    ./vector-try-wait  -t -n 2 -l 100000 -d
    Retries: 1276526
    Time: 0.16 seconds
    
    ./vector-try-wait  -t -n 2 -l 200000 -d
    Retries: 3948901
    Time: 0.43 seconds
    
    ./vector-try-wait  -t -n 2 -l 300000 -d
    Retries: 1510390
    Time: 0.24 seconds
    
    ./vector-try-wait  -t -n 2 -l 400000 -d
    Retries: 4880362
    Time: 0.59 seconds
    


    ./vector-try-wait -t -n 2 -l 100000 -d
    Retries: 1510325
    Time: 0.19 seconds
    
    ./vector-try-wait -t -n 4 -l 50000 -d 
    Retries: 5620105
    Time: 0.80 seconds
    
    ./vector-try-wait -t -n 8 -l 25000 -d 
    Retries: 8854285
    Time: 1.41 seconds
    
    ./vector-try-wait -t -n 16 -l 12500 -d
    Retries: 18162853
    Time: 3.06 seconds
    
    
    ./vector-try-wait -t -n 2 -l 10000000 -d -p
    Retries: 0
    Time: 1.64 seconds
    
    ./vector-try-wait -t -n 4 -l 5000000 -d -p 
    Retries: 0
    Time: 1.49 seconds
    
    ./vector-try-wait -t -n 8 -l 2500000 -d -p 
    Retries: 0
    Time: 0.89 seconds
    
    ./vector-try-wait -t -n 16 -l 1250000 -d -p 
    Retries: 0
    Time: 0.55 seconds
    ```

8. Now let’s look at ```vector-avoid-hold-and-wait.c```. What is the main problem with this approach? How does its performance compare to the other versions, when running both with -p and without it?

    It shows poor performance with the ```-p``` flag because pararell processing is not possible because of the global lock.
    ```
    ./vector-avoid-hold-and-wait -t -n 2 -l 100000 -d
    Time: 0.06 seconds
    ./vector-avoid-hold-and-wait -t -n 4 -l 50000 -d 
    Time: 0.49 seconds
    ./vector-avoid-hold-and-wait -t -n 8 -l 25000 -d
    Time: 0.55 seconds
    ./vector-avoid-hold-and-wait -t -n 16 -l 12500 -d
    Time: 0.56 seconds

    ./vector-avoid-hold-and-wait -t -n 2 -l 10000000 -d -p
    Time: 3.93 seconds
    ./vector-avoid-hold-and-wait -t -n 4 -l 5000000 -d -p 
    Time: 4.39 seconds
    ./vector-avoid-hold-and-wait -t -n 8 -l 2500000 -d -p
    Time: 4.61 seconds
    ./vector-avoid-hold-and-wait -t -n 16 -l 1250000 -d -p
    Time: 4.60 seconds
    ```

9. Finally, let’s look at ```vector-nolock.c```. This version doesn’t use locks at all; does it provide the exact same semantics as the other versions? Why or why not?

    Yes, it provides the same as the other versions. If fetch and add instruction is done atomically there is no need of using locks.

10. Now compare its performance to the other versions, both when threads are working on the same two vectors (no ```-p```) and when each thread is working on separate vectors (-p). How does this no-lock version perform?

    This version has the best performance.
    ```
    ./vector-nolock -t -n 2 -l 100000 -d
    Time: 0.41 seconds
    ./vector-nolock -t -n 4 -l 50000 -d 
    Time: 0.31 seconds
    ./vector-nolock -t -n 8 -l 25000 -d
    Time: 0.26 seconds
    ./vector-nolock -t -n 16 -l 12500 -d
    Time: 0.25 seconds

    ./vector-nolock -t -n 2 -l 1000000 -d -p
    Time: 0.54 seconds
    ./vector-nolock -t -n 4 -l 500000 -d -p 
    Time: 0.33 seconds
    ./vector-nolock -t -n 8 -l 250000 -d -p 
    Time: 0.22 seconds
    ./vector-nolock -t -n 16 -l 125000 -d -p
    Time: 0.22 seconds
    ```