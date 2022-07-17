# Overview

 In this homework, you’ll gain some experience with writing concurrent code and measuring its performance. Learning to build code that performs well is a critical skill and thus gaining a little experience here with it is quite worthwhile.

# Questions

1. We’ll start by redoing the measurements within this chapter. Use the call ```gettimeofday()``` to measure time within your program. How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the cycle counter available on x86 via the ```rdtsc``` instruction.

    The resoultion of```gettimeofday()``` is 1000 ns in Mac OS. Code to measure resolution is [here](./get-timer-resolution.c). (Clock id ```CLOCK_REALTIME``` is used in the code because it is equivalent to the ```gettimeofday```.) ```rdtsc``` has a higher reoultion because it counts number of clock cycles. But It can be wrong when CPU speed change by power-saving measures taken by the OS, or the system be hibernated and later resumed, resetting the TSC.

2. Now, build a simple concurrent counter and measure how long it takes to increment the counter many times as the number of threads increases. How many CPUs are available on the system you are using? Does this number impact your measurements at all?

    [simple concurrent counter](./concurrent-counter.c)
    ```
    running on only one cpu

    number of threads: 1 increase count each: 10000000 total time 0.167942s
    number of threads: 2 increase count each: 10000000 total time 0.326556s
    number of threads: 3 increase count each: 10000000 total time 0.485321s
    number of threads: 4 increase count each: 10000000 total time 0.645143s

    one thread per cpu

    number of threads: 1 increase count each: 10000000 total time 0.168496s
    number of threads: 2 increase count each: 10000000 total time 1.139014s
    number of threads: 3 increase count each: 10000000 total time 1.725517s
    number of threads: 4 increase count each: 10000000 total time 1.971672s

    ```
    4 cpus are available on my laptop. Using multiple CPUs slow down the result because of the cost of context switch.

3. Next,build a version of the sloppy counter. Once again,measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?

4. Build a version of a linked list that uses hand-over-hand locking [MS04], as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?

5. Pick your favorite data structure, such as a B-tree or other slightly more interesting structure. Implement it, and start with a simple locking strategy such as a single lock. Measure its performance as the number of concurrent threads increases.

6. Finally, think of a more interesting locking strategy for this favorite data structure of yours. Implement it, and measure its performance. How does it compare to the straightforward locking approach?