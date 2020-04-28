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

    void *efs_init(struct fuse_conn_info *conn)
    {
        (void) conn;
        FsInfo *info = getFsInfo();
        fchdir(info->mountPointFd);
        close(info->mountPointFd);

        return NULL; 
    } 

    int efs_getattr(const char *path, struct stat *st)
    {
        FsInfo *info = getFsInfo();
        int retVal = 0;
        memset(st, 0, sizeof(struct stat));

        //Full path should be used - add utils
        const char *fullPath = getRelPath(path, info->mountPoint);
        
        std::string log = "getattr path: ";
        log.append(fullPath);
        LOG_OP(log);

        retVal = lstat(fullPath, st);
        if(-1 == retVal)
        {
            return -errno;
        }
        return 0;
    }

    int efs_access(const char *path, int mask)
    {
        int retVal = 0; 
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);

        retVal = access(fullPath, mask);
        if(-1 == retVal)
        {
            return -errno;
        }
        return 0;

    }

    int efs_mknod(const char *path, mode_t mode, dev_t dev)
    {
        int ret = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);

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
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        struct dirent *dentry;
        DIR *dirp;

        //dirp = (DIR *)(uintptr_t) fi->fh;

        dirp = opendir(fullPath);
        if(NULL == dirp)
        {
            std::string errlog = "opendir failed dirp NULL, path: ";
            errlog.append(fullPath); 
            LOG_ERR(errlog);
            return -errno; 
        }

        while((dentry = readdir(dirp)) != NULL)
        {
            if(filler(buf, dentry->d_name, NULL, 0) != 0)
            {
                break;
            }
        }

        closedir(dirp);
        std::string log = "readdir path: ";
        log.append(fullPath);
        LOG_OP(log);

        return 0;

    }

    int efs_open(const char *path, struct fuse_file_info *fi)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        int fd = 0;

        std::string log = "open path: ";
        log.append(fullPath);
        LOG_OP(log);
        
        fd = open(fullPath, fi->flags, 0);
        if(-1 == fd)
        {
            return -errno;
        }
        fi->fh = fd;

        return 0;
    }

    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
    {
        int retVal = 0;
        retVal = pread(fi->fh, buf, size, offset);
        if(-1 == retVal)
        {
            return -errno;
        }

        std::string log = "read path: ";
        log.append(path);
        LOG_OP(log);

        return retVal;
    }

    int efs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
    {
        int retVal = 0; 
        retVal = pwrite(fi->fh, buf, size, offset);
        if(-1 == retVal)
        {
            return -errno; 
        }

        std::string log = "write path: ";
        log.append(path);
        LOG_OP(log);

        return retVal;
    }

    int efs_release(const char *path, struct fuse_file_info *fi)
    {
        close(fi->fh);
        return 0;

    }

    int efs_unlink(const char *path)
    {
        int retVal = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        retVal = unlink(fullPath);
        if(-1 == retVal)
        {
            return -errno;
        }

        return 0; 
    }

    int efs_truncate(const char *path, off_t newsize)
    {
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        return truncate(fullPath, newsize);
    }

    int efs_utime(const char *path, struct utimbuf *ubuf)
    {
        
        FsInfo *info = getFsInfo(); 
        const char *fullPath = getRelPath(path, info->mountPoint);

        return utime(fullPath, ubuf);
    }

    int efs_utimens(const char *path, const struct timespec ts[2])
    {
        int retVal = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint); 

        retVal = utimensat(AT_FDCWD, fullPath, ts, AT_SYMLINK_NOFOLLOW);
        if(-1 == retVal)
        {
            return -errno;
        } 
        return 0; 
    }

    int efs_mkdir(const char *path, mode_t mode)
    {
        int retVal = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        retVal = mkdir(fullPath, mode);

        if(-1 == retVal)
        {
            return -errno;
        }

        return 0;
    }

    int efs_rmdir(const char *path)
    {
        int retVal = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        retVal = rmdir(fullPath);
        if(-1 == retVal)
        {
            return -errno;
        }

        return 0;
    }

    int efs_rename(const char *path, const char *npath)
    {
        int retVal = 0;
        FsInfo *info = getFsInfo();
        const char *fullPath = getRelPath(path, info->mountPoint);
        const char *newFullPath = getRelPath(npath, info->mountPoint);
        retVal = rename(fullPath, newFullPath);
        if(-1 == retVal)
        {
            return -errno; 
        }
        return 0;
    }

    int efs_fsync(const char *path, int datasync, struct fuse_file_info *fi)
    {
        return fsync(fi->fh);

    }

}
