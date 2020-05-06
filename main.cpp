#include "wait.h"
#include "dll.h"
#include "getblk.h"
#include<iostream>
#include<unistd.h>//sleep
#include<thread>
#include<mutex>
#include<signal.h>
#include<stdlib.h>//rand()
using namespace std;
int main()
{      
  srand(time(NULL));
  
  for(int i=0;i<4;i++)
  {
    FL.insert_at_start(0,0,0);                  //FL here is object of class DLL defined in file dll.h , calling insert_at_end fn of DLL with arguements as data , status , status2
  }	
  cout<<"\n--------------------Initially Hash Queues :--------------\n";
  for(int i=0;i<4;i++)
  {       
    cout<<"hq "<<i<<" :  ";
    HQ[i].display();                           //HQ[] is also object of class DLL  , display() is used to show 4 initial hash queues..
    cout<<"\n";
  }
  cout<<"\n--------------------Initially Free List :-----------------\n";
  FL.display();                               // calling display() function of class DLL
  cout<<"\n----------------------------------------------------------\n";
  thread t1(threadhandler,0);                //creating Thread t1 .. threadhandler is a function defined in file getblk.h
  sleep(2);
  thread t2(threadhandler,0);               //creating another thread t2..
  t1.join();  
  t2.join();		                        // main will wait for these threads before quitting itself.
  return 0;
}
