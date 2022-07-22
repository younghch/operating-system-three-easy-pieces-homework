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

    It quite matches the result. The reason of different result when running 4 threads is context switch between sloppy-counter process and other process running when tested( I guess it is VSCode.).

    ```
    bash ./run-sloppy-counter.sh

    threshold : 1
    threads: 1   time: 0.447273s    global: 12000000
    threads: 2   time: 0.980402s    global: 12000000
    threads: 3   time: 0.970632s    global: 12000000
    threads: 4   time: 1.383397s    global: 12000000

    threshold : 8
    threads: 1   time: 0.253008s    global: 12000000
    threads: 2   time: 0.281764s    global: 12000000
    threads: 3   time: 0.322712s    global: 12000000
    threads: 4   time: 0.630252s    global: 12000000

    threshold : 64
    threads: 1   time: 0.230059s    global: 12000000
    threads: 2   time: 0.140111s    global: 12000000
    threads: 3   time: 0.103200s    global: 12000000
    threads: 4   time: 0.584832s    global: 12000000

    threshold : 512
    threads: 1   time: 0.226625s    global: 11999744
    threads: 2   time: 0.123365s    global: 11999232
    threads: 3   time: 0.085416s    global: 11999232
    threads: 4   time: 0.593103s    global: 11999232

    threshold : 4096
    threads: 1   time: 0.227715s    global: 11997184
    threads: 2   time: 0.120788s    global: 11993088
    threads: 3   time: 0.090564s    global: 11993088
    threads: 4   time: 0.587647s    global: 11993088

    threshold : 32768
    threads: 1   time: 0.226806s    global: 11993088
    threads: 2   time: 0.120094s    global: 11993088
    threads: 3   time: 0.082023s    global: 11993088
    threads: 4   time: 0.561524s    global: 11927552

    threshold : 262144
    threads: 1   time: 0.229490s    global: 11796480
    threads: 2   time: 0.119738s    global: 11534336
    threads: 3   time: 0.083025s    global: 11796480
    threads: 4   time: 0.569183s    global: 11534336

    threshold : 2097152
    threads: 1   time: 0.227154s    global: 10485760
    threads: 2   time: 0.119548s    global: 8388608
    threads: 3   time: 0.081986s    global: 6291456
    threads: 4   time: 0.578152s    global: 8388608
    ```

