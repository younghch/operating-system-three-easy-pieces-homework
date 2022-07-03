
# Overview

In this homework, you'll use a real tool on Linux to find problems in
multi-threaded code. The tool is called `helgrind` (available as part of the
valgrind suite of debugging tools).

See `http://valgrind.org/docs/manual/hg-manual.htm` for details about
the tool, including how to download and install it (if it's not
already on your Linux system).

You'll then look at a number of multi-threaded C programs to see how you can
use the tool to debug problematic threaded code.

First things first: download and install `valgrind` and the related `helgrind` tool. 

Then, type `make` to build all the different programs. Examine the `Makefile`
for more details on how that works.

Then, you have a few different C programs to look at:
- `main-race.c`: A simple race condition
- `main-deadlock.c`: A simple deadlock
- `main-deadlock-global.c`: A solution to the deadlock problem
- `main-signal.c`: A simple child/parent signaling example
- `main-signal-cv.c`: A more efficient signaling via condition variables
- `common_threads.h`: Header file with wrappers to make code check errors and be more readable

With these programs, you can now answer the questions in the textbook.

# Questions
1. First build ```main-race.c```. Examine the code so you can see the (hopefully obvious) data race in the code. Now run helgrind (by typing ```valgrind --tool=helgrind main-race```) to see how it reports the race. Does it point to the right lines of code? What other information does it give to you?

	```
	Possible data race during read of size 4 at 0x10C014 by thread #1
	Locks held: none
		at 0x10922D: main (main-race.c:15)

	This conflicts with a previous write of size 4 by thread #2
	Locks held: none
		at 0x1091BE: worker (main-race.c:8)
		by 0x4842B1A: ??? (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_helgrind-amd64-linux.so)
		by 0x4869608: start_thread (pthread_create.c:477)
		by 0x49A3162: clone (clone.S:95)
		Address 0x10c014 is 0 bytes inside data symbol "balance"
	```
2. What happens when you remove one of the offending lines of code? Now add a lock around one of the updates to the shared variable, and then around both. What does helgrind report in each of these cases?

	See ```main-race-mutex.c``` for the code of adding lock. Both case report no error.

3. Now let’s look at main-deadlock.c. Examine the code. This code has a problem known as deadlock (which we discuss in much more depth in a forthcoming chapter). Can you see what problem it might have?

	Two thread could wait for each other indefinitely if the context switch to p2 happened right after p1 locked m1. p2 would lock m2 and wait for m1 to be unlocked, and p1(which gained m1) would wait for p2 to unlock m2.

4. Now run helgrind on this code. What does helgrind report?

	```Thread #3: lock order "0x10C040 before 0x10C080" violated```

5. Now run helgrind on main-deadlock-global.c. Examine the code; does it have the same problem that main-deadlock.c has? Should helgrind be reporting the same error? What does this tell you about tools like helgrind?

	Yes, it has a same problem. Yes, It should report the same error and it does. For me helgrind seems a good tool to find miss ordered locks.

6. Let’s next look at ```main-signal.c```. This code uses a variable (done) to signal that the child is done and that the parent can now continue. Why is this code inefficient? (what does the parent end up spending its time doing, particularly if the child thread takes a long time to complete?)

	For program ```main-signal``` context switch would happen after running a amount of useless loop instructions. Using a mutex and condition, thread would know it should wait for the variable to be changed by another thread. If condition is not satasified thread would unlock the lock and fall asleep, and aqurie lock and proceed the code when condition is satasified. More efficient cpu utilization can be achieved using mutex.

7. Now run helgrind on this program. What does it report? Is the code
correct?

	No, it's not correct.
	```
	Possible data race during read of size 4 at 0x10C014 by thread #1
	Locks held: none
	   at 0x109239: main (main-signal.c:16)

	This conflicts with a previous write of size 4 by thread #2
	Locks held: none
	   at 0x1091C5: worker (main-signal.c:9)
	   by 0x4842B1A: ??? (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_helgrind-amd64-linux.so)
	   by 0x4869608: start_thread (pthread_create.c:477)
	   by 0x49A3162: clone (clone.S:95)
	 Address 0x10c014 is 0 bytes inside data symbol "done"

	----------------------------------------------------------------

	Possible data race during write of size 1 at 0x527A1A5 by thread #1
	Locks held: none
	   at 0x48488A6: mempcpy (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_helgrind-amd64-linux.so)
	   by 0x4913661: _IO_new_file_xsputn (fileops.c:1236)
	   by 0x4913661: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
	   by 0x4908527: puts (ioputs.c:40)
	   by 0x10924E: main (main-signal.c:18)
	 Address 0x527a1a5 is 21 bytes inside a block of size 1,024 alloc'd
	   at 0x483C893: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_helgrind-amd64-linux.so)
	   by 0x4905D33: _IO_file_doallocate (filedoalloc.c:101)
	   by 0x4915EFF: _IO_doallocbuf (genops.c:347)
	   by 0x4914F5F: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:745)
	   by 0x49136E4: _IO_new_file_xsputn (fileops.c:1244)
	   by 0x49136E4: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
	   by 0x4908527: puts (ioputs.c:40)
	   by 0x1091C4: worker (main-signal.c:8)
	   by 0x4842B1A: ??? (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_helgrind-amd64-linux.so)
	   by 0x4869608: start_thread (pthread_create.c:477)
	   by 0x49A3162: clone (clone.S:95)
	 Block was alloc'd by thread #2
	 ```

8. Now look at a slightly modified version of the code, which is found in
main-signal-cv.c. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?

	This code is preffered more for both. It is more correctness because we ensure the order of execution. When multiple thread are waiting for a variable to be changed, there could be a case multiple threads break the loop and make interrupt right after they are notified that the variable is changed. They update the variable after they are in critical section, but checking was already done. Consequently multiple threads could be in a critical section. The reaon for more performance is already described in q6. 

9. Once again run helgrind on main-signal-cv. Does it report any errors?

	No error.


