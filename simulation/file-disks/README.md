
# Overview

This homework uses `disk.py` to familiarize you with how a modern hard
drive works. It has a lot of different options, and unlike most of the other
simulations, has a graphical animator to show you exactly what happens when
the disk is in action. 

Note: there is also an experimental program, `disk-precise.py`.  This
version of the simulator uses the python Decimal package for precise
floating point computation, thus giving slightly better answers in
some corner cases than `disk.py`. However, it has not been very
carefully tested, so use at your own caution.

Let's do a simple example first. To run the simulator and compute some basic
seek, rotation, and transfer times, you first have to give a list of requests
to the simulator. This can either be done by specifying the exact requests, or
by having the simulator generate some randomly.

We'll start by specifying a list of requests ourselves. Let's do a single
request first:

```sh
prompt> disk.py -a 10
```

At this point you'll see:

```sh
...
REQUESTS [br '10']

For the requests above, compute the seek, rotate, and transfer times.
Use -c or the graphical mode (-G) to see the answers.
```


To be able to compute the seek, rotation, and transfer times for this request,
you'll have to know a little more information about the layout of sectors, the
starting position of the disk head, and so forth. To see much of this
information, run the simulator in graphical mode (-G):

```sh
prompt> ./disk.py -a 10 -G
```

At this point, a window should appear with our simple disk on it. 
The disk head is positioned on the outside track, halfway through sector 6.
As you can see, sector 10 (our example sector) is on the same track, about a
third of the way around. The direction of rotation is counter-clockwise.
To run the simulation, press the "s" key while the simulator window is
highlighted.

When the simulation completes, you should be able to see that the disk spent
105 time units in rotation and 30 in transfer in order to access sector 10,
with no seek time. Press "q" to close the simulator window.

To calculate this (instead of just running the simulation), you would need to
know a few details about the disk. First, the rotational speed is by default
set to 1 degree per time unit. Thus, to make a complete revolution, it takes
360 time units. Second, transfer begins and ends at the halfway point between
sectors. Thus, to read sector 10, the transfer begins halfway between 9 and 10,
and ends halfway between 10 and 11.  Finally, in the default disk, there are
12 sectors per track, meaning that each sector takes up 30 degrees of the
rotational space. Thus, to read a sector, it takes 30 time units (given our
default speed of rotation).

With this information in hand, you now should be able to compute the seek,
rotation, and transfer times for accessing sector 10. Because the head starts
on the same track as 10, there is no seek time. Because the disk rotates at
1 degree / time unit, it takes 105 time units to get to the beginning of sector
10, halfway between 9 and 10 (note that it is exactly 90 degrees to the middle
of sector 9, and another 15 to the halfway point). Finally, to transfer the
sector takes 30 time units.

Now let's do a slightly more complex example:

```sh
prompt> ./disk.py -a 10,11 -G
```

In this case, we're transferring two sectors, 10 and 11. How long will it take?
Try guessing before running the simulation!

As you probably guessed, this simulation takes just 30 time units longer, to
transfer the next sector 11. Thus, the seek and rotate times remain the same,
but the transfer time for the requests is doubled. You can in fact see these
sums across the top of the simulator window; they also get printed out to the
console as follows:

```sh
...
Sector:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Sector:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
TOTALS       Seek:  0  Rotate:105  Transfer: 60  Total: 165
```

Now let's do an example with a seek. Try the following set of requests:

```sh
prompt> disk.py -a 10,18 -G
```

To compute how long this will take, you need to know how long a seek will
take. The distance between each track is by default 40 distance units, and the
default rate of seeking is 1 distance unit per unit time. Thus, a seek from
the outer track to the middle track takes 40 time units. 

You'd also have to know the scheduling policy. The default is FIFO, though, so
for now you can just compute the request times assuming the processing order
matches the list specified via the `-a` flag.

To compute how long it will take the disk to service these requests, we first
compute how long it takes to access sector 10, which we know from above to be
135 time units (105 rotating, 30 transferring). Once this request is complete,
the disk begins to seek to the middle track where sector 18 lies, taking 40
time units. Then the disk rotates to sector 18, and transfers it for 30 time
units, thus completing the simulation. But how long does this final rotation
take?

To compute the rotational delay for 18, first figure out how long the disk
would take to rotate from the end of the access to sector 10 to the beginning
of the access to sector 18, assuming a zero-cost seek. As you can see from the
simulator, sector 10 on the outer track is lined up with sector 22 on the middle
track, and there are 7 sectors separating 22 from 18 (23, 12, 13, 14, 15, 16,
and 17, as the disk spins counter-clockwise). Rotating through 7 sectors takes
210 time units (30 per sector). However, the first part of this rotation is
actually spent seeking to the middle track, for 40 time units. Thus, the
actual rotational delay for accessing sector 18 is 210 minus 40, or 170 time
units. Run the simulator to see this for yourself; note that you can run
without graphics and with the "-c" flag to just see the results without
seeing the graphics.

