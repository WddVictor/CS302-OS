# SUSTech CS302 OS Lab7 Report

#### Title:  **Deadlock**                                                  

#### Student: 徐逸飞 11611209

#### Time:    2019/04/13

#### Experimental Environment:   
- MacOS Mojave
- g++ 4.2.1                              

#### Summit by: Blackboard



### Task：

- Task 1. Implement the Banker’s algorithm.
- Task 2. Finish the report



### Experiments:

1. fundatmental

   - What is deadlock?

     - Deadlock is a situdation where a set of processes are blocked because each process is holding a resource and waiting for the resource held by other processes.

   - What are the requirements of deadlock?

     - Mutual excursion
     - No preemption
     - Hold and wait
     - Circular wait

   - What’s different between deadlock prevention and deadlock avoidance?

     - Deadlock avoidance is to avoid sharing resource between processes.

     - Deadlock prevention is to use a semaphore or a lock to make sure each process is able to run the ciritical section correctly.

       

   - How to prevent deadlock? Give at least two examples.

     - We can prevent deadlock by breaking its four requirements.
       - Mutual excursion:
         - There is no satisfying way to eliminate this requirement due to unsharable resources like the printer.
       - No preemption
         - Releasse the resource when another process with a higher priority requests it.
       - Hold and wait
         - Allocate resources to processes before the start of their execution.
       - Circular wait
         - Number all resources. And processes only can request resources in ascending order. For example, if a process requests for $R_j$, it must release all resources $R_i$ such that i ≥ j. But the problem is how to give an appropriate number to each resource.

   

   - Which way does recent UNIX OS choose to deal with deadlock problem, why?
     - I think it does nothing but rolls back.

   

2. Banker’s algorithm

   - What data structures you use in your implementation? Where and why you use them? Are they optimal for your purpose?
     - **map**
       - Three maps are used as global variables.
       -  They are used to store the information of the allocation of resources, the maximum amount of needed resources, and the termination of processes, because the id of processes are out of order and we unknow the next process's id.
       - I think they are satisfying because I can just use the id of processes as index to access their allocation and maximum requirement of resources.
       - I also considered using **vector** to store the correspondence between the process and its allocation. However, I think it is time-cost to search the whole list everytime with a time complexity ***O(n)*** to get the allocation of resources.
     - **int[]**
       - Several integer arrays are used as the **value** part of maps.
       - They are aimed at storing the allocation and maximum amounts of needed resources of processes. 
       - They are optimal for my purpose.

### Conclusion:

1. A **key** will be created after an access no matter whether the **value** part existes or not when I am using **map**.

2. If I want to delete an element in a map, I should delete or free the **value** part and then call `map.erase(key)` to delete the **key** part.
3. An element in an array could not be assigned if I initialized its length at first.