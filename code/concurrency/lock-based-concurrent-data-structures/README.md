# Overview

 In this homework, you’ll gain some experience with writing concurrent code and measuring its performance. Learning to build code that performs well is a critical skill and thus gaining a little experience here with it is quite worthwhile.

# Questions

1. We’ll start by redoing the measurements within this chapter. Use the call ```gettimeofday()``` to measure time within your program. How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the cycle counter available on x86 via the ```rdtsc``` instruction.

    The resoultion of```gettimeofday()``` is 1000 ns in Mac OS. Code to measure resolution is [here](./get-timer-resolution.c). (Clock id ```CLOCK_REALTIME``` is used in the code because it is equivalent to the ```gettimeofday```.) ```rdtsc``` has a higher reoultion because it counts number of clock cycles. But It can be wrong when CPU speed change by power-saving measures taken by the OS, or the system be hibernated and later resumed, resetting the TSC.

2. Now, build a simple concurrent counter and measure how long it takes to increment the counter many times as the number of threads increases. How many CPUs are available on the system you are using? Does this number impact your measurements at all?

    [simple concurrent counter](./concurrent-counter.c)
    ```
    running on only one cpu

    number of threads: 1, total increase count: 12000000, total time 0.198736s, final value : 12000000
    number of threads: 2, total increase count: 12000000, total time 0.200319s, final value : 12000000
    number of threads: 3, total increase count: 12000000, total time 0.211258s, final value : 12000000
    number of threads: 4, total increase count: 12000000, total time 0.201875s, final value : 12000000


    one thread per cpu

    number of threads: 1, total increase count: 12000000, total time 0.211998s, final value : 12000000
    number of threads: 2, total increase count: 12000000, total time 0.580595s, final value : 12000000
    number of threads: 3, total increase count: 12000000, total time 0.486172s, final value : 12000000
    number of threads: 4, total increase count: 12000000, total time 0.568907s, final value : 12000000
    ```
    4 cpus are available on my laptop. Using multiple CPUs slow down the result because of the cost of context switch.

3. Next,build a version of the sloppy counter. Once again,measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?


    ```
    bash ./run-sloppy-counter.sh

    threshold : 1
    threads: 1   time: 0.427456s    global: 12000000
    threads: 2   time: 1.278762s    global: 12000000
    threads: 3   time: 1.035457s    global: 12000000
    threads: 4   time: 1.378785s    global: 12000000

    threshold : 8
    threads: 1   time: 0.251333s    global: 12000000
    threads: 2   time: 0.960590s    global: 12000000
    threads: 3   time: 0.893054s    global: 12000000
    threads: 4   time: 0.961532s    global: 12000000

    threshold : 64
    threads: 1   time: 0.229729s    global: 12000000
    threads: 2   time: 0.785679s    global: 12000000
    threads: 3   time: 0.693660s    global: 12000000
    threads: 4   time: 0.811846s    global: 12000000

    threshold : 512
    threads: 1   time: 0.227643s    global: 11999744
    threads: 2   time: 0.904062s    global: 11999232
    threads: 3   time: 0.774055s    global: 11999232
    threads: 4   time: 0.792479s    global: 11999232

    threshold : 16384
    threads: 1   time: 0.226493s    global: 11993088
    threads: 2   time: 0.922105s    global: 11993088
    threads: 3   time: 0.760279s    global: 11993088
    threads: 4   time: 0.753972s    global: 11993088

    threshold : 131072
    threads: 1   time: 0.228227s    global: 11927552
    threads: 2   time: 0.870274s    global: 11796480
    threads: 3   time: 0.679693s    global: 11796480
    threads: 4   time: 0.769445s    global: 11534336

    threshold : 1048576
    threads: 1   time: 0.226977s    global: 11534336
    threads: 2   time: 0.857633s    global: 10485760
    threads: 3   time: 0.679236s    global: 9437184
    threads: 4   time: 0.737452s    global: 8388608
    ```
4. Build a version of a linked list that uses hand-over-hand locking [MS04], as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?

5. Pick your favorite data structure, such as a B-tree or other slightly more interesting structure. Implement it, and start with a simple locking strategy such as a single lock. Measure its performance as the number of concurrent threads increases.

6. Finally, think of a more interesting locking strategy for this favorite data structure of yours. Implement it, and measure its performance. How does it compare to the straightforward locking approach?