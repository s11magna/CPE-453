#include "lwp.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include "scheduler.h" 

#define lnext lib_one
#define lprev lib_two 

thread h = NULL; 
tid_t counter = 1; 

static struct scheduler rr_publish = {NULL,NULL,rr_admit,rr_remove,rr_next};
scheduler rr = &rr_publish;
/*to save original context when lwp_start is called*/ 
rfile oriState; 
/*nextThread is currently running thread*/
thread temp,nextThread; 

tid_t lwp_create(lwpfun function, void* args, size_t stackSize) {
   temp = h;
   unsigned long* newSP; 
   unsigned long* newBP;  

   thread new = malloc(sizeof(context)); 
  
   if(new==NULL) { /*thread cannot be created*/ 
      return -1; 
   } 

   if(h==NULL) {
      h=new; 
      new->lprev = NULL; 
      new->lnext = NULL; 
   } 
   else {
      while(temp->lnext!= NULL) {
         temp = temp->lnext;
      } 
      new->lnext = NULL; 
      temp->lnext = new; 
      new->lprev = temp; 
   }

   new->tid = counter++;  
   new->stack = malloc(stackSize * sizeof(unsigned long));  
   new->stacksize = stackSize; 
   
   newSP = (new->stack + stackSize);
   *(--newSP) = (unsigned long) lwp_exit;
   *(--newSP) = (unsigned long) function;
   newBP = --newSP;
   new->state.rdi = (unsigned long)args; 
   new->state.rbp = (unsigned long)newBP; 
   new->state.rsp = (unsigned long)newSP; 

   rr->admit(new); 
   return new->tid; 
}     

/* terminates the current lwp
 * removes the thread from the thread linked lsit
 * removes the thread from the scheduler too 
 * frees the thread's stack and then the thread 
 * calls the next thread in the scheduler 
 * should call lwp stop of there are no more threads 
 */ 
void lwp_exit(void) {
   removeThread(nextThread); 
   rr->remove(nextThread); 
   // deleteStack(nextThread);
   //free(nextThread->stack); 
   //free(nextThread); 
   nextThread = rr->next();
 
   if(nextThread == NULL) {
      lwp_stop(); 
   } 
   else {
      load_context(&(nextThread->state)); 
   } 

}

/*function to delete thread's stack from another stack*/ 
void deleteStack(thread nextThread) {
   free(nextThread->stack); 
   free(nextThread); 
} 

void removeThread(thread victim) {
   thread temp; 
   if(victim == h && victim->lprev == NULL && victim->lnext == NULL) { 
      h = NULL; 
   } 
   else if(victim->lprev == NULL && victim->lnext != NULL ) {
      h = h ->lnext; 
      h->lprev = NULL; 
   } 
   else {
      temp = victim->lprev; 
      temp->lnext = victim->lnext;  
      if(victim->lnext != NULL) {
         victim->lnext->lprev = temp; 
      } 
   } 
} 

/* starts the lwp system
 * saves the original context 
 * picks a new thread 
 * if none, then load original context 
 */
void lwp_start() {
   nextThread = rr->next(); 
   /*check if the thread exists, else run original context*/ 
   if(nextThread != NULL) {
      	swap_rfiles(&(oriState),&(nextThread->state));
   } 
   else { // save and load the orginal context????
	swap_rfiles(&(oriState),&(oriState));
   } 
} 

/*save the context of the current thread*/ 
void lwp_stop() {
   if(nextThread!=NULL) {
     swap_rfiles(&(nextThread->state),&(oriState));
   }
   else{
  /*restore the original context*/ 
	load_context(&(oriState));
  }
} 

tid_t lwp_gettid() {
   if(nextThread != NULL) {
      return (nextThread->tid); 
   } 
   else {
      return NO_THREAD; 
   }
} 

scheduler lwp_get_scheduler() {
   return rr; 
} 


void lwp_yield() { 
   thread temp = nextThread;
   nextThread = rr->next();
   if(nextThread != NULL){
	swap_rfiles(&(temp->state),&(nextThread->state));
   }  
  else{
	swap_rfiles(&(nextThread->state),&(oriState));
  }  
} 

void lwp_set_scheduler(scheduler fun) {
   thread t = h;
   
   while (t != NULL) {
      fun->admit(t);
      rr->admit(t);
    t = t->lnext;
   }

   rr = fun;
} 

thread tid2thread(tid_t tid) { 
	thread temp =  h;
    while(temp != NULL){
	if(temp->tid == tid)
	{
	  return temp;
	}
    }
     return NULL;
} 




