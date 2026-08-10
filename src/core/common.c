#include<stdio.h>
#include<stdlib.h>


typedef struct {
    char *Fname;
    char *Ftype;
    int   Fsize;
    int   Fid;
    char Fdate[50];
    int   Fblock;
} FileInfo;

