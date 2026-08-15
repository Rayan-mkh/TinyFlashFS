#include "../core/common.h"
#include<stdio.h>
#include<string.h>

Block FAT[BLOCKnum];
int TotusedB = 0;


void blocks(void) {
    for (int i = 0; i < BLOCKnum; i++) {
        if (FAT[i].status == Used) {
            printf("Block %d is used\n", i + 1);
           
        } else if (FAT[i].status == Free) {
            printf("Block %d is free\n", i + 1);
        } else {
            printf("Block %d is partially used\n", i + 1);
           printf("the size of this block is %zu \n", sizeof(FAT[i].data) - strlen(FAT[i].data));
        }
        printf("*****************************************************\n");
    }
}