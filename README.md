#  and practice codes of [Operating System Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

# Link to the answer of each chapter
**link starts with 'Run' is still working on**

***if you found something wrong with the answers please make pull request or send me an email at [younghch42@gmail.com](malito:younghch42@gmail.com)***

## Virtualization

Chapter | What is homework about
--------|-----------
[Process API](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf) | [write some codes to get familiar with process management api](code/virtualization/process-api)
[Direct Execution](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-mechanisms.pdf) | [measure cost of a system call and context switch](code/virtualization/limited-direct-execution)
[Scheduling Basics](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf) | [compare SJF, FIFO, RR schedulers under various conditions](simulation/cpu-sched)
[MLFQ Scheduling](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-mlfq.pdf)	| [see how a MLFQ scheduler behaves](simulation/cpu-sched-mlfq)
[Lottery Scheduling](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-lottery.pdf) | [see how a lottery scheduler works](simulation/cpu-sched-lottery)
[Multiprocessor Scheduling](http://www.cs.wisc.edu/~remzi/OSTEP/cpu-sched-multi.pdf) | Run multi.py
[Abstraction: Address Spaces](http://www.cs.wisc.edu/~remzi/OSTEP/vm-intro.pdf) | [examine virtual memory usage on Linux-based systems](code/virtualization/address-space)
[Memory API](http://www.cs.wisc.edu/~remzi/OSTEP/vm-api.pdf) | [write buggy program and use tools to find the bug](code/virtualization/memory-api)
[Relocation](http://www.cs.wisc.edu/~remzi/OSTEP/vm-mechanism.pdf) | [calculate translated address and check valid](simulation/vm-mechanism)
[Segmentation](http://www.cs.wisc.edu/~remzi/OSTEP/vm-segmentation.pdf) | [calculate translated address with segmentation](simulation/vm-segmentation)
[Free Space](http://www.cs.wisc.edu/~remzi/OSTEP/vm-freespace.pdf) | [calculate and compare best/worst/first fit](simulation/vm-freespace)
[Paging](http://www.cs.wisc.edu/~remzi/OSTEP/vm-paging.pdf) | [work with paging](simulation/vm-paging)
[TLBs](http://www.cs.wisc.edu/~remzi/OSTEP/vm-tlbs.pdf) | [measure size of TLB](code/virtualization/paging)
[Multi-level Paging](http://www.cs.wisc.edu/~remzi/OSTEP/vm-smalltables.pdf) | [translate address for multi-level paging](simulation/vm-smalltables)
[Paging Mechanism](http://www.cs.wisc.edu/~remzi/OSTEP/vm-beyondphys.pdf) | [measure performance affect of swapping](simulation/vm-beyondphys)
[Paging Policy](http://www.cs.wisc.edu/~remzi/OSTEP/vm-beyondphys-policy.pdf) | [compare paging policies](simulation/vm-beyondphys-policy)
[Complete VM](http://www.cs.wisc.edu/~remzi/OSTEP/vm-complete.pdf) | No homework

## Concurrency

Chapter | What is homework about
--------|-----------
[Threads Intro](http://www.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf) | [learn the condition of critical section](simulation/threads-intro)
[Thread API](http://www.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)	| [run some c codes to understand the benefit of using mutex](simulation/threads-api)
[Locks](http://www.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)	| [Compare various locking algorithms](simulation/threads-locks)
[Lock Usage](http://www.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf) | [Build concurrent data structures and measure its performance](code/concurrency/lock-based-concurrent-data-structures)
[Condition Variables](http://www.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf) | [Learn the right usage of condition variables](simulation/threads-cv)
[Semaphores](http://www.cs.wisc.edu/~remzi/OSTEP/threads-sema.pdf) | Read and write [some code](threads-sema)
[Concurrency Bugs](http://www.cs.wisc.edu/~remzi/OSTEP/threads-bugs.pdf) | Run [some C code](threads-bugs)
[Event-based Concurrency](http://www.cs.wisc.edu/~remzi/OSTEP/threads-events.pdf) | 

## Persistence

Chapter | What is homework about
--------|-----------
[Hard Disk Drives](http://www.cs.wisc.edu/~remzi/OSTEP/file-disks.pdf) | 
[RAID](http://www.cs.wisc.edu/~remzi/OSTEP/file-raid.pdf) | 
[FS Intro](http://www.cs.wisc.edu/~remzi/OSTEP/file-intro.pdf) | 
[FS Implementation](http://www.cs.wisc.edu/~remzi/OSTEP/file-implementation.pdf) | Run [vsfs.py](file-implementation)
[Fast File System](http://www.cs.wisc.edu/~remzi/OSTEP/file-ffs.pdf) | Run [ffs.py](file-ffs)
[Crash Consistency and Journaling](http://www.cs.wisc.edu/~remzi/OSTEP/file-journaling.pdf) | Run [fsck.py](file-journaling)
[Log-Structured File Systems](http://www.cs.wisc.edu/~remzi/OSTEP/file-lfs.pdf) | Run [lfs.py](file-lfs)
[Solid-State Disk Drives](http://www.cs.wisc.edu/~remzi/OSTEP/file-ssd.pdf) | Run [ssd.py](file-ssd)
[Data Integrity](http://www.cs.wisc.edu/~remzi/OSTEP/file-integrity.pdf) | Run [checksum.py](file-integrity) and 
[Distributed Intro](http://www.cs.wisc.edu/~remzi/OSTEP/dist-intro.pdf) | 
[NFS](http://www.cs.wisc.edu/~remzi/OSTEP/dist-nfs.pdf) | Write some analysis code
[AFS](http://www.cs.wisc.edu/~remzi/OSTEP/dist-afs.pdf) | Run [afs.py](dist-afs)