// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/pac
// date: Sep 22, 2019
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

#define SCRIPT_VERSION "v1.0.0-beta-8, Sep 22, 2019"

// To avoid that warning.
// TODO: Theres proboly a better way to handle this.
int fileno(const FILE*);

void helpMenu(char* SCRIPT_NAME) {
    printf("USAGE:\n");
    printf("  %s [option] <path/file>\n", SCRIPT_NAME);
    printf("\n");
    printf("Print file, or stdin to stdout, or custom file (comming soon).\n");
    printf("\n");
    exit(0);
}

void versionPrint() {
    printf("%s\n", SCRIPT_VERSION);
    exit(0);
}

void pac(int input) {
    // write to stdout
    int output = fileno(stdout);

    static char* filebuff;
    struct stat filestats;

    if (fstat(output, &filestats) == -1) {
        perror("fstat");
        fprintf(stderr, "%d", errno);
    }

    // st_blksize is size_t
    filebuff = malloc(filestats.st_blksize);
    if (filebuff == NULL) {
        perror("malloc");
        fprintf(stderr, "malloc failed\n");
        exit(255);
    }

    ssize_t readb = read(input, filebuff, filestats.st_blksize);
    while (readb > 0) {
        write(output, filebuff, readb);
        readb = read(input, filebuff, filestats.st_blksize);
    }
}

int main(int argc, char **argv) {
    char *SCRIPT_NAME = *argv;
    int descriptor;

    if (argc >= 1) {
        for (int i=1; i < argc; i++) {
            if (strcmp(argv[i], "--help") == 0) {
                helpMenu(SCRIPT_NAME);
                break;
            } else if (strcmp(argv[i], "--version") == 0) {
                versionPrint();
                break;
            }
        }
    }

    if (argc < 2) {
        descriptor = fileno(stdin);
        pac(descriptor);
    } else {
        for (int i = 1; i < argc; i++) {
            if ((strlen(argv[i]) == 1) && (argv[i][0] == '-')) {
                descriptor = fileno(stdin);
            } else {
                if (access(argv[i], F_OK ) != 0) {
                    fprintf(stderr, "%s: %s: No such file or directory\n", SCRIPT_NAME, argv[i]);
                    continue;
                }

                if (access(argv[i], R_OK) != 0) {
                    fprintf(stderr, "%s: %s: Permision denied\n", SCRIPT_NAME, argv[i]);
                    continue;
                }

                struct stat statbuf;
                stat(argv[i], &statbuf);
                if (S_ISDIR(statbuf.st_mode) != 0) {
                    fprintf(stderr, "%s: %s: Is a directory\n", SCRIPT_NAME, argv[i]);
                    continue;
                }
                descriptor = open(argv[i], O_RDONLY);
            }
            if (descriptor < 0) {
                fprintf(stderr, "%s: unable to open file: %s: %s\n", SCRIPT_NAME, argv[i], strerror(errno));
                continue;
            }
            pac(descriptor);
            close(descriptor);
        }
    }
}

//
// End main-pac.c
//
