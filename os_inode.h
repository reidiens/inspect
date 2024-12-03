#ifndef _F_OS_INODE_
    #define _F_OS_INODE_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

void pr_path(const char *fname);
void pr_inode(ino_t ino);
void pr_home(dev_t dev);
void pr_uid(uid_t uid);
void pr_gid(gid_t gid);
void pr_owners(uid_t uid, gid_t gid);
void pr_type(mode_t mode);
void print_inode_info(const char *fname, struct stat *fstat);

#endif