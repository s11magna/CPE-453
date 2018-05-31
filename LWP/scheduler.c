#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include "lwp.h"
#include "fp.h" 
#include "scheduler.h" 

thread head = NULL; /*global pointer to head of doubly linked list of threads*/
/*Add tne passed context to the scheduler's next pool
 * sched1 represents a thread's next thread in list 
 * sched2 represents a thread's previous thread in list 
 */ 
void rr_admit(thread new) {
   thread temp = head; 
   if(head==NULL) { /*if first thread added to list*/
      head = new; 
      head->sched_one = NULL; 
      head->sched_two = NULL; 
   } 
   else { /*if there is already a thread in the list*/
      while(temp->sched_one!=NULL) { /*iterate to last element of list*/ 
         temp = temp->sched_one; 
      } 
      temp->sched_one = new; /*set last threads "next" equal to new thread*/ 
      new->sched_two = temp; /*set "previous" and "next" fields of new thread*/ 
      new->sched_one = NULL; 
   }     
} 

/*remove the passed context from the scheduler's scheduling pool*/ 
void rr_remove(thread victim) {
   /* case where this is just the head*/
   if((victim == head) && (victim->sched_two == NULL) 
      && (victim->sched_one == NULL))
    {
	head = NULL;
    } 
   /* case where this is the start*/
   else if((victim->sched_two == NULL) && (victim->sched_one != NULL))
    {
	head = head->sched_one;
	head->sched_two = NULL;	
    }
   else{
       thread temp = victim->sched_two;
       temp->sched_one = victim->sched_one;
       if(victim->sched_one != NULL){
	 victim->sched_one->sched_two = temp;
	}		
   }
}  

/* return the next thread to be run or NULL if there isn't one
 * round robin: next thread = first element in list 
 * place thread at end of list to be used again 
 */ 
thread rr_next() {
   thread temp = head; 
   thread toInsert = temp; 
   if(head==NULL) {
      return NULL; 
   } 
   else {
	if(head->sched_one == NULL){
		return temp;
	}
	head = temp->sched_one; /*set head of list to new thread*/
      while(temp->sched_one!=NULL) { 
         temp = temp->sched_one; 
      }
      temp->sched_one = toInsert; /*insert previous head to end of list*/  
      toInsert->sched_two = temp; 
      toInsert->sched_one = NULL; 
   }      
   return toInsert; 
} 
