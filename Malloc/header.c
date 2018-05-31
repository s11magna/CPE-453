#include "header.h"
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


// Linked List Data structure

// if we first initalize this memeory on our first call to sbrk we need to add a header
HEADER *initHeader(HEADER *head)
{
    void *check = sbrk(0);           // where the orginal break is
    void *request = sbrk(SBRK_SIZE); // Move the break to the new start
    if (request == (void *)-1)
    {            // checkut its not NULL
        exit(-1); // sbrk failed.
    }
    else
    {
        assert(check == request); // Not thread safe.
        return check;
    }
    head = (HEADER *) request;
    head->size = SBRK_SIZE - BLOCK_SIZE;
    assert(head->size == 0);
    head->next = NULL;
    head->free = 1;
return head;

}
