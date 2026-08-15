#ifndef FILE_H
#define FILE_H

#include "../core/common.h"

struct Metadata {
    char name[50];
    char type[10];
    int  id;
    int  bytes;
    int  blocks;
    char date[50];
    int  blockIndices[128];
};

void SaveMetadataJSON(struct Metadata meta);
FileInfo CreateF(void);
struct Metadata DOfile(FileInfo file);
void ReadFromBlocks(struct Metadata meta);
void SaveFAT(void);
void LoadFAT(void);

#endif