#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "../core/common.h"
#include "./fat.h"
#define DATE_LEN 50

struct Metadata {
    char name[50];
    char type[10];
    int  id;
    int  bytes;
    int  blocks;
    char date[50];
    int  blockIndices[128];
};

void SaveMetadataJSON(struct Metadata meta) {
    char metaPath[128];
    snprintf(metaPath, sizeof(metaPath), "storage/%s.json", meta.name);

    FILE *fp = fopen(metaPath, "w");
    if (fp == NULL) {
        perror("Error opening metadata file");
        return;
    }

    fprintf(fp, "{\n");
    fprintf(fp, "  \"name\": \"%s\",\n", meta.name);
    fprintf(fp, "  \"type\": \"%s\",\n", meta.type);
    fprintf(fp, "  \"id\": %d,\n", meta.id);
    fprintf(fp, "  \"bytes\": %d,\n", meta.bytes);
    fprintf(fp, "  \"blocks\": %d,\n", meta.blocks);
    fprintf(fp, "  \"date\": \"%s\",\n", meta.date);

    fprintf(fp, "  \"blockIndices\": [");
    for (int i = 0; i < meta.blocks; i++) {
        fprintf(fp, "%d", meta.blockIndices[i]);
        if (i < meta.blocks - 1) fprintf(fp, ", ");
    }
    fprintf(fp, "]\n");

    fprintf(fp, "}\n");

    fclose(fp);
    printf("Metadata saved to '%s'.\n", metaPath);
}

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

    printf("Enter the Type ");
    scanf("%9s", file.Ftype);

    printf("Give the ID \n");
    printf("ID [          ]\033[11D");
    scanf("%d", &file.Fid);

    return file;
}


struct Metadata DOfile(FileInfo file) {
    char fullname[64];
    snprintf(fullname, sizeof(fullname), "%s.%s", file.Fname, file.Ftype);

    struct Metadata meta = {0};  
    printf("--- Writing '%s' to blocks ---\n", fullname);
    printf("Type whatever you want below. Press Enter after each line.\n");
    printf("Type 'EXIT' on a new line and press Enter to save and finish.\n\n");

    char *content = malloc(1);   
    content[0] = '\0';
    size_t contentLen = 0;

    char *line = NULL;
    size_t cap = 0;
    ssize_t totalBytes = 0;

    while (1) {
        printf(">> ");
        ssize_t len = getline(&line, &cap, stdin);
        if (len == -1) break;
        if (strcasecmp(line, "EXIT\n") == 0) break;

        totalBytes += len;

        content = realloc(content, contentLen + len + 1);
        memcpy(content + contentLen, line, len);
        contentLen += len;
        content[contentLen] = '\0';
    }

    printf("-------------------------------------------------------\n");
    printf("$$ You wrote %zd bytes total.\n", totalBytes);

    int neededBlocks = contentLen / 64;
    if (contentLen % 64 != 0) neededBlocks++;

    int blockIndices[128];  
    int fatIndex = 0;

    for (int b = 0; b < neededBlocks; b++) {
       
        while (fatIndex < BLOCKnum && FAT[fatIndex].status == Used) {
            fatIndex++;
        }
        if (fatIndex >= BLOCKnum) {
            printf("Error: not enough free blocks!\n");
            break;
        }

        int offset = b * 64;
        int chunkSize = (contentLen - offset < 64) ? (contentLen - offset) : 64;

       memcpy(FAT[fatIndex].data, content + offset, chunkSize);
        FAT[fatIndex].status = (chunkSize == 64) ? Used : Partial;
          TotusedB++;

        blockIndices[b] = fatIndex;
        fatIndex++;
    }

   
    strncpy(meta.name, file.Fname, sizeof(meta.name));
    strncpy(meta.type, file.Ftype, sizeof(meta.type));
    meta.id = file.Fid;
    meta.bytes = (int)contentLen;
    meta.blocks = neededBlocks;
    strncpy(meta.date, file.Fdate, sizeof(meta.date));
    memcpy(meta.blockIndices, blockIndices, neededBlocks * sizeof(int));

    SaveMetadataJSON(meta);

    free(content);
    free(line);
    printf("\nContent stored successfully in %d block(s) for '%s'.\n", meta.blocks, fullname);

    return meta;
}


void ReadFromBlocks(struct Metadata meta) {
    printf("--- Reconstructed content from FAT blocks ---\n");

    int remaining = meta.bytes;
    for (int i = 0; i < meta.blocks; i++) {
        int idx = meta.blockIndices[i];
        int chunkSize = (remaining < 64) ? remaining : 64;

        fwrite(FAT[idx].data, 1, chunkSize, stdout);
        remaining -= chunkSize;
    }

    printf("\n--- End of reconstructed content ---\n");
}
void SaveFAT(void) {
    FILE *fp = fopen("storage/fat.img", "wb");
    if (fp == NULL) { perror("Error saving FAT"); return; }
    fwrite(FAT, sizeof(Block), BLOCKnum, fp);
    fwrite(&TotusedB, sizeof(int), 1, fp);
    fclose(fp);
}
void LoadFAT(void) {
    FILE *fp = fopen("storage/fat.img", "rb");
    if (fp == NULL) return;
    fread(FAT, sizeof(Block), BLOCKnum, fp);
    fread(&TotusedB, sizeof(int), 1, fp);
    fclose(fp);
}

