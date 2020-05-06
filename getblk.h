#ifndef GETBLK_H
#define GETBLK_H
#include "dll.h"
#include "wait.h"
#include<iostream>
#include<unistd.h>
#include<thread>
#include<mutex>
using namespace std;

DLL HQ[4],FL;              //  HQ[] & FL of type Doubly Linked List fb5om dll.h
node *temp=new node;
mutex mtx;                  //mutex uses the lock-based technique 
int r_hqnum;                //to store hashqueue number for scenario 5
int b5;                      //to stob5e block numbeb5 fob5 scenario 5 
int r_w5=0;                   //to store read or write for scenario5

void wait_for_specific();
void signal_for_specific();
void wait_for_any();
void signal_for_any();
void threadhandler(int);
void getBlock(int blockNum,int HQnum,int ch1);

// update busy or free status
void update_hashqueues(int blkno,int HQnum)
{ 
  temp=HQ[HQnum].getBuffer(blkno);
  temp->statusf_b=0;
} 

//frees a block from dll
void brelse(int blkno,int HQnum)
{  
  sleep(3);
  temp=HQ[HQnum].getBuffer(blkno);	
  if(temp->status2==2)
  {
    if(!FL.isPresent(blkno))            //old is deleted and new added 
	FL.insert_at_start(blkno,0,2);  
  }
  else
  {  
    if(!FL.isPresent(blkno)) 
	FL.insert_at_end(blkno,0,1);         //free and vaild
  }
  
  update_hashqueues(blkno,HQnum);        //if not call this fn then hash queue buffer status will remains busy always
}

//changed status to DW 
void handle_write(int HQnum,int BlockNum)
{ 
  
  temp=HQ[HQnum].getBuffer(BlockNum);
  temp->status2=2;                   //making status delayed write
  sleep(5); 
  brelse(BlockNum,HQnum);             //free this now
}

// Asynchronous write to disk
void handle_async_write(int block_0)
{
 cout<<"\nAsynchronously writing buffer to disk for : "<<std::this_thread::get_id()<<endl;
 sleep(2);
 cout<<"\n\nAsynchronous Writing completed for : "<<std::this_thread::get_id()<<endl;

}

//starting point from where threads execution begins
void threadhandler(int block_0)
{ 
  int blockNum,HQnum,r_w;
  mtx.lock();                         
  blockNum=(rand()%10)+1;             // generate random block no. blockNum b/w (1-10)
  cout<<"\nThread : "<<std::this_thread::get_id();  
  cout<<" Needs Block number :";
  cout<<blockNum<<endl;
  mtx.unlock();                         
  mtx.lock();
  r_w=(rand()%2)+1;
  r_w==2?cout<<"Request is for Read":cout<<"Request is for Write";
  cout<<" by Thread: "<<std::this_thread::get_id()<<endl;    
  HQnum=blockNum%4;                                //Hash Function deciding which hq to refer
  cout<<endl;
  mtx.unlock();    
  getBlock(blockNum,HQnum,r_w);
}

