#ifndef HEADER_H
#define HEADER_H
#define SBRK_SIZE 64000 // 64K size allocation
#define BLOCK_SIZE sizeof(HEADER);
#include <stddef.h>

typedef struct HEADER{
    size_t size;
    struct HEADER *next;
    int free;
}HEADER;
/************************************************************************/
// Function for the first time that malloc is called
HEADER *initHeader(HEADER *head);


// define a function if we need to split a block




#endif