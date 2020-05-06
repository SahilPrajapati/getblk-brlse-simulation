#ifndef DLL_H
#define DLL_H
#include<iostream>
using namespace std;

//Node of the doubly linked list
struct node
{
  int statusf_b;   //0-free,1-busy
  int status2;//0-invalid,1-valid,2-delayed write
  int data;
  node *next; // point to the next node 
  node *prev; // point to the previous node
};


class DLL
{
	// To store starting(head) and ending(tail) of the DLL
  node *head,*tail;

  int count; // to track total no. of nodes
  
  public:

  	// initializing on constructor call
  DLL()
  {
    head=NULL;
    tail=NULL;
    count=0;        
  }

  //Functions declaration that will be required.
  void insert_at_end(int,int,int); 
  void insert_at_start(int,int,int);
  node* delete_from_begin();
  void delete_by_value(int);
  bool isPresent(int);
  bool isEmpty();
  node *getBuffer(int);
  void display();
  void change_status2(int,int);
};

//DLL HQ[4],FL;

// It will change the status
void DLL::change_status2(int blockNum,int newstatus)
{
/*Input:blockNum,newstatus
  Output:none 
  Description:changes status of given block number to newstatus 
*/
  node *t=head;
  while(t!=NULL)//scan entire dll
  {
  	//on getting the  required block
     if(t->data==blockNum)
     {
       t->status2=newstatus; // change the status
       return;
     }
     t=t->next;
  }
} 

//To insert a node at beginning
void DLL::insert_at_start(int data,int status,int status2) 
{
/*Input:data,status,status2
  Output:none 
  Description:insert at beggining of dll 
*/
  node *t=new node;
  t->data=data;
  t->statusf_b=status;
  t->status2=status2;
  t->next=NULL;
  t->prev=NULL;

  // if first node then make it head
  if(head==NULL||count==0)
  {
    head=t;
    tail=head;
  }
  else //else update the head
  {
    t->next=head;
    head->prev=t;
    head=t;
  }

  // increase the count of nodes.
  count++;
}

// To insert a node at end
void DLL::insert_at_end(int data,int status,int status2)
{	
/*Input:data,status,status2
  Output:none 
  Description:insert at end of dll 
*/
  node *t=new node;
  t->data=data;
  t->statusf_b=status;
  t->next=NULL;
  t->prev=NULL;
  t->status2=status2;

// if first node then make it head and tail both
  if(tail==NULL || count==0)
  {
    head=t;
    tail=head;
  }

  else // else update tail
  {
    tail->next=t;
    t->prev=tail;
    tail=t;
  }

  count++; // update count
}


// To delete the first node of DLL
node* DLL::delete_from_begin()
{ 
/*Input:none
  Output:pointer to buffer that is deleted from beginning 
  Description:A node is deleted from dll and pointer to it is returned 
*/

	// If no node is present
  if(count==0 ||head==NULL)
  {
    cout<<"\n empty list  ";
    return NULL;
  }

  node *temp=head;
  node *temp2;

  // if only one node is their
  if(head==tail)
  {
    temp2=head;
   //delete head;
   tail=NULL;
   head=NULL;	
  }
  else // else update the head after deleting first node
  { 
    temp2=head;
    node *t=head;
    head=t->next;
    head->prev=NULL;
    delete t;
  }

  count--; // update count

  // return deleted node
  return temp2;
}


// To delete the node by searching its value
void DLL::delete_by_value(int val)
{/*Input:val of type integer
  Output:none
  Description:delete a node from dll by its value 
*/

   // if no node is present
  if(head==NULL || count==0)
    cout<<"\n empty list "<<count;
  /*if(head==tail)
  { 
  //  delete head;
    tail=NULL;	
    head=NULL;
  }*/

   // if we have the first node which is to be deleted by value
  if(head->data==val)
  {
  	// and if only one node is present
    if(head->next==NULL)
    {
      delete head;
      return;
    }
    else // else update the head
    {
      node *t=head;
      head=t->next;
      head->prev=NULL;
      delete t;
    }
  }

  else // if node to be deleted is other than first
  {
    node *pred,*temp,*temp2;
    pred=head; // to store previous node to target
    temp=head->next; // to store node to be deleted

    // updating pred and temp until target is reached
    for(pred,temp;temp->data!=val;pred=pred->next,temp=temp->next);
    
    // if target node is tail then update tail after deleting target node
    if(temp->next==NULL)
    {
      pred->next=NULL;
      tail=pred;
      delete temp;
      return;
    }	
    else //else delete the node and update the DLL
    {
      pred->next=temp->next;
      temp->next->prev=pred;
      delete temp;
    }
  }
  display();

  //update the count
  count--; 
}

//To check whether the node is present or not
bool DLL::isPresent(int blockNum)
{/*Input:blockNum
  Output:true if blockNum is present otherwise false
  Description:block number is present in dll or not 
*/
  node *t=head;
  while(t!=NULL)
  {
  	// If found
    if(t->data==blockNum)
      return true;
    t=t->next;
  }

  //if not found
  return false;
}

//To search and return the buffer
node* DLL::getBuffer(int blockNum) 
{/*Input:blockNum
  Output:pointer to buffer containing given blockNum otherwise NULL
  Description: Searches for blockNum,if found then returns pointer to buffer
*/
  node *t=head;
  while(t!=NULL)
  {
  	// If found
    if(t->data==blockNum)
      return t;
  	//else update the temporary pointer(t)
    t=t->next;
  }
  return NULL;  //when buffer with given block number is not found
}

//To check whether doubly linked list is empty or not
bool DLL::isEmpty()
{/*Input:none
  Output:true if blockNum is present otherwise false
  Description:block number is present in dll or not 
*/

  if(count==0 ||head==NULL)
    return true;
  else 
    return false;
}


//To print the linked list.
void DLL::display()
{/*Input:none
  Output:display dll with data and status
  Description:display dll with status which indicates whether buffer is free/busy/valid/invalid/delayedwrite
*/ 
  node *t=head;
  if(t==NULL) 
    cout<<"empty list ";
  else
  {
    while(t->next!=NULL)
    {
      cout<<t->data<<"/"<<t->statusf_b<<"/"<<t->status2<<" <---> ";
      t=t->next;
    }
    cout<<t->data<<"/"<<t->statusf_b<<"/"<<t->status2;
  }
}
#endif
