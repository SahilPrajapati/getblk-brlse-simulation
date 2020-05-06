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
