#ifndef _I_PR_INF_
    #define _I_PR_INF_

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

void pr_path(const char *fname);
void pr_inode(ino_t ino);
void pr_home(dev_t dev);
void pr_uid(uid_t uid);
void pr_gid(gid_t gid);
void pr_owners(uid_t uid, gid_t gid);
void pr_inode_type(struct stat *fstat);
void pr_blksize(size_t blksize);
void pr_permissions(mode_t mode);
void pr_size(size_t size);

void pr_filetype(int filetype);

void print_info(file_inf *finf);

#endif