//locks a buffer for given blocknumber
void getBlock(int blockNum,int HQnum,int r_w)
{ 
  
  int Insert_at_start,PrevTemp; 
  signal_for_any();                      //to handle  any buffer case if existing
  signal_for_specific();                 //to handle  specific buffer case if existing
 	
  Insert_at_start=0;
  lb:
      switch(r_w)
      {
    
       case 1:                       //To handle Write Operation
	 
      	   if(HQ[HQnum].isPresent(blockNum))                   //blockNum Present in hq 
      	   { 
     	      temp=HQ[HQnum].getBuffer(blockNum);
	          if(temp->statusf_b==1)                      //scenario 5(status is 'busy')
               {       
	              cout<<"\n--SCENARIO 5 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		          sleep(2);	        
		          b5=blockNum;
	              r_w5=1;	
		          r_hqnum=HQnum;	
	              wait_for_specific();     	       
	           }
	          else                        //scenario 1(status is 'free')
	           {		
	              temp->statusf_b=1;           //change status to 'busy' and locks it    
	              mtx.lock();	       
	              if(FL.isPresent(blockNum)) 
	 	              FL.delete_by_value(blockNum);                //status is changed to bsy		             
	              cout<<"\n--SCENARIO 1 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;	
	              sleep(2);
	   	          cout<<"\nbuffer is locked for : "<<std::this_thread::get_id()<<endl;
		          mtx.unlock();	        
		          handle_write(HQnum,blockNum);	     		
	           }	
	       }
          else
          {
 	           sleep(1);
               if(FL.isEmpty())                                  //scenario 4(no buffers left in FL)
	           {   
                 cout<<"\n--SCENARIO 4 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		         sleep(2);	 	
		         wait_for_any();		
	           }
	           temp=FL.delete_from_begin();
               if(temp->status2==2)                       //scenario 3(status is 'delayed write')
	           {                    
	             cout<<"\n--SCENARIO 3 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
 		         sleep(2);
		         thread async_thread(handle_async_write,0);
		         async_thread.join();
                 Insert_at_start=1;
		         PrevTemp=temp->data;
	             goto lb;
	           } 
	          else                //scenario 2(Pick a free buffer from hq and insert at end of required hq) 
	          { 
		         mtx.lock(); 
     	         cout<<"\n--SCENARIO 2 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		         if(HQ[HQnum].isPresent(blockNum)) 
				    goto l12;
		       	HQ[HQnum].insert_at_end(blockNum,1,2);
		        mtx.unlock();
 			    l12:
		        cout<<"\nBuffer is locked for : "<<std::this_thread::get_id()<<endl;		  
		        handle_write(HQnum,blockNum);
         	  }
           } 
           if(Insert_at_start==1)         //if status is delayed write then insertion at beginning
	       {  
             mtx.lock();
             FL.insert_at_start(PrevTemp,0,1);
	         HQ[PrevTemp%4].change_status2(PrevTemp,1); 
	         mtx.unlock();	
	        }	
	      break;
	   case 2:                               // To handle Read Operation
	       if(HQ[HQnum].isPresent(blockNum))                //blockNum Present in hq 
       	    {	
				sleep(1);
           	    temp=HQ[HQnum].getBuffer(blockNum);
	     	    if(temp->statusf_b==1)                      //scenario 5(status is 'busy')
	     	   {       
		         cout<<"\n--SCENARIO 5 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		         sleep(2);		
		         b5=blockNum;
		         r_hqnum=HQnum;			
		         wait_for_specific();		        	  
 		       }
	     	   else                          //scenario 1(status is 'free')
	    	  {		
		         temp->statusf_b=1;              //change status to 'busy' and locks it    
		         mtx.lock();	 
		         if(FL.isPresent(blockNum)) 
		             FL.delete_by_value(blockNum);           //status is changed to bsy			
		         mtx.unlock();		    
		         cout<<"\n--SCENARIO 1 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		         sleep(2);		  
		         cout<<"\nbuffer is locked for : "<<std::this_thread::get_id()<<endl;
		         temp->status2=1;                           //valid status
		         brelse(blockNum,HQnum);     		
		       }	
	        }
         else
        {	 
			  sleep(1);
              if(FL.isEmpty())                           //scenario 4(no buffers left in FL)
		      {	   
		        cout<<"\n--SCENARIO 4 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		        sleep(2);		
		        wait_for_any();		
		      }
		     temp=FL.delete_from_begin();
        	 if(temp->status2==2)                         //scenario 3(status is 'delayed write')
		     {                    
		       cout<<"\n--SCENARIO 3 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		       sleep(2);
		       thread async_thread(handle_async_write,0);
		       async_thread.join();
		       Insert_at_start=1;
	           PrevTemp=temp->data;
               goto lb;
		     } 
		    else                  //scenario 2(Pick a free buffer from hq and insert at end of required hq) 
		    {  
     	       cout<<"\n--SCENARIO 2 IS APPLYING-- for : "<<std::this_thread::get_id()<<endl;
		       sleep(2);
		       mtx.lock();        	
		       if(HQ[HQnum].isPresent(blockNum)) 
			       goto l11;
		   	   HQ[HQnum].insert_at_end(blockNum,1,1);
		       mtx.unlock(); 
		       l11:
		       cout<<"\nBuffer is locked for: "<<std::this_thread::get_id()<<endl;
		       temp->status2=1;              //valid
		       brelse(blockNum,HQnum); 	 	
		     }		   
        }
	    if(Insert_at_start==1)
	    {
		  FL.insert_at_start(PrevTemp,0,1);
		  HQ[PrevTemp%4].change_status2(PrevTemp,1);
	    }	
	    break;
 	}

    mtx.lock();
	cout<<"\n****************Hash Queues now :****************\n";
    for(int i=0;i<4;i++)
    {  
	  sleep(0.5); 
	  cout<<"hq "<<i<<" :  ";
	  HQ[i].display();
	  cout<<"\n";
  	}
    cout<<"\n*****************Free List now  :****************\n";
    FL.display();
    cout<<"\n*************************************************\n";
    mtx.unlock();
    sleep(3);	
           		  
	cout<<"\n\nThread :"<<std::this_thread::get_id()<<" needs another buffer \n";
	threadhandler(0);    
}
#endif

