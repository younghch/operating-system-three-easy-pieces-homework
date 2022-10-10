
# Overview

This homework uses `disk.py` to familiarize you with how a modern hard
drive works. It has a lot of different options, and unlike most of the other
simulations, has a graphical animator to show you exactly what happens when
the disk is in action. 

You should now have a basic idea of how the simulator works. The questions
below will explore some of the different options, to better help you build a
model of how a disk really works. [details](https://github.com/remzi-arpacidusseau/ostep-homework/tree/master/file-disks)

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


6. Here is a request stream to try: `-a 10,11,12,13`. What goes poorly when it runs? Try adding track skew to address this problem (`-o skew`). Given the default seek rate, what should the skew be to maximize performance? What about for different seek rates (e.g., `-S 2`, `-S 4`)? In general, could you write a formula to figure out the skew?

    Can not read a block 13 sequentially and large rotate time is needed because the seek time is needed to move to the next track. This problem can be solved by giving a skew of 2(`-o 2`) .

    ```
    python3 disk.py -a 10,11,12,13
    
    Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
    Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390
    Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

    TOTALS      Seek: 40  Rotate:425  Transfer:120  Total: 585

    python3  disk.py -a 10,11,12,13 -o 2

    Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
    Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  12  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
    Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

    TOTALS      Seek: 40  Rotate:125  Transfer:120  Total: 285

    python3 disk.py -a 10,11,12,13 -S 2 -o 1 

    Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
    Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  12  Seek: 20  Rotate: 10  Transfer: 30  Total:  60
    Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

    TOTALS      Seek: 20  Rotate:115  Transfer:120  Total: 255

    python3 disk.py -a 10,11,12,13 -o 3 -S 0.5 -G

    Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
    Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  12  Seek: 80  Rotate: 10  Transfer: 30  Total: 120
    Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30    
    ```
    In general, `skew = seek time // rotate time + 1`. (rotate time = time needed to rotate clockwise adjacent block, seek time =  time needed to move to adjacent track, seek time != 0)

7. Specify a disk with different density per zone, e.g., `-z 10,20,30`, which specifies the angular difference between blocks on the outer, middle, and inner tracks. Run some random requests (e.g., `-a -1 -A 5,-1,0`, which specifies that random requests should be used via the -a -1 flag and that five requests ranging from 0 to the max be generated), and compute the seek, rotation, and transfer times. Use different random seeds. What is the bandwidth (in sectors per unit time) on the outer, middle, and inner tracks?

    `python3 disk.py -z 10,20,30 -a -1 -A 5,-1,0 -s 1 `

    Block  |Seek   |Rotate |Transfer   |Total  
    -------|-------|-------|-----------|-------
    7      |0      |245    |10         |255    
    45     |40     |55     |20         |115    
    41     |0      |260    |20         |280    
    13     |40     |335    |10         |385    
    26     |0      |120    |10         |130    
    total  |80     |1015   |70         |1165

    Circle |Bandwidth<br>(sector/unit time)|
    -------|---------|
    inner  |0.1      |
    middle |0.05     |
    outer  |0.033    |
    
8. A scheduling window determines how many requests the disk can examine at once. Generate random workloads (e.g., `-A 1000,-1,0`, with different seeds) and see how long the SATF scheduler takes when the scheduling window is changed from 1 up to the number of requests. How big of a window is needed to maximize performance? Hint: use the -c flag and don’t turn on graphics (-G) to run these quickly. When the scheduling window is set to 1, does it matter which policy you are using?

    Window size of 256 showed the best performance. When scheduling window is set to 1, It would show the same result regardless of the policy used.

    ```
    ./sh satf-window-size-run.sh

    window size : 1
    TOTALS      Seek:19640  Rotate:178315  Transfer:30000  Total:227955

    window size : 2
    TOTALS      Seek:16280  Rotate:101455  Transfer:30000  Total:147735

    window size : 4
    TOTALS      Seek:12040  Rotate:56735  Transfer:30000  Total:98775

    window size : 8
    TOTALS      Seek:8400  Rotate:32655  Transfer:30000  Total:71055

    window size : 16
    TOTALS      Seek:4880  Rotate:18895  Transfer:30000  Total:53775

    window size : 32
    TOTALS      Seek:2200  Rotate:10775  Transfer:30000  Total:42975

    window size : 64
    TOTALS      Seek:1040  Rotate:8545  Transfer:30000  Total:39585

    window size : 128
    TOTALS      Seek:480  Rotate:7305  Transfer:30000  Total:37785

    window size : 256
    TOTALS      Seek:400  Rotate:6665  Transfer:30000  Total:37065

    window size : 512
    TOTALS      Seek:800  Rotate:6625  Transfer:30000  Total:37425

    window size : 1024
    TOTALS      Seek:720  Rotate:6705  Transfer:30000  Total:37425
    ```

9. Create a series of requests to starve a particular request, assuming an SATF policy. Given that sequence, how does it perform if you use a bounded SATF (BSATF) scheduling approach? In this approach, you specify the scheduling window (e.g., -w 4); the scheduler only moves onto the next window of requests when all requests in the current window have been serviced. Does this solve starvation? How does it perform, as compared to SATF? In general, how should a disk make this trade-off between performance and starvation avoidance?

    Yes, It solves starvation but performs bad. 

    ```
    python3 disk.py -a 7,30,8,9,10,11,0,1,2,3,31,4,5 -p SATF

    Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
    Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   9  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  10  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   0  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   1  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   2  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   3  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   4  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   5  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  30  Seek: 80  Rotate:280  Transfer: 30  Total: 390
    Block:  31  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

    TOTALS      Seek: 80  Rotate:295  Transfer:390  Total: 765

    python3 disk.py -a 7,30,8,9,10,11,0,1,2,3,31,4,5 -p BSATF -w 4

    Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
    Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   9  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  30  Seek: 80  Rotate:160  Transfer: 30  Total: 270
    Block:  10  Seek: 80  Rotate: 10  Transfer: 30  Total: 120
    Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   0  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   1  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   2  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   3  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:   4  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
    Block:  31  Seek: 80  Rotate:340  Transfer: 30  Total: 450
    Block:   5  Seek: 80  Rotate:190  Transfer: 30  Total: 300

    TOTALS      Seek:320  Rotate:715  Transfer:390  Total:1425
    ```
10. All the scheduling policies we have looked at thus far are greedy; they pick the next best option instead of looking for an optimal schedule. Can you find a set of requests in which greedy is not optimal?

    ```
    python3 disk.py -a 9,20 -c 

    Block:   9  Seek:  0  Rotate: 75  Transfer: 30  Total: 105
    Block:  20  Seek: 40  Rotate:260  Transfer: 30  Total: 330

    TOTALS      Seek: 40  Rotate:335  Transfer: 60  Total: 435

    python3 disk.py -a 9,20 -c -p SATF

    Block:  20  Seek: 40  Rotate:  5  Transfer: 30  Total:  75
    Block:   9  Seek: 40  Rotate:320  Transfer: 30  Total: 390

    TOTALS      Seek: 80  Rotate:325  Transfer: 60  Total: 465
    ```