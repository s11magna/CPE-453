#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/ioc_secret.h> 
#include <minix/ds.h> 
#include <minix/drivers.h> 
#include <minix/driver.h>
#include "fcntl.h"
#include "string.h"

int main(int argc, char* argv[]) {
    int res, uid, fd; 
    char message[] = "this is a message\n"; 
    fd = open ("/dev/Secret", O_WRONLY); 
    printf("Opening ... fd=%d\n", fd); 
    res = write(fd, message, strlen(message));
    printf("Writing... res=%d\n", res); 
    /*try grant*/ 
    if(argc > 1 && 0 != (uid = atoi(argv[1]))) {
       if(res = ioctl(fd,SSGRANT, &uid)) {
           perror("ioctl"); 
       } 
       printf("Trying to change owner to %d ... res = %d\n", uid, res); 
    } 
    res = close(fd);
}   
