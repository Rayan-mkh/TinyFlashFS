#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../core/common.h"

char name[15] = "Rayan123456789";
Block FAT[BLOCKnum];

int main() {
    memcpy(FAT[1].data, name, sizeof(name));

    printf("Total blocks: %d\n\n", BLOCKnum);

    for (int i = 0; i < BLOCKnum - 125; i++) {
        if (strlen(FAT[i].data) == 64) {
            FAT[i].status = Used;
        } else if (strlen(FAT[i].data) == 0) {
            FAT[i].status = Free;
        } else {
            FAT[i].status = Partial;
        }

        if (FAT[i].status == Partial) {
            printf("Block %d is partially used ...\n", i + 1);
            printf("Content inside: \"%s\"\n", FAT[i].data);
            printf("the size of this block's data is %zu bytes\n", sizeof(FAT[i].data) - strlen(FAT[i].data));
            printf("*****************************************************\n");
        }
    }

    return 0;
}