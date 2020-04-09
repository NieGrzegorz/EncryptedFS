/**
 * Definitions of filesystem specific operations
 *
 */

#include <encryptedfs.h>
#include <unistd.h> 

namespace encryptedfs
{

    int efs_getattr(const char *path, struct stat *st)
    {
            int ret = 0;
            memset(st, 0, sizeof(struct stat)); 

            ret = lstat(path, st); 
            return 0; 
    }

    int efs_readdir()
    {

    }

    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) 
    {

    }

    int efs_open(const char *path) 
    {

    }

    int efs_close()
    {

    }
}
