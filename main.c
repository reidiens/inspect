#include "inode.h"
#include "filetypes.h"

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
    print_inode_info(argv[1], &fstat);
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Could not open file");
        return errno;
    }
    file_t *file = open_file(argv[1]); 
    if (!file) {
        printf("Couldn't open file\n");
        return -1;
    }
    if (!S_ISDIR())
        printf("\nFiletype:\t\t");
        switch (file->ftype) {
            case FT_ELF:
                printf("ELF\n");
                break;
            case FT_PNG:
                printf("PNG\n");
                break;
            case FT_PDF:
                printf("PDF\n");
                break;
            default: printf("Unknown\n");
        }
}

void usage() {
    puts("usage: f <filename>");
    puts("View detailed information about <filename>\n");
}