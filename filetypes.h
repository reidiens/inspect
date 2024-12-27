#ifndef _FILETYPES_H_
    #define _FILETYPES_H_

#define NMAGICNUMS      3

#define FT_ELF          0
#define FT_PNG          1
#define FT_PDF          2

#include <stdio.h>

typedef struct FileInfo {
    FILE *fp;
    int ftype;
    char *magic;
} file_t;

#define FT_ELF      0
#define FT_PNG      1

file_t* open_file(const char *fname);

#endif