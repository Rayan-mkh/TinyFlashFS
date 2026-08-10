#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../core/common.h"
#define DATE_LEN 50
#define BUFFER_SIZE 256

FileInfo CreateF(void) {
    FileInfo file;
    time_t rawtime;

    time(&rawtime);
    struct tm *localTime = localtime(&rawtime);

    file.Fname = malloc(50);
    file.Ftype = malloc(10);
    

    strftime(file.Fdate, DATE_LEN, "%d-%b-%Y | %T ", localTime);

    printf("Enter the name of the file: ");
    scanf("%49s", file.Fname);

    printf("Enter the Type");
    scanf("%9s", file.Ftype);

    printf("Give the ID \n");

    printf("ID [          ]\033[11D");

    scanf("%d", &file.Fid);
    

    return file;
}
void DOfile(FileInfo file) {
    char inputBuffer[BUFFER_SIZE];
    char fullname[64];  

    
    snprintf(fullname, sizeof(fullname), "%s.%s", file.Fname, file.Ftype);

    FILE *fp = fopen(fullname, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("--- Writing to '%s' ---\n", fullname);
    printf("Type whatever you want below. Press Enter after each line.\n");
    printf("Type 'EXIT' on a new line and press Enter to save and finish.\n\n");

    while (1) {
        printf("> ");
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            break;
        }

        if (strcasecmp(inputBuffer, "EXIT\n") == 0) {
            break;
        }

        fputs(inputBuffer, fp);
    }

    fclose(fp);
    printf("\nData saved successfully to '%s'.\n", fullname);
}
void ReadF(FileInfo file) {
    char fullname[64];
    char inputBuffer[BUFFER_SIZE];

    snprintf(fullname, sizeof(fullname), "%s.%s", file.Fname, file.Ftype);

    FILE *fp = fopen(fullname, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    printf("****Contents of '%s'0***\n", fullname);

    while (fgets(inputBuffer, sizeof(inputBuffer), fp) != NULL) {
        fputs(inputBuffer, stdout);
    }

    printf("¤¤¤ End of file ¤¤¤\n");

    fclose(fp);
}

int main(void) {
    FileInfo file = CreateF();

   

    DOfile(file);
    


    printf("\n--- File Info ---\n");
    printf("Formatted Date: %s\n", file.Fdate);
    printf("Name : %s\n", file.Fname);
    printf("Type : %s\n", file.Ftype);
    printf("ID   : %d\n", file.Fid);
    ReadF(file);
    free(file.Fname);
    free(file.Ftype);

    return 0;
}

