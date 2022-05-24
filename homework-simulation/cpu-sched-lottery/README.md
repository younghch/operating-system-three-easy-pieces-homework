
# Overview

This program, lottery.py, allows you to see how a lottery scheduler
works. As always, there are two steps to running the program. First, run
without the -c flag: this shows you what problem to solve without
revealing the answers.  [link](https://github.com/remzi-arpacidusseau/ostep-homework/tree/master/cpu-sched-lottery)

# Questions

1. Compute the solutions for simulations with 3 jobs and randomseeds of 1, 2, and 3.

    ***Seed: 1***

    ***Job List:***
    - Job 0 ( length = 1, tickets = 84 )
    - Job 1 ( length = 7, tickets = 25 )
    - Job 2 ( length = 4, tickets = 44 )

    ***Execution Trace***
    1. Random 651593
        
        - total tickets: 153
        - winning ticket: 651593%153 = 119
        - run: JOB 2(left 3)
    1. Random 788724

        - total tickets: 153
        - winning ticket: 788724%153 = 9
        - run: JOB 0(left 0, Done)

    1. Random 93859

        - total tickets: 69
        - winning ticket: 93859%69 = 19
        - run: JOB 1(left 6)
    1. Random 28347

        - total tickets: 69
        - winning ticket: 28347%69 = 57
        - run: JOB 2(left 2)
    1. Random 835765

        - total tickets: 69
        - winning ticket: 835765%69 = 37
        - run: JOB 2(left 1)

    1. Random 432767
        - total tickets: 69
        - winning ticket: 432767%69 = 68
        - run: JOB 2(left 0, Done)

    1. Random 762280, 2106, 445387, 721540, 228762, 945271
        - run JOB 1 until done


2. Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., -l 10:1,10:100). What happens when the number of tickets is so imbalanced? Will job 0 ever run before job 1 completes? How often? In general, what does such a ticket imbalance do to the behavior of lottery scheduling?

    Job 0 almost never gets a chance of running, so it is impossible to complete before job 1. The probability of Job 0 end before Job 1 is nearly 10 power of -20. In general, it acts as running larger probability job first.

3. When running with two jobs of length 100 and equal ticket alloca- tions of 100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.


4. How does your answer to the previous question changeas the quantum size (-q) gets larger?
 
    As quantum size gets largert, unfairness gets larger.

5. Can you make a version of the graph that is found in the chapter? What else would be worth exploring? How would the graph look with a stride scheduler?

    With a stride schedular, fairness will be **JOB_LENGTH/(JOB_LENGTH+1)**. As length of the job gets higher, fairness gets closer to one.
