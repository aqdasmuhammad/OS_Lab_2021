#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include "task.h"

#include "list.h"

#include "schedulers.h"

#include "cpu.h"

struct node * head = NULL;
struct node * current = NULL;
struct node * new = NULL;
struct node * final = NULL;
int num = 0;

void add(char * name, int priority, int burst) {
	
  if (head == NULL) {
    head = malloc(sizeof(struct node));

    // set the name of the task 
    head -> task = malloc(sizeof(struct task));
    head -> task -> name = name;
    head -> task -> burst = burst;
    head -> task -> priority = priority;
    // set the next node to be null
    head -> next = NULL;

    current = head;
  
  } else {

    new = malloc(sizeof(struct node));

    new -> task = malloc(sizeof(struct task));
    new -> task -> burst = burst;
    new -> task -> name = name;
    new -> task -> priority = priority;
    // if current->next is NULL
    if (!(current -> next)) {
      if (((new -> task -> burst) > (current -> task -> burst)) || ((new -> task -> burst) ==(current -> task -> burst))) {
        current -> next = new; // head points to second node 
        new -> next = NULL;
		
      }
	// if the second node burst is smaller than the current burst  
	  else {
		 
	  // set new to point to head which is in the second position
	  new->next = current; 	
	  // head now holds the address of new which is in the first position
	  head = new;
	  // reset current to new
      current = new;	  
	  // we still have the second node connected to null  
      }	   
    }
 
	// T3 and on execute from here I think 
		else {
    	  
	  while(1) {
    
	 	  
      if ((new -> task -> burst < current -> next -> task -> burst) ) {
	  
	  if (new->task->burst > current->task->burst) {
	  new->next = current->next;
	  current->next = new ;
	  current = head; // do  i need this
	  break;
	  }
	  else if (new->task->burst < current->task->burst) {

	  head = new;
      new->next = current;
	  current = head;
	  break;
	   

	  
	  }	  
	   
      }
	  
	  else if (new -> task -> burst == current -> next -> task -> burst) {
	   current = current -> next ;
	   
	   if (current->next == NULL) {
	  
	    
       new->next = NULL;
	   
	   }
	   
	   
	   else if (new-> task-> burst == current->next->task->burst) {
		   
		current = current -> next ;   
		new->next = current->next;   
		   
	   }   
	   
	   else {
		   
	   new->next = current->next;	   
		   
	   }   
	   
	    current->next = new;
	  	   
	   current = head;		    
		break;  
	  }  
	  
	  
	  
	  // if the new burst is greater than the current burst 
	  else if ((new -> task -> burst) > (current -> next -> task -> burst)) {
	  current = current -> next;
	  
	  if ( current->next == NULL) {
	//printf("testing"); 
	  current->next = new;  
	  new->next = NULL;
	  current = head;
	  break;

	  
	  }	  
	  
	   
      }	  
	   	  
		  
		  
		  
	  }



	  
    }
  }
 
}

// invoke the scheduler
void schedule() {
 float turnaroundtime = 0;
 float ResponseTime = 0;
 float WaitTime = 0;
 int burst = 0;
  struct node * ref = head;
  while (ref != NULL) {
	 num = num + 1;
    run(ref -> task, ref -> task -> burst);
   burst = burst + ref->task->burst;
	turnaroundtime = turnaroundtime + burst ; //5 (5+10+5)20  50      (5 + 5+10 + 5+10+15)
	if (ref->next !=NULL) {
	ResponseTime = ResponseTime + burst;
	}
	ref = ref -> next;

}
WaitTime = turnaroundtime - burst;

printf("The average turnaround time is : %f time units \n" , (turnaroundtime)/num);    
printf("The average ResponseTime is : %f time units \n" , (ResponseTime)/num);
printf("The average WaitTime is : %f time units\n " , (WaitTime)/num);
 
  

}