
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define SCRIPT_VERSION "v1.0.0-beta-1, Feb 8, 2019"

void sfp(int input) {

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
        fprintf(stderr, "SEGMENTATION FAULT. FUCK YOU");
    }

    ssize_t readbytes;

    readbytes = read(input, buffer, filestats.st_blksize);
    while (readbytes > 0) {
        ssize_t writtenbytes = write(output, buffer, readbytes);
        readbytes = read(input, buffer, filestats.st_blksize);
    }
}

int main(int argc, char *argv[]) {
    int descriptor;
    char *SCRIPT_NAME = *argv;
    // if there are no args just read from stdin
    // this gives us a file descriptor to pass to the write call
    if (argc < 2) {
        descriptor = fileno(stdin);
        sfp(descriptor);
    } else {
        argv++;
        while (*argv) {
            if (strcmp("-", *argv) == 0) {
                descriptor = fileno(stdin);
            } else {
                if (access(*argv, F_OK ) == -1) {
                    fprintf(stderr, "%s: No such file or directory: %s\n", SCRIPT_NAME, *argv);
                    exit(2);
                }
                descriptor = open(*argv, O_RDONLY);
            }
            if (descriptor < 0) {
                fprintf(stderr, "An error has occured...\n");
                fprintf(stderr, "%d", errno);
                exit(2);
            }
            sfp(descriptor);
            argv++;
        }
    }
}

//
// End main-sfp.c
//
