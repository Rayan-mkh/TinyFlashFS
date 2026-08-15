#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../core/common.h"
#include"./dir.h"
#include"./fat.h"
#include"./file.h"


void FashBare(void) {
    double total_storage = FLASHs;
    double used_storage = TotusedB;
    int bar_width = 20;

    double current_load = 0.0;
    double step = used_storage / 50.0;
    if (step == 0) step = 1;  

    while (current_load <= used_storage) {
        int percentage = (int)((current_load / total_storage) * 100);
        int filled_width = (percentage * bar_width) / 100;

        printf("\rStorage: [");
        for (int i = 0; i < bar_width; ++i) {
            printf(i < filled_width ? "#" : " ");
        }
        printf("] %d%% (%.1f Bytes / %.1f Bytes)", percentage, current_load, total_storage);
        fflush(stdout);

        current_load += step;
        usleep(40000);
    }

   
    int percentage = (int)((used_storage / total_storage) * 100);
    int filled_width = (percentage * bar_width) / 100;
    printf("\rStorage: [");
    for (int i = 0; i < bar_width; ++i) printf(i < filled_width ? "#" : " ");
    printf("] %d%% (%.1f Bytes / %.1f Bytes)", percentage, used_storage, total_storage);

    printf("\nStorage Scan Complete!\n");
}

int main(){
    LoadFAT();   

    char input[10];

    while (1) {
        printf("\n///// THE STORAGE //////\n");
        printf("/////////////////////////////////////////////////////");
        FashBare();
        printf("\n%% THE FILE SYSTEM %%\n");
        printf("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤\n");
        printf("** What do you want USER\n");
        printf("   1 = GO to DIRECTORY, 2 = CREATE a FILE, f = EXIT\n");
        printf("Choice: ");
        scanf("%9s", input);

        if (input[0] == 'f' || input[0] == 'F') {
            break;
        } else if (input[0] == '1') {
            DIRs();
        } else if (input[0] == '2') {
            FileInfo file = CreateF();
            struct Metadata meta = DOfile(file);

            printf("\n--- File Info ---\n");
            printf("Formatted Date: %s\n", file.Fdate);
            printf("Name : %s\n", file.Fname);
            printf("Type : %s\n", file.Ftype);
            printf("ID   : %d\n", file.Fid);

            ReadFromBlocks(meta);
            printf("****************************************\n");
            blocks();

            free(file.Fname);
            free(file.Ftype);
        } else {
            printf("Invalid choice.\n");
        }
    }

    SaveFAT();  
    return 0;
}