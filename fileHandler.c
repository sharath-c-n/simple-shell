#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//file header
#include <fileHandler.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFFSIZE 32768

int simpleCat(char *src,char* dest,bool isAppend) {
	int n;
	char buf[BUFFSIZE];
    int fd=-1;
	  int fd2=-1;
   /**
    * This is the addition that was made to the simple-cat to handle filenames
    * passed as parameter.This opens file whose name is passed in the argument in the read only mode and prints it
    */

   fd=open(src,O_RDONLY);
   if(isAppend)
   {
		  //Opening the file in append mode if the isAppend flag is true
	   fd2=open(dest,O_RDWR|O_APPEND);
   }
	 else {
		  //Opening the file in write mode if the isAppend flag is false
		 if (access( dest, F_OK ) != -1) {
		  //If the file is already present removing it
		 	 remove(dest);
		 }
		  fd2=open(dest,O_RDWR|O_CREAT);
	 }

   fflush(stdout);
   if(-1 == fd || -1 == fd2){
      perror("Open Failed");
      return 1;
   }

	while ((n = read(fd, buf, BUFFSIZE)) > 0) {
		//Reading from one fd(src) and writing it to fd2(dest)
		if (write(fd2, buf, n) != n) {
			fprintf(stderr, "write error, %d\n",n);
			exit(1);
		}
	}

	if (n < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}
	else {
		fprintf(stderr, "Successfully written to file\n");
		close(fd);
		close(fd2);
	}
	return(0);
}
