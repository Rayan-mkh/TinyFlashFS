#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include"./fat.h"
#include"../core/common.h"  

void PrintMetadataRow(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) return;

    char name[50] = "", type[10] = "", date[50] = "";
    int id = 0, bytes = 0, blocks = 0;

    char *line = NULL;
    size_t cap = 0;
    ssize_t len;

    while ((len = getline(&line, &cap, fp)) != -1) {
        sscanf(line, " \"name\": \"%49[^\"]\"", name);
        sscanf(line, " \"type\": \"%9[^\"]\"", type);
        sscanf(line, " \"id\": %d", &id);
        sscanf(line, " \"bytes\": %d", &bytes);
        sscanf(line, " \"blocks\": %d", &blocks);
        sscanf(line, " \"date\": \"%49[^\"]\"", date);
    }

    free(line);
    fclose(fp);

    printf("%-15s %-6s %-8d %-8d %-8d %s\n", name, type, id, bytes, blocks, date);
}

void ListAllFiles(void) {
    DIR *dir = opendir("storage");
    if (dir == NULL) {
        perror("Error opening storage directory");
        return;
    }

    printf("%-15s %-6s %-8s %-8s %-8s %s\n", "NAME", "TYPE", "ID", "BYTES", "BLOCKS", "DATE");
    printf("--------------------------------------------------------------------\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
       
        if (strstr(entry->d_name, ".json") == NULL) continue;

        char fullpath[300];   
        snprintf(fullpath, sizeof(fullpath), "storage/%s", entry->d_name);
        PrintMetadataRow(fullpath);
    }

    closedir(dir);
}

void DeleteFile(const char *name) {
    char path[128];
    snprintf(path, sizeof(path), "storage/%s.json", name);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("No such file: '%s'\n", name);
        return;
    }

    
    int blockIndices[128];
    int blockCount = 0;

    char *line = NULL;
    size_t cap = 0;
    ssize_t len;

    while ((len = getline(&line, &cap, fp)) != -1) {
        char *bracket = strstr(line, "\"blockIndices\": [");
        if (bracket != NULL) {
            char *cursor = bracket + strlen("\"blockIndices\": [");
            int idx;
            while (sscanf(cursor, "%d", &idx) == 1) {
                blockIndices[blockCount++] = idx;
                cursor = strchr(cursor, ',');
                if (cursor == NULL) break;
                cursor++;   
            }
        }
    }

    free(line);
    fclose(fp);

    
    for (int i = 0; i < blockCount; i++) {
        int idx = blockIndices[i];
        FAT[idx].status = Free;
           memset(FAT[idx].data, 0, sizeof(FAT[idx].data));
            TotusedB--; 
    }

    
    if (remove(path) == 0) {
        printf("Deleted '%s' and freed %d block(s).\n", name, blockCount);
    } else {
        perror("Error deleting metadata file");
    }
}

void DIRs(void){
      int choice;
    printf("1. Look up one file\n");
    printf("2. List all files\n");
    printf("3. Delete a file\n");
    printf("4. Blocks Status\n");
    printf("Choice: ");
     scanf("%d", &choice);

    if (choice == 1) {
        char name[50];
        printf("Enter the file name to look up: ");
        scanf("%49s", name);

        char path[128];
        snprintf(path, sizeof(path), "storage/%s.json", name);

        FILE *fp = fopen(path, "r");
      if (fp == NULL) {
    perror("Error opening metadata file");
    return;
}

        char *line = NULL;
        size_t cap = 0;
        ssize_t len;
        while ((len = getline(&line, &cap, fp)) != -1) {
            fputs(line, stdout);
        }

        free(line);
        fclose(fp);

    } else if (choice == 2) {
        ListAllFiles();

    } else if (choice == 3) {
    char name[50];
    printf("Enter the file name to delete: ");
    scanf("%49s", name);
    DeleteFile(name);
}else if(choice == 4){
    blocks();
}
    
    
}