4. Build a version of a linked list that uses hand-over-hand locking [MS04](http://people.csail.mit.edu/shanir/publications/concurrent-data-structures.pdf), as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?

    It seems the cost of locking and unlocking nodes are higher than the benefit of being able to parallel access the list.

    ```
    ===========================================
    list size: 1024
    number of threads: 1
    num_of_search: 64

    linear search for hand-over-hand list
    time: 0.003212s

    linear search for concurrent-linked list
    time: 0.000947s

    random search for hand-over-hand list
    time: 0.001901s

    random search for concurrent-linked list
    time: 0.000496s

    ===========================================
    list size: 1024
    number of threads: 2
    num_of_search: 64

    linear search for hand-over-hand list
    time: 0.002688s

    linear search for concurrent-linked list
    time: 0.001035s

    random search for hand-over-hand list
    time: 0.001330s

    random search for concurrent-linked list
    time: 0.000650s

    ===========================================
    list size: 1024
    number of threads: 4
    num_of_search: 64

    linear search for hand-over-hand list
    time: 0.002052s

    linear search for concurrent-linked list
    time: 0.001054s

    random search for hand-over-hand list
    time: 0.002886s

    random search for concurrent-linked list
    time: 0.000668s

    ===========================================
    list size: 1024
    number of threads: 8
    num_of_search: 64

    linear search for hand-over-hand list
    time: 0.002776s

    linear search for concurrent-linked list
    time: 0.001257s

    random search for hand-over-hand list
    time: 0.001202s

    random search for concurrent-linked list
    time: 0.000705s

    ===========================================
    list size: 4096
    number of threads: 1
    num_of_search: 256

    linear search for hand-over-hand list
    time: 0.029648s

    linear search for concurrent-linked list
    time: 0.004945s

    random search for hand-over-hand list
    time: 0.008480s

    random search for concurrent-linked list
    time: 0.002360s

    ===========================================
    list size: 4096
    number of threads: 2
    num_of_search: 256

    linear search for hand-over-hand list
    time: 0.011836s

    linear search for concurrent-linked list
    time: 0.005291s

    random search for hand-over-hand list
    time: 0.006183s

    random search for concurrent-linked list
    time: 0.003031s

    ===========================================
    list size: 4096
    number of threads: 4
    num_of_search: 256

    linear search for hand-over-hand list
    time: 0.008519s

    linear search for concurrent-linked list
    time: 0.005487s

    random search for hand-over-hand list
    time: 0.003664s

    random search for concurrent-linked list
    time: 0.002832s

    ===========================================
    list size: 4096
    number of threads: 8
    num_of_search: 256

    linear search for hand-over-hand list
    time: 0.010884s

    linear search for concurrent-linked list
    time: 0.005642s

    random search for hand-over-hand list
    time: 0.004743s

    random search for concurrent-linked list
    time: 0.002794s

    ===========================================
    list size: 16384
    number of threads: 1
    num_of_search: 1024

    linear search for hand-over-hand list
    time: 0.263199s

    linear search for concurrent-linked list
    time: 0.072389s

    random search for hand-over-hand list
    time: 0.136327s

    random search for concurrent-linked list
    time: 0.037049s

    ===========================================
    list size: 16384
    number of threads: 2
    num_of_search: 1024

    linear search for hand-over-hand list
    time: 0.142742s

    linear search for concurrent-linked list
    time: 0.077089s

    random search for hand-over-hand list
    time: 0.078758s

    random search for concurrent-linked list
    time: 0.039959s

    ===========================================
    list size: 16384
    number of threads: 4
    num_of_search: 1024

    linear search for hand-over-hand list
    time: 0.084757s

    linear search for concurrent-linked list
    time: 0.084834s

    random search for hand-over-hand list
    time: 0.047958s

    random search for concurrent-linked list
    time: 0.042764s

    ===========================================
    list size: 16384
    number of threads: 8
    num_of_search: 1024

    linear search for hand-over-hand list
    time: 0.100742s

    linear search for concurrent-linked list
    time: 0.083336s

    random search for hand-over-hand list
    time: 0.061781s

    random search for concurrent-linked list
    time: 0.042696s

    ===========================================
    list size: 65536
    number of threads: 1
    num_of_search: 4096

    linear search for hand-over-hand list
    time: 4.310899s

    linear search for concurrent-linked list
    time: 1.619928s

    random search for hand-over-hand list
    time: 2.162882s

    random search for concurrent-linked list
    time: 0.698931s

    ===========================================
    list size: 65536
    number of threads: 2
    num_of_search: 4096

    linear search for hand-over-hand list
    time: 2.368641s

    linear search for concurrent-linked list
    time: 1.737049s

    random search for hand-over-hand list
    time: 1.162360s

    random search for concurrent-linked list
    time: 0.717006s

    ===========================================
    list size: 65536
    number of threads: 4
    num_of_search: 4096

    linear search for hand-over-hand list
    time: 1.269051s

    linear search for concurrent-linked list
    time: 1.826516s

    random search for hand-over-hand list
    time: 0.632811s

    random search for concurrent-linked list
    time: 0.791201s

    ===========================================
    list size: 65536
    number of threads: 8
    num_of_search: 4096

    linear search for hand-over-hand list
    time: 1.368692s

    linear search for concurrent-linked list
    time: 1.813958s

    random search for hand-over-hand list
    time: 0.852183s

    random search for concurrent-linked list
    time: 0.780220s
    ```


5. Pick your favorite data structure, such as a B-tree or other slightly more interesting structure. Implement it, and start with a simple locking strategy such as a single lock. Measure its performance as the number of concurrent threads increases.

6. Finally, think of a more interesting locking strategy for this favorite data structure of yours. Implement it, and measure its performance. How does it compare to the straightforward locking approach?