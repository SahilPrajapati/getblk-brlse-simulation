#ifndef WAIT_H
#define WAIT_H
#include "getblk.h"
#include "dll.h"
#include<iostream>
#include<condition_variable>
#include<thread>
#include<chrono>
#include<stdlib.h> 

std::condition_variable cv;
std::mutex cv_m; /* This mutex is used for three purposes:
                  1) to synchronize accesses to i and k
                  2) to synchronize accesses to std::cerr
                  3) for the condition variable cv */
int freeBuffer = 0;       //1 if free list contains a buffer
bool done=true;
int specificBuffer=0;     //1 if specific buffer becomes free 

void wait_for_any()       //any buffer to become free
{
  std::unique_lock<std::mutex> lk(cv_m);
  std::cerr << "Waiting... \n";
  cv.wait(lk, []{return freeBuffer == 1  ;});     //wait till freeBuffer is set to 1
  std::cerr << "...finished waiting..free buffer available in free list\n";
  done=false;
  threadhandler(0); 
}
void signal_for_any()
{
  if(done) 
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
      std::lock_guard<std::mutex> lk(cv_m);
      if(!FL.isEmpty())            //if freelist contains any  buffer  
     	freeBuffer = 1;
    }
    cv.notify_all();
  }
} 


void wait_for_specific()          //specific buffer
{
  std::unique_lock<std::mutex> lk(cv_m);
  std::cerr << "Waiting... \n";
  cv.wait(lk, []{return  specificBuffer==1 ;});    //checks that specificBuffer=1 or not
  std::cerr << "...finished waiting..(specific buffer available in free list)\n";  
  done=false;   
  getBlock(b5,r_hqnum,r_w5);          //To lock specific buffer that  is available in freelist 
}
void signal_for_specific()
{
  if(done) 
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
      std::lock_guard<std::mutex> lk(cv_m);
      if(FL.isPresent(b5))            //freelist contains specific  buffer
	     specificBuffer=1;
    }
    cv.notify_all();           //wakeup all sleeping threads
  }
} 
#endif
