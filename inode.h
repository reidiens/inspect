#ifndef _I_INODE_
    #define _I_INODE_


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

typedef struct Inode_Info {
    char *path;
    char *fname;
    
} inode_inf_t;

void pr_path(const char *fname);
void pr_inode(ino_t ino);
void pr_home(dev_t dev);
void pr_uid(uid_t uid);
void pr_gid(gid_t gid);
void pr_owners(uid_t uid, gid_t gid);
void pr_type(struct stat *fstat);
void pr_blksize(size_t blksize);
void pr_permissions(mode_t mode);
void pr_size(size_t size);
void print_inode_info(const char *fname, struct stat *fstat);

#endif
