# SUSTech CS302 OS Project1 Report

#### Title:      Pintos                                                  

#### Student: 徐逸飞 11611209

#### Time:    2019/03/15

#### Experimental Environment:  1. Ubuntu 16.04 2. pintos

####Tasks:

>	*Task 1: Efficient Alarm Clock
	Task 2: Priority Scheduler
	Task 3: Multi-level Feedback Queue Scheduler (MLFQS)
	Task 4: Test Pintos with GDB*

# Task 1: Efficient Alarm Clock

## Data structures and functions

**Addition**

1. `int64_t ticks_blocked` - a propertiy of **struct thread** to identify the waiting time of each thread, which should be initialized to 0.
2. `check_threads()` - a new function in both **timer.c** and **timer.h** to check every thread's waiting time and it uses `thread_unblock()` to awake threads in case. 

**Modification**:

1. `thread_create()` - add codes that initialize the `ticks_blocked` of a thread to 0.

2. `timer_interrupt()` - add `check_threads()` in it.

3. `timer_sleep()` - add `thread_block()`.

## Algorithms

​    When call `timer_sleep()`, I put the current thread to sleep by calling `thread_block()`, which makes the current thread unable to be scheduled again, and set the `ticks_blocked` of the current thread to an appropriate number. Then because `timer_interrupt()` is executed per second, every thread's `ticks_blocked`  whose value is not zero is checked and decreased by 1 and if it becomes zero, it will be awake and ready again. Then call `thread_yield()` to decide which thread should be run next.

## Synchronization

1. Interruption considered as an issue, so I use following codes to disable the interruption during the execution of `thread_sleep()`.

```c
  enum intr_level old_level;
  old_level = intr_disable ();
//-------codes start----------

//-------codes end------------
  intr_set_level (old_level);
```



## Rationale 

**Advantages**

Using `timer_block()` deals with ***busy while*** issue and increases the efficiency of execution. Additionally, it is easy to implment and the lines of modified codes are few.

**Disadvantages**

