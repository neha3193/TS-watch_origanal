

#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define HIGH_LEVEL
#undef  HIGH_LEVEL
//#define man
#undef  man
#ifndef man
#define var extern
#else
#define var
#endif
typedef int filedescriptor;
filedescriptor fd=0;
//var char dirbuf[50];
//var char sudirbuf[50];
void auto_subfile(char *dir, char *subdir, char *file , char *string)
{
    
#ifdef HIGH_LEVEL
    char *ptr;
    FILE *fp = 0;
    mkdir(dir,0700);
    ptr = strcat(dir,subdir);
    mkdir(ptr,0700);
    ptr = strcat(dir,file);
    fp=fopen(ptr,"a+");
    fwrite(string,strlen(string),1,fp);
    fflush(fp);
    fclose(fp);
#else
    char *ptr;
    mkdir(dir,0700);
    ptr = strcat(dir,subdir);
    mkdir(ptr,0700);
    ptr = strcat(dir,file);
    //printf("ptr:%s\n",ptr);
    fd = open(ptr,O_CREAT|O_APPEND|O_RDWR,00600 ); 
    //printf("fd:%u\n",fd);
    if(fd<0)
    {
        perror("open");
       
    }
    if(string)
    if(write(fd,string,strlen(string))<0)
    {
        perror("write");
    }
    close(fd);
#endif
}

