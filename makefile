CC = gcc
CFLAGS = -Wall

SRCS = src/filesystem/main.c src/filesystem/file.c src/filesystem/dir.c src/filesystem/fat.c src/core/common.c
TARGET = main

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)