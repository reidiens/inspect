#include "pr_inf.h"

#include "filetypes.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

void pr_path(const char *fname) {
    printf("Path: ");

    /* if path is absolute, print it as-is and quit */
    if (fname[0] == '/') {
        printf("%s\n\n", fname);
        return;
    }

    /* if path is relative, find absolute path and quit */
    else if (fname[0] == '.' && (fname[1] == '/' || fname[1] == '.')) {
        char *p = realpath(fname, NULL);
        if (!p) {
            if (errno == EACCES) {
                puts("Permission denied\n");
                return;
            }
            else return;
        }
        printf("%s\n\n", p);
        free(p);
        return;
    }

    /* if only filename is given, find and prepend path */
    size_t abs_path_size = (4096 + (strlen(fname) + 1)) * sizeof(char);
    
    char *abs_path = malloc(abs_path_size);
    if (!abs_path) {
        perror("Malloc");
        exit(errno);
    }

    abs_path = getcwd(abs_path, abs_path_size);
    if (abs_path == NULL) {
        if (errno == EACCES) puts("Permission denied\n");

        /* if absolute path buffer is too small, realloc until big enough */
        else if (errno == ERANGE) {
            do {
                abs_path_size *= 2;
                abs_path = realloc(abs_path, abs_path_size);
                if (!abs_path) {
                    perror("Realloc");
                    exit(errno);
                }
                abs_path = getcwd(abs_path, abs_path_size);
            } while (abs_path == NULL && errno == ERANGE);
        }

        else {
            perror("Getcwd");
            exit(errno);
        }
    }

    strcat(abs_path, "/");
    strcat(abs_path, fname);
    printf("%s\n\n", abs_path);
    free(abs_path);
}

void pr_inode(ino_t ino) {
    printf("Inode:\t\t\t%ld\n", ino);
}

void pr_home(dev_t dev) {
    /* print info about the "home" of the file - where it is in the file system */
     
    /* print the home number */
    printf("Home:\t\t\t%ld\t\t", dev);
    char dev_ver[128], buf[256];
    if (sprintf(dev_ver, "%d:%d", (int)dev >> 8, (int)dev & 0xff) < 0) {
        printf("\n");
        return;
    }

    /* print the home name - usually /dev/[something] */
    FILE *fp = fopen("/proc/self/mountinfo", "r");
    if (!fp) {
        printf("\n");
        return;
    }
     
    char *tok, *loc;
    while (fgets(buf, sizeof(buf), fp)) {
        tok = strtok(buf, "-");
        loc = strstr(tok, dev_ver);
        if (loc) {
            for (int i = 0; i < 2; i++) {
                tok = strtok(NULL, " -:");
            }
            break;
        }
    }
    fclose(fp);
    printf("%s\n", (loc) ? tok : " ");
}

void pr_uid(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    printf("UID:\t\t\t%u\t\t%s\n", uid, (pw) ? pw->pw_name : " "); 
}

void pr_gid(gid_t gid) {
    struct group *gr = getgrgid(gid);
    printf("GID:\t\t\t%u\t\t%s\n", gid, gr->gr_name);
}

void pr_owners(uid_t uid, gid_t gid) {
    pr_uid(uid);
    pr_gid(gid);
}

char* ret_itype(mode_t mode) { // helper function
    switch (mode & __S_IFMT) {
        case __S_IFDIR: return "Directory";
        case __S_IFBLK: return "Block device";
        case __S_IFCHR: return "Char. device";
        case __S_IFREG: return "Regular file";
        case __S_IFIFO: return "Pipe/FIFO";
        case __S_IFSOCK: return "Socket";
        case __S_IFLNK: return "Symbolic link";
        default: return "Unknown";
    };
}

void pr_inode_type(struct stat *fstat) {
    printf("Type:\t\t\t%s", ret_itype(fstat->st_mode));
    if (S_ISBLK(fstat->st_mode) || S_ISCHR(fstat->st_mode))
        printf("\t%ld\n", fstat->st_rdev);
    else puts(" ");
}

void get_entcount(const char *fname) { // helper function (for directories only)
    /* count the number of files (or directories) in a directory */

    printf("# of entries:\t\t");
    int count = 0, hidden = 0;
    DIR *d = opendir(fname);
    if (!d) {
        if (errno == EACCES) {
            printf("Permission denied\n");
            return;
        }
        else if (errno == ELOOP) {
            printf("Unknown (symlink loop)\n");
            return;
        }
        else {
            perror("");
            return;
        }
    } 

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
        if (ent->d_type == DT_REG || ent->d_type == DT_DIR || ent->d_type == DT_LNK) {
            count++;
            if (ent->d_name[0] == '.') hidden++;
        }
    }
    closedir(d);

    printf("%d", count);
    if (hidden != 0) printf(" (%d hidden)", hidden);
    printf("\n");
}

void pr_blksize(size_t blksize) {
    printf("Block size:\t\t%ld\n", blksize);
}

void pr_permissions(mode_t mode) {
    /* print permissions for the file in standard rwxrwxrwx format */
    printf("Permissions:\t\t");

    int bits = mode;

    for (int i = 0; i < 3; i++) {
        bits = mode;
        bits &= 0x1C0 >> (3 * i);
        bits = bits << (3 * i);
        for (int k = 0; k < 3; k++) {
            if ((bits & 0x100) == 0x100) {
                switch (k) {
                    case 0:
                        printf("r");
                        break;
                    case 1:
                        printf("w");
                        break;
                    case 2:
                        printf("x");
                        break;
                    default: break;
                }
            }
            else printf("-");
            bits = bits << 1;
        }
    }
    printf("\n");
}

void pr_size(size_t size) {
    int mb = 1024 * 1024;
    printf("Size:\t\t\t");

    if (size >= 1024 && size < mb)
        printf("%.2fK\n", (float)size / 1024);
    else if (size >= mb && size < (mb * 1024))
        printf("%.2fM\n", (float)size / mb);
    else if (size > (mb * 1024))
        printf("%.2fG\n", (float)size / (mb * 1024));
    else
        printf("%lu bytes\n", size);
}

void pr_filetype(int filetype) {
    /* print file type of (regular) file */
    printf("File type:\t\t");
    switch (filetype) {
        case FT_ELF:
            printf("Elf\n");
            break;
        case FT_PNG:
            printf("PNG\n");
            break;
        case FT_PDF:
            printf("PDF\n");
            break;
        default: printf("Unknown\n");
    };
}

void print_info(file_inf *finf) {
    /* print all file information available */

    struct stat fs = finf->fstat; // store teh stat so that ur not typing finf->fstat.st_...
    
    /* inode information */
    pr_path(finf->fname);
    pr_inode(fs.st_ino);
    pr_home(fs.st_dev);
    pr_owners(fs.st_uid, fs.st_gid); 
    pr_inode_type(&fs);
    if (finf->isdir)
        get_entcount(finf->fname);
    pr_permissions(fs.st_mode);
    pr_size(fs.st_size);
    
    /* file information */
    puts(" ");
    if (S_ISREG(fs.st_mode))
        pr_filetype(finf->filetype);
}