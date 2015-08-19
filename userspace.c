#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

#include "platform.h"

int main(int argc,char *argv[])
{

   char *buf = "suresh";
   int  fd, rc, count=8;
  	
   fd = open("/dev/suresh", O_RDONLY | O_WRONLY, 0);
   if (fd == -1)
	printf("unable to open the file\n");
   rc = ioctl (fd, READ_IOCTL, buf);
   if(rc == -1)
	printf("Error in ioctl\n");
 
    close (fd); 
   return 0;
}
