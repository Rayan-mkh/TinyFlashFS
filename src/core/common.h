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

#define FLASHs 8192
#define BLOCKs 64
#define BLOCKnum (FLASHs / BLOCKs)

typedef enum {
    Free,
    Partial,
    Used
} BlockStatus;

typedef struct {
    BlockStatus status;
    char data[64];
} Block;

extern Block FAT[BLOCKnum];
extern int TotusedB;

void blocks(void);

#endif