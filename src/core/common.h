#ifndef COMMON_H
#define COMMON_H

typedef struct {
    char *Fname;
    char *Ftype;
    int   Fsize;
    int   Fid;
    char  Fdate[50];
    int   Fblock;
} FileInfo;

#endif