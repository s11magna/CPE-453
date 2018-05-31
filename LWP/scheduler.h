#ifndef SCHEDULERH
#define SCHEDULERH 
#include "lwp.h" 
#include "fp.h" 
void rr_admit(thread new); 
void rr_remove(thread victim); 
thread rr_next(); 

#endif 
