/**
 * Definitions of filesystem specific operations
 *
 */

#include "encryptedfs.h"
#include "fsutils.h"
#include "logger.h" 

extern "C"
{

#include <unistd.h>
#include <dirent.h>
#include <cstring>
}

namespace encryptedfs
{

    static FsInfo* getFsInfo()
    {
        return (FsInfo *)fuse_get_context()->private_data;
    }

    int efs_getattr(const char *path, struct stat *st)
    {
        FsInfo *info = getFsInfo();
        int ret = 0;
        memset(st, 0, sizeof(struct stat));

        //Full path should be used - add utils
        const char *fullPath = getAbsPath(path, info->mountPoint);
        std::string s(fullPath); 
        Logger::getInstance().Log_trace(s); 
        ret = lstat(fullPath, st);
        return ret;
    }

    int efs_mknod(const char *path, mode_t mode, dev_t dev)
    {
        int ret = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);

        // mkfifo for fifo special files
        if(S_ISFIFO(mode))
        {
            ret = mkfifo(fullPath, mode);
        }
        else
        {
            ret = mknod(fullPath, mode, dev);
        }
        return ret;

    }

    int efs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
    {
        struct dirent *dentry;
        DIR *dirp;

        dirp = (DIR *)(uintptr_t) fi->fh;

        dentry = readdir(dirp);

        if(0 == dentry) return 0; 

        do
        {
            if(filler(buf, dentry->d_name, NULL, 0) != 0)
            {
                return -ENOMEM; 
            }
        }
        while((dentry = readdir(dirp)) != NULL);
        return 0;

    }

    int efs_open(const char *path, struct fuse_file_info *fi)
    {
        int ret = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);
        int fd;

        fd = open(fullPath, fi->flags, 0);
        fi->fh = fd;

        return ret;
    }

    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
    {
        return pread(fi->fh, buf, size, offset);
    }

    int efs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
    {
        return pwrite(fi->fh, buf, size, offset);
    }

    int efs_release(const char *path, struct fuse_file_info *fi)
    {
        return close(fi->fh);

    }

    int efs_unlink(const char *path)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);
        return unlink(fullPath);

    }

    int efs_truncate(const char *path, off_t newsize)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);
        return truncate(fullPath, newsize);
    }

    int efs_utime(const char *path, struct utimbuf *ubuf)
    {
        return 0;
    }

    int efs_mkdir(const char *path, mode_t mode)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);
        return mkdir(fullPath, mode);

    }

    int efs_rmdir(const char *path)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);
        return rmdir(fullPath);

    }

    int efs_rename(const char *path, const char *npath)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getAbsPath(path, info->mountPoint);
        const char *newFullPath = getAbsPath(npath, info->mountPoint);
        return rename(fullPath, newFullPath);

    }

    int efs_fsync(const char *path, int datasync, struct fuse_file_info *fi)
    {
        return fsync(fi->fh);

    }

}
