/**
 * Definitions of filesystem specific operations
 *
 */

#include <encryptedfs.h>
#include <unistd.h> 
#include <direntry.h> 

namespace encryptedfs
{

    int efs_getattr(const char *path, struct stat *st)
    {
        int ret = 0;
        memset(st, 0, sizeof(struct stat)); 

        ret = lstat(path, st); 
        return ret; 
    }

    int efs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) 
    {
        struct dirent *dentry; 
        DIR *dirp; 

        dentry = readdir(dirp); 

    }

    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) 
    {

    }

    int efs_open(const char *path) 
    {
        int ret = 0; 
        int fd; 

        fd = open(path); 

        return ret; 
    }

    int efs_release()
    {

    }
}
