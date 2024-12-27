#include "filetypes.h"

#include <mutex>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct FT_Magic {
    char elf[4];
    char png[8];
    char pdf[7];
} ft_magics;

ft_magics* init_magics() {
    ft_magics *ret = (ft_magics *)malloc(sizeof(ft_magics));
    if (!ret) {
        perror("Malloc");
        exit(errno);
    }
    
    char elf[4] = {0x7f, 'E', 'L', 'F'};
    char png[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    char pdf[7] = {'%', 'P', 'D', 'F', '-', '1', '.'};

    strcpy(ret->elf, elf);
    strcpy(ret->png, png);
    strcpy(ret->pdf, pdf);

    return ret;
}

int get_type(FILE *fp) {
    ft_magics *magic = init_magics();
    char c;
    int k;
    for (int i = 0; i < NMAGICNUMS; i++) {
        k = 0;
        do {
            c = fgetc(fp);
            if (c != magics[i][k]) break; 
            if (k == strlen(magics[i])) return i;
            k++; 
        } while (c != EOF);
    }
    free(magics);
    return -1;
}

file_t* open_file(const char *fname) {
    file_t *file = (file_t *)malloc(sizeof(file_t));
    if (!file) return NULL;
    file->fp = fopen(fname, "rb");
    if (!file->fp) {
        free(file);
        return NULL;
    }
    file->ftype = get_type(file->fp);
    return file;
}
