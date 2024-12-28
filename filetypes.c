#include "filetypes.h"

#include <complex.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

bool ft_is_elf(FILE *fp) {
    fseek(fp, 0, SEEK_SET); // read the right bytes

    char elf_mag[ELF_MAG_LEN] = {0x7f, 'E', 'L', 'F'};
    char buf[ELF_MAG_LEN] = {};

    fread(buf, sizeof(buf), 1, fp);
    
    for (int i = 0; i < ELF_MAG_LEN; i++)
        if (buf[i] != elf_mag[i]) return false;
    return true;
}

bool ft_is_png(FILE *fp) {
    fseek(fp, 0, SEEK_SET); // read the right bytes

    char png_mag[PNG_MAG_LEN] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
    char buf[PNG_MAG_LEN] = {};

    fread(buf, sizeof(buf), 1, fp);

    for (int i = 0; i < PNG_MAG_LEN; i++)
        if (buf[i] != png_mag[i]) return false;
    return true;
}

bool ft_is_pdf(FILE *fp) {
    fseek(fp, 0, SEEK_SET); // read the right bytes 

    char pdf_mag[PDF_MAG_LEN - 1] = {'%', 'P', 'D', 'F', '-', '1', '.'};
    char buf[PDF_MAG_LEN] = {};

    fread(buf, sizeof(buf), 1, fp);

    for (int i = 0; i < PDF_MAG_LEN - 1; i++)
        if (buf[i] != pdf_mag[i]) return false;

    /* check if it's a valid version */
    if (buf[sizeof(buf) - 1] >= '0' && buf[sizeof(buf) - 1] <= '7')
        return true;

    return false;
}

int ft_determine_filetype(FILE *fp) {
    if (ft_is_elf(fp)) return FT_ELF;
    if (ft_is_png(fp)) return FT_PNG;
    if (ft_is_pdf(fp)) return FT_PDF;
    return FT_UNKNOWN; /* always return a value */
}

file_inf ft_get_file_info(const char *fname) {
    assert(fname);
    file_inf finf;
    
    /* open the file */
    finf.fp = fopen(fname, "r");
    if (!finf.fp) {
        if (errno == ENOENT) {
            printf("No such file: %s\n", fname);
            exit(-errno);
        }
        else if (errno == EACCES) {
            printf("Permission denied.\nRun with sudo to access %s\n", fname);
            exit(-errno);
        }
        else {
            perror("fopen");
            exit(-errno);
        }
    }

    finf.fname = fname;

    /* get file system info about file */
    if (lstat(fname, &finf.fstat) == -1) {
        perror("lstat");
        exit(-errno);
    }

    /* determine if is dir */
    if (S_ISDIR(finf.fstat.st_mode))
        finf.isdir = true;
    else finf.isdir = false;    

    finf.filetype = ft_determine_filetype(finf.fp);

    return finf;
}