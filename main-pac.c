// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/pac
// date: Feb 13, 2019
// version-1.0.0
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define SCRIPT_VERSION "v1.0.0-beta-4, Feb 13, 2019"

void helpMenu(char* SCRIPT_NAME) {
    printf("USAGE:\n");
    printf("  %s [option] <path/file>\n", SCRIPT_NAME);
    printf("\n");
    printf("Print file, or stdin to stdout, or stderr out (comming soon).\n");
    printf("\n");
    exit(0);
}

void versionPrint() {
    printf("%s\n", SCRIPT_VERSION);
    exit(0);
}

void pac(int input) {
    ssize_t readbytes;
//    ssize_t writtenbytes;

    // write to stdout
    int output = fileno(stdout);

    // buffer for the fast reads
    static char* buffer;
    struct stat filestats;

    if (fstat(output, &filestats) == -1) {
        fprintf(stderr, "%d", errno);
    }

    // st_blksize is size_t
    buffer = malloc(filestats.st_blksize);
    if (buffer == NULL) {
        fprintf(stderr, "SEGMENTATION FAULT.\n");
        exit(100);
    }

    readbytes = read(input, buffer, filestats.st_blksize);
    while (readbytes > 0) {
//        writtenbytes = write(output, buffer, readbytes);
        write(output, buffer, readbytes);
        readbytes = read(input, buffer, filestats.st_blksize);
    }
}

int main(int argc, char *argv[]) {
    char *SCRIPT_NAME = *argv;

    int descriptor;

    if (argc >= 1) {
        for (int i=1; i < argc; i++) {
            if (strcmp(argv[i], "--help") == 0) {
                helpMenu(SCRIPT_NAME);
            } else if (strcmp(argv[i], "--version") == 0) {
                versionPrint();
            }
        }
    }

    // if there are no args just read from stdin
    // this gives us a file descriptor to pass to the write call
    if (argc < 2) {
        descriptor = fileno(stdin);
        pac(descriptor);
    } else {
        argv++;
        while (*argv) {
            if (strcmp(*argv, "-") == 0) {
                descriptor = fileno(stdin);
            } else {
                if (access(*argv, F_OK ) == -1) {
                    fprintf(stderr, "%s: No such file or directory: %s\n", SCRIPT_NAME, *argv);
                    return(2);
                }
                struct stat path_stat;
                stat(*argv, &path_stat);
                if (!S_ISREG(path_stat.st_mode)) {
                    fprintf(stderr, "%s: %s: Is a directory.\n", SCRIPT_NAME, *argv);
                    return(2);
                }

                if (!access(*argv, R_OK) == 0) {
                    fprintf(stderr, "%s: %s: Permision denied.\n", SCRIPT_NAME, *argv);
                    return(2);
                }
                descriptor = open(*argv, O_RDONLY);
            }
            if (descriptor < 0) {
                fprintf(stderr, "ERROR: An error has occured: unable to read file.\n");
                return(2);
            }
            pac(descriptor);
            argv++;
        }
    }
}

//
// End main-pac.c
//
