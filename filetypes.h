#ifndef _FILETYPES_H_
    #define _FILETYPES_H_

#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>

#define FT_UNKNOWN  0
#define FT_ELF      1
#define FT_PNG      2
#define FT_PDF      3
#define FT_GZIP     4
#define FT_PKZIP    5
#define FT_RAR      6

#define ELF_MAG_LEN     4
#define PNG_MAG_LEN     8
#define PDF_MAG_LEN     8
#define GZIP_MAG_LEN    2
#define PKZIP_MAG_LEN   4
#define RAR_MAG_LEN     7

typedef struct File_Info {
    const char *fname;
    FILE *fp;
    struct stat fstat;
    int filetype;
    bool isdir;
} file_inf;

bool ft_is_elf(FILE *fp);
bool ft_is_png(FILE *fp);
bool ft_is_pdf(FILE *fp);
bool ft_is_gzip(FILE *fp);
bool ft_is_pkzip(FILE *fp);
bool ft_is_rar(FILE *fp);

int ft_determine_filetype(FILE *fp);

file_inf ft_get_file_info(const char *fname);

#endif