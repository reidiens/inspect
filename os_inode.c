#include "os_inode.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

void pr_inode(ino_t ino) {
    printf("Inode:\t\t\t%ld\n", ino);
}

void pr_home(dev_t dev) {
    printf("Home:\t\t\t%ld\t\t", dev);
    char dev_ver[128], buf[256];
    if (sprintf(dev_ver, "%d:%d", (int)dev >> 8, (int)dev & 0xff) < 0) {
        printf("\n");
        return;
    }
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

char* ret_ftype(mode_t mode) {
    switch (mode & __S_IFMT) {
        case __S_IFDIR: return "Directory";
        case __S_IFBLK: return "Block device";
        case __S_IFCHR: return "Character device";
        case __S_IFREG: return "Regular file";
        case __S_IFIFO: return "Pipe/FIFO";
        case __S_IFSOCK: return "Socket";
        case __S_IFLNK: return "Symbolic link";
        default: return "Unknown";
    }
}

void pr_type(mode_t mode) {
    printf("Type:\t\t\t%s\n", ret_ftype(mode));
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
        printf("%lu\n", size);
}

void print_inode_info(struct stat *fstat) {
    pr_inode(fstat->st_ino);
    pr_home(fstat->st_dev);
    pr_owners(fstat->st_uid, fstat->st_gid); 
    pr_type(fstat->st_mode);
    pr_size(fstat->st_size);
}