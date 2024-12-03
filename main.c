#include "os_inode.h"

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

void usage();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return -1;
    }
    struct stat fstat;
    if (lstat(argv[1], &fstat) == -1) {
        if (errno == ENOENT) {
            printf("No such file: %s\n", argv[1]);
            return -2;
        }
        else {
            perror("lstat:");
            return errno;
        }
    }
    print_inode_info(&fstat);
}

void usage() {
    puts("usage: f <filename>");
    puts("View detailed information about <filename>\n");
}