#include "header.h"
#include <stdio.h>
#include <assert.h>

// global header to keep hold of the LinkList
HEADER *head = NULL;   // intalize it to the start of the heap? only once?  dont think this is right
//extern int errno;
char printBuf[100];
void *malloc(size_t size)
{
   /* // check i header is NULL;
    if(head == NULL){  // if its equal to NULL then there must not be any memeory allocation 
       initHeader(head); // return initalize header
      
    }*/

}