```sh
prompt> ./disk.py -a 10,18 -c
...
Sector:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Sector:  18  Seek: 40  Rotate:170  Transfer: 30  Total: 240
TOTALS       Seek: 40  Rotate:275  Transfer: 60  Total: 375
```

You should now have a basic idea of how the simulator works. The questions
below will explore some of the different options, to better help you build a
model of how a disk really works.

# Questions
1. Compute the seek, rotation, and transfer times for the following sets of requests: -a 0, -a 6, -a 30, -a 7,30,8, and finally -a 10,11,12,13.

    **Head is always on the middle of sector 6.**

    1. `python3 disk.py -a 0 `    

        Block  |Seek   |Rotate |Transfer   |Total  
        -------|-------|-------|-----------|-------
        0      |0      |165    |30         |195    

    1. `python3 disk.py -a 6 `    

        Block  |Seek   |Rotate |Transfer   |Total  
        -------|-------|-------|-----------|-------
        6      |0      |345    |30         |375    
    
    1. `python3 disk.py -a 7,30,8 `    

        Block  |Seek   |Rotate |Transfer   |Total  
        -------|-------|-------|-----------|-------
        7      |0      |15     |30         |45    
        30     |80     |220    |30         |330    
        8      |80     |310    |30         |420    
        total  |160    |545    |90         |795    
        
    1. `python3 disk.py -a 10,11,12,13 `    

        Block  |Seek   |Rotate |Transfer   |Total  
        -------|-------|-------|-----------|-------
        10     |0      |105    |30         |165    
        11     |0      |0      |30         |30    
        12     |40     |320    |30         |390    
        13     |0      |0      |30         |30    
        total  |40     |425    |120        |585    
    
    

2. Do the same requests above, but change the seek rate to different values: `-S 2, -S 4, -S 8, -S 10, -S 40, -S 0.1`. How do the times change?

    Faster seek speed reduces the total time. But when seek speed is large enough that rotate time to the clockwise adjacent block is smaller than the seek time to the adjacent track, increasing seek speed does not decrease the total time.

    1. `python3 disk.py -a 7,30,8 -S 0.1`    

        ```
        Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
        Block:  30  Seek:801  Rotate:219  Transfer: 30  Total:1050
        Block:   8  Seek:801  Rotate:309  Transfer: 30  Total:1140

        TOTALS      Seek:1602  Rotate:543  Transfer: 90  Total:2235
        ```  

    1. `python3 disk.py -a 7,30,8 -S 2`    

        ```
        Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
        Block:  30  Seek: 40  Rotate:260  Transfer: 30  Total: 330
        Block:   8  Seek: 40  Rotate:350  Transfer: 30  Total: 420

        TOTALS      Seek: 80  Rotate:625  Transfer: 90  Total: 795
        ```  
    1. `python3 disk.py -a 7,30,8 -S 8`    

        ```
        Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
        Block:  30  Seek: 10  Rotate:290  Transfer: 30  Total: 330
        Block:   8  Seek: 10  Rotate: 20  Transfer: 30  Total:  60

        TOTALS      Seek: 20  Rotate:325  Transfer: 90  Total: 435
        ```  
    1. `python3 disk.py -a 7,30,8 -S 40`    

        ```
        Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
        Block:  30  Seek:  2  Rotate:298  Transfer: 30  Total: 330
        Block:   8  Seek:  2  Rotate: 28  Transfer: 30  Total:  60

        TOTALS      Seek:  4  Rotate:341  Transfer: 90  Total: 435
        ```  
        
  
3. Do the same requests above, but change the rotation rate: `-R 0.1,-R 0.5,-R 0.01`. How do the times change?

    Higher roatition rate decrease the total time.

    1. `python3 disk.py -a 7,30,8 -R 0.01`    

        ```
        Block:   7  Seek:  0  Rotate:1500  Transfer:3000  Total:4500
        Block:  30  Seek: 80  Rotate:29920  Transfer:3001  Total:33001
        Block:   8  Seek: 80  Rotate:2920  Transfer:2999  Total:5999

        TOTALS      Seek:160  Rotate:34340  Transfer:9000  Total:4350
        ```  

    1. `python3 disk.py -a 7,30,8 -R 0.1`    

        ```
        Block:   7  Seek:  0  Rotate:150  Transfer:299  Total: 449
        Block:  30  Seek: 80  Rotate:2920  Transfer:301  Total:3301
        Block:   8  Seek: 80  Rotate:219  Transfer:300  Total: 599

        TOTALS      Seek:160  Rotate:3289  Transfer:900  Total:4349
        ```  

    1. `python3 disk.py -a 7,30,8 -R 0.5`    

        ```
        Block:   7  Seek:  0  Rotate: 30  Transfer: 60  Total:  90
        Block:  30  Seek: 80  Rotate:520  Transfer: 60  Total: 660
        Block:   8  Seek: 80  Rotate:700  Transfer: 60  Total: 840

        TOTALS      Seek:160  Rotate:1250  Transfer:180  Total:1590
        ```  


