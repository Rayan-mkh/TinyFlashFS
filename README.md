# TinyFlashFS

A simple simulated filesystem written in C, built as a learning project.

It mimics how a basic flash filesystem works: files are split into fixed-size
64-byte blocks, tracked with a FAT (File Allocation Table), and stored
in-memory during runtime. Each file's metadata (name, type, ID, date, size,
and block indices) is saved as a JSON file, and the FAT itself is persisted
to disk so block usage survives between runs.

## Features
- Create files and write content, split and stored across FAT blocks
- List, look up, and delete stored files (frees their blocks on delete)
- Metadata saved as JSON per file
- FAT state persisted to `storage/fat.img` between runs
- Simple terminal menu to navigate the filesystem

## Build & Run
```bash
make
./main
```
or, to build and run in one step:
```bash
./makeandrun.sh
```

## Project structure
```
src/core/         common.h / common.c   - shared types (FileInfo, Block, FAT)
src/filesystem/   file.c / file.h       - file creation & block storage
                  dir.c / dir.h         - list / lookup / delete files
                  fat.c / fat.h         - FAT array & block utilities
                  main.c                - entry point, menu loop
storage/          generated metadata (.json) and fat.img (not tracked in git)
```

## Status
Work in progress — built step by step while learning C and filesystem concepts.