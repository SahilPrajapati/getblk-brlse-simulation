# getblk-brlse-simulation
Simulation of "getblk" and "brlse" algorithms in a multiprocessing environment.

The simulation done using threads with the help of a buffer pool(buffer cache) structure. Used mutex locks and condition variables for efficient interleaving of processes.




## Step 1. To create Buffer Cache

### 1. How to implement Buffer Cache ?
For Buffer Cache implemented a doubly linked list(file named as dll.h) having following functions which has to be implemented:


-> **structure of node in doubly linked list**
  - int data 
  - node *next 
  - node *prev 
  - int statusf_b (0-free,1-busy)
  - int status2 (0-invalid,1-valid,2-delayed write)
  
‐> **insert_at_start()**:‐needed when buffer marked as delayed write and kernel performs asynchronous write on disk block, after completion that buffer will be added at front of free list.

‐> **insert_at_end()**:‐ When buffer get unlocked or free then it added at tail of free list.

‐> **delete_from_begin()**:‐When kernel want any free buffer then it will be removed from head of free list.

‐> **delete_by_value()**:‐ When kernel want any specific buffer it may be present in middle of the list so we will search buffer by blocknumber. 

‐> **isPresent()**:‐it searches for block whether it is present in buffer cache or not ‐>isEmpty():‐ Used to check whether free list is empty or not.

‐> **getBuffer()**:‐ when kernel found the buffer it returns address of that buffer or node in terms of doubly linked list. 

‐> **change_status()**:‐To change status for specific buffer (from free to busy or busy to free).

-> **change_status2()**:- changes status of given block number to newstatus.

-> **display()**:- To display the contents of the doubly linked list.

-> **isEmpty()**:- To check whether the doubly lined list is empty or not.




### 2. Why we choose doubly linked list ?
We choose doubly linked list for following reasons:
‐ insertion and deletion takes O(1) time and we usually need insertion and deletion from end points.
‐ traversing is not possible in another data structures like queue and stacks. For queue we need to maintain extra data structure like Hashset(to check whether block is present or not).


### 3. How we can maintain hash queue for efficient searching for block in buffer cache ?
For efficient searching we will create array of 4 doubly linked list 
eg:‐DLL hq[4] 
 - blocknum 0 mod 4 in hq[0]
 - blocknum 1 mod 4 in hq[1] 
 - blocknum 2 mod 4 in hq[2] 
 - blocknum 3 mod 4 in hq[3]


## Step 2.To Implement GetBlk() algorithm 
In this file (named as getblk.h) we will implement following methods:

-> **getBlock()**:‐ This function will handle all five cases of getbllk() algorithm 
  - Case 1. The kernel finds the block on its hash queue and its buffer is free.
  - Case 2. The kernel cannot find the block on the hash queue,so it allocates a buffer from the free list. 
  - Case 3. The kernel cannot find the block on the hash queue and in attempting to allocate a buffer from free list , find a buffer on             free list that has marked “delayed write”.
  - Case 4. The kernel cannot find the block on the hash queue and the free list of buffers is empty.
  - Case 5. The kernel finds the block on the hash queue,but its buffer is currently busy.

-> **Handle_write()**:‐ use to mark buffer with delayed write i.e.change buffer status

-> **Handle_writeAsync()**:‐ if buffer marked with delayed write , remove that buffer from free list write its content to disk block and again add that buffer at front of free list.


## Step 3. Driver code
File saved as (main.cpp). 
We will create two threads in main function to implement multi programming/multi threading
- Thread t1(fun,0) 
- Thread t2(fun,0)

Main thread will be terminated after termination of thread t1 and thread t2 implemented as 
- t1.join() 
- t2.join();


### ***Why we choose multilple threads ?***
Multi threading is the ability of a process to manage its use by more than one user at a time and to manage multiple requests by the same user without having to have multiple copies of the program.
 1. Easy to handle.

 2. Lightweight process.

 3. Enhances efficiency in the context of communication.

 4. Share resources like code section and data section.