4. FIFO is not always best, e.g., with the request stream `-a 7,30,8`,whatorder should the requests be processed in? Run the shortest seek-time first (SSTF) scheduler (-p SSTF) on this workload; how long should it take (seek, rotation, transfer) for each request to be served?

    Block  |Seek   |Rotate |Transfer   |Total  
    -------|-------|-------|-----------|-------
    7      |0      |15     |30         |45    
    8      |0      |0      |30         |30    
    30     |80     |190    |30         |300    
    total  |80     |205    |90         |375    

5. Now use the shortest access-time first(SATF)scheduler(`-p SATF`).Does it make any difference for `-a 7,30,8` workload? Find a set of requests where SATF outperforms SSTF; more generally, when is SATF better than SSTF?

    SATF makes a same result for `-a 7,30,8`.

    ```
    Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
    Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  30  Seek: 80  Rotate:190  Transfer: 30  Total: 300

    TOTALS      Seek: 80  Rotate:205  Transfer: 90  Total: 375
    ```

    Set of requests where SATF outperforms SSTF
    ```
    python3 disk.py -a 7,22,1,16,7,22,1,16,7 -p SATF
    
    Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
    Block:  22  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:   1  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:  16  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:   7  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:  22  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:   1  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:  16  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:   7  Seek: 40  Rotate: 20  Transfer: 30  Total:  90

    TOTALS      Seek:320  Rotate:175  Transfer:270  Total: 765
    
    python3 disk.py -a 7,22,1,16,7,22,1,16,7 -p SSTF

    Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
    Block:   1  Seek:  0  Rotate:150  Transfer: 30  Total: 180
    Block:   7  Seek:  0  Rotate:150  Transfer: 30  Total: 180
    Block:   1  Seek:  0  Rotate:150  Transfer: 30  Total: 180
    Block:   7  Seek:  0  Rotate:150  Transfer: 30  Total: 180
    Block:  22  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:  16  Seek:  0  Rotate:150  Transfer: 30  Total: 180
    Block:  22  Seek:  0  Rotate:150  Transfer: 30  Total: 180
    Block:  16  Seek:  0  Rotate:150  Transfer: 30  Total: 180

    TOTALS      Seek: 40  Rotate:1085  Transfer:270  Total:1395
    ```


6. Here is a request stream to try: `-a10,11,12,13`. What goes poorly when it runs? Try adding track skew to address this problem (`-o skew`). Given the default seek rate, what should the skew be to maximize performance? What about for different seek rates (e.g., `-S 2`, `-S 4`)? In general, could you write a formula to figure out the skew?

7. Specify a disk with different density per zone, e.g., -z 10,20,30, which specifies the angular difference between blocks on the outer, middle, and inner tracks. Run some random requests (e.g., -a -1 -A 5,-1,0, which specifies that random requests should be used via the -a -1 flag and that five requests ranging from 0 to the max be generated), and compute the seek, rotation, and transfer times. Use different random seeds. What is the bandwidth (in sectors per unit time) on the outer, middle, and inner tracks?
8. Aschedulingwindowdetermineshowmanyrequeststhediskcanexamine at once. Generate random workloads (e.g., -A 1000,-1,0, with different seeds) and see how long the SATF scheduler takes when the scheduling win- dow is changed from 1 up to the number of requests. How big of a window is needed to maximize performance? Hint: use the -c flag and don’t turn on graphics (-G) to run these quickly. When the scheduling window is set to 1, does it matter which policy you are using?
9. Create a series of requests to starve a particular request, assuming an SATF policy. Given that sequence, how does it perform if you use a bounded SATF (BSATF) scheduling approach? In this approach, you specify the scheduling window (e.g., -w 4); the scheduler only moves onto the next window of requests when all requests in the current window have been ser- viced. Does this solve starvation? How does it perform, as compared to SATF? In general, how should a disk make this trade-off between perfor- mance and starvation avoidance?
10. Alltheschedulingpolicieswehavelookedatthusfararegreedy;theypick the next best option instead of looking for an optimal schedule. Can you find a set of requests in which greedy is not optimal?