Function ``timer_interrupt()` should run as fast as possible so that adding addtional codes in it may cause issues.

# Task 2: Priority Scheduler

## Data structures and functions

**Addition**

1. `int base_priority` - a property of **struct thread** to store an initial priority.
2. `struct list locks` -  a property of **struct thread** to store obtained locks.
3. `struct lock *acquired_lock ` -  a property of **struct thread** to store the pointer of an acquired lock, null if not waiting for any lock.
4. `int max_priority` - a property of **struct lock** to store the maximum prioriy of waiting threads in `struct list waiters` in **synch.h**.
5. `struct list_elem elem ` - a property of **struct lock** to store the thread in its waiting list with the highest priority.
6. `thread_compare_priority()` - a new function in both **thread.c** and **thread.h** to compare two threads.

**Modification**:

1. `thread_unblock(), thread_yield() and thread_init()`  - change `list_push_back()` to `list_insert_ordered()` to maintain a priority queue of ready threads .
2. `thread_set_priority() and thread_create() ` - add `thread_yield()` at the end of these functions, which to ensure that once a thread's priority changes, another thread with higher priority has chance to run immediately.
3. `struct list waiters` of `struct semaphore ` in **synch.h** - modified to a priority queue.

## Algorithms

- **Choose the next thread to run**

    ​    Instead of choosing a thread from **ready-list** randomly, I change the ready-list to a prioriy queue so that I change most `list_push_back()` to `list_insert_ordered()`. Every time a thread is created or called by `thread_set_priority()`,  all ready threads should be rescheduled by calling `thread_yield()`.

- **Acquiring a Lock**

    ​    Firstly, the current thread, represented as **A** , verify whether the target lock is occupied. If no then **A** can acquire the lock successfully, otherwise it will be added to this lock's waiting list, an ordered queue. Then **A** will donate its priority to the lock holder thread **B**. Furthermore, if **B** is waiting on another lock whose holder is thread **C**, then **B** will also donate its priority, equal to or larger than  **A**'s, to **C** and so does **C** like a recusive process.

- **Releasing a Lock**

    1. After removing lock **x** in the current thread **A**'s locks list, **A** will access  locks list and change its priority to the bigger one between **A**'s `base_priority` and a lock's `max_priority` which is the highest in **A**'s locks list. 
    2. **A** will access the `waiting_list` of **x** and pick up a thread **B** with the highest priority, which may be a donor of the current thread, and give **x** to **B**.

- **Computing the effective priority**

    The effective priority of thread **A** is determined by following things.

    1. The highest priority of the lock in **A**'s locks list
    2. The `basic_priority` of **A**

- **Priority scheduling for semaphores and locks** 

    ​    Every time a thread requests a lock **x**, it will check **x**'s `max_priority` and try to increase it. And the effect will transfer through the requesting chain of **x**.

- **Changing thread’s priority**

    1. If a thread is already donated, then we should operate on its `basic_priority` and compare `basic_priority` with its current priority.
    2. After a thread releases a lock, we should check whether it obtains other locks and set its current priority to the maximum value among its `basic priority` and its locks list.

## Synchronization

1. Interruptions

   Interruptions are turned off by following codes.

   ```c
     enum intr_level old_level;
     old_level = intr_disable ();
   //-------codes start----------
   
   //-------codes end------------
     intr_set_level (old_level);
   ```



## Rationale 

​    Donors are memorized by a lock rather than donation receivers. It saves memory because if threads remember their donors, the same donor may be memorized twice. Additionally, allocating memory and freeing them every time that a thread gains and releases a lock waste more time than just adding or deleting lock holders by a lock itself.

# Task 3: Multi-level Feedback Queue Scheduler (MLFQS)

## Data structures and functions

**Addition**

1. `fixed_t load_avg` - an global value with initial value 0 defined in **thread.h**
2. `int nice` - add to `struct thread` in **thread.h** 
3. `fixed_t recent_cpu` - add to `struct thread` in **thread.h**
4. `thread_set_nice (int new_nice)` - Sets the current thread’s nice value to **new_nice** and recalculates the thread’s priority based on the new value. 
5. `thread_get_nice()` - Returns the current thread’s nice value.
6. `thread_get_load_avg()` - Returns 100 times the current system load average, rounded to the nearest integer.
7. `thread_get_recent_cpu()` - Returns 100 times the current thread’s recent_cpu value, rounded to the nearest integer.
8. `thread_calculate_priority()` - Use **priority = PRI_MAX − ( recent_cpu $\div$ 4 ) − ( nice $\times$ 2 )**
9. `thread_calculate_recent_cpu()` - Use **recent_cpu = ( 2 × load_avg ) $\div​$ ( 2 × load_avg + 1 ) × recent_cpu + nice**
10. `thread_calculate_load_avg()` - Use **load_avg = ( 59 $ \div $ 60 )× load_avg + ( 1$ \div $60 ) × ready_threads**

**Modification**

1. `time_interrupt()` - add codes that calculate **load_avg** and **recent_cpu** per second based on`TIMER_FREQ` and recalculate priority of every thread every 4 clock ticks.

2. `thread_create()` - initialize the value of `recent_cpu` and `nice`

## Algorithms

​    Instead of donating priorities, MLFQS enables timer to adjust the priority of each thread every 4 ticks by functions in **Addition** part ***8, 9, 10***. Therefore the ready list will be refreshed automatically and the current thread will be changed timely. Note that every time the nice value and priority of a thread change, `thread_yield()` should be called. 

## Rationale

**Advantages**

1. It is easy to implement such changes and most logic parts are provided by the Tutorial.
2. Every thread even with relatively low priority has opportunities to run because of `recent_cpu`

# Design Document Additional Questions

1. (This question uses the MLFQS scheduler.) Suppose threads A, B, and C have nice values **0**, **1**, and **2**. Each has a recent_cpu value of 0. Fill in the table below showing the scheduling decision and the recent_cpu and priority values for each thread after each given number of timer ticks. We can use **R(A)** and **P(A)** to denote the recent_cpu and priority values of thread A, for brevity.



| timer ticks | R(A) | R(B) | R(C) | R(A) | R(B) | R(C) | thread to run |
| :---------: | :--: | :--: | :--: | :--: | :--: | :--: | :-----------: |
|    **0**    |  0   |  0   |  0   |  63  |  61  |  59  |       A       |
|    **4**    |  4   |  0   |  0   |  62  |  61  |  59  |       A       |
|    **8**    |  8   |  0   |  0   |  61  |  61  |  59  |       B       |
|   **12**    |  8   |  4   |  0   |  61  |  60  |  59  |       A       |
|   **16**    |  12  |  4   |  0   |  60  |  60  |  59  |       B       |
|   **20**    |  12  |  8   |  0   |  60  |  59  |  59  |       A       |
|   **24**    |  16  |  8   |  0   |  59  |  59  |  59  |       C       |
|   **28**    |  16  |  8   |  4   |  59  |  59  |  58  |       B       |
|   **32**    |  16  |  12  |  4   |  59  |  58  |  58  |       A       |
|   **36**    |  20  |  12  |  4   |  58  |  58  |  58  |       C       |



2. Did any ambiguities in the scheduler specification make values in the table (in the previous question) uncertain? If so, what rule did you use to resolve them?

​    Yes. If there comes to two or more threads with the same priority, it is uncertain which thread should start to run.

​    I list some strategies to solve this and it is what I applied to the schedule table, where **N(A)** is the nice value of thread **A**.

- **P(A) = P(B) = P(C)** and **R(A) > R(B) > R(C)**

  The running order should be **C, B, A**.

- **P(A) = P(B) = P(C)**,  **R(A) = R(B) = R(C)** and **N(A) > N(B) > N(C)**

  The running order should be **C, B, A**

- **P(A) = P(B) = P(C)**,  **R(A) = R(B) = R(C)** and **N(A) = N(B) = N(C)**

  The next thread should be chosed randomly.

3. Answer questions about pintos source code

    a. Tell us about how pintos start the first thread in its thread system (only consider the thread part).

    ​    By calling `thread_start()`. 

    

    b. Consider priority scheduling, how does pintos keep running a ready thread with highest priority after its time tick reaching **TIME_SLICE**?

    ​    By calling `thread_yield()` . With ordered insertion, the front of ready list must have the highest priority.

    

    c. What will pintos do when switching from one thread to the other? By calling what functions and doing what?

    ​    By calling `schedule()`. 

    1. In the `schedule()`, pintos calls `next_thread_to_run()` to determine which thread should run next. 
    2. In the `next_thread_to_run()`, if the current thread is not the next thread, then it will call `switch_thread()` to switch threads. 
    3. In the `switch_thread()`, which is in **switch.S** and is programmed in assembly language, pintos stores the current thread's condition and rebuild the running environment of the next thread.

    

4. How does pintos implement floating point number operation?

    By using lower-16 bits to represent the fraction part of a number and higher bits to represent the integer part.

    

5. What do priority-donation test cases(priority-donate-chain and priority-donate-nest) do and illustrate the running process?

    - **priority-donate-chain**

      In my abstract understanding, in the for loop, it does the folloing things:

      1. Create a thread whose priority is determined by ***i***, the iteration number, and the larger the ***i*** is, the higher priority a thread is.

      2. Once a thread is created, it will run `donor_thread_func()` and is willing to acquire the lock owned by the previous thread. So this process acts like a chain.

         

    - **priority-donate-nest**

      In my abstract understanding, in the for loop, it does the folloing things:

      1. A thread **L** with lower priority is created firstly and it acquires lock **a**.
      2. A thread **M** with medium priority is created secondly and the current thread switches to **M**. Then **M** acquires lock **b**. But it fails to acquire **a**,so it donates its priority to **a**'s holder **L** and then switch to **L**.
      3. A thread **H** with high priority is created thirdly and the current thread switches to **H**. But it tries to acquire **b**, so it donates its priority to both **M** and **L**, then switch to **L**.
      4. Once **a** is released, **M** blocks **L** due to L's lower priority and so on.
      5. Finally, the finishing order should be **H, M, L**.

# Task 4: Test Pintos with GDB

To be continued.



