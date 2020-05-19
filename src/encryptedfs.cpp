/**
 * Definitions of filesystem specific operations
 *
 */

#include "encryptedfs.h"
#include "fsutils.h"
#include "logger.h" 
#include "AesCipher.h"
#include "BasicFile.h"
#include <fstream>

extern "C"
{

#include <unistd.h>
#include <dirent.h>
#include <cstring>
}

namespace encryptedfs
{

    std::string ivFile = "";
    std::string keyFile = "";
    constexpr int Err = -1; 

    static FsInfo* getFsInfo()
    {
        return (FsInfo *)fuse_get_context()->private_data;
    }

    void *efs_init(struct fuse_conn_info *conn)
    {
        (void) conn;
        FsInfo *info = getFsInfo();
        keyFile.append(info->keyFile);
        ivFile.append(info->ivFile);
        fchdir(info->mountPointFd);
        close(info->mountPointFd);

        return NULL; 
    } 

    int efs_getattr(const char *path, struct stat *st)
    {
        int retVal = 0;
        memset(st, 0, sizeof(struct stat));

        const char *fullPath = getRelPath(path);
        
        std::string log = "getattr path: ";
        log.append(fullPath);
        LOG_OP(log);

        retVal = lstat(fullPath, st);
        if(Err == retVal)
        {
            return -errno;
        }
        return 0;
    }

    int efs_access(const char *path, int mask)
    {
        int retVal = 0; 
        const char *fullPath = getRelPath(path);
        retVal = access(fullPath, mask);
        if(Err == retVal)
        {
            return -errno;
        }
        return retVal;
    }

    int efs_mknod(const char *path, mode_t mode, dev_t dev)
    {
        int retVal = 0;
        const char *fullPath = getRelPath(path);

        // mkfifo for fifo special files
        if(S_ISFIFO(mode))
        {
            retVal = mkfifo(fullPath, mode);
        }
        else
        {
            retVal = mknod(fullPath, mode, dev);
        }
        return retVal;
    }

    int efs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
    {
        const char *fullPath = getRelPath(path);
        struct dirent *dentry;
        DIR *dirp;

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
        const char *fullPath = getRelPath(path);

        std::string log = "open path: ";
        log.append(fullPath);
        LOG_OP(log);
        
        int fd = open(fullPath, fi->flags, 0);

        if(Err == fd)
        {
            return -errno;
        }
        fi->fh = fd;

        return 0;
    }

    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
    {
        int retVal = 0;
        std::string filePath = getRelPath(path);
        auto file = std::make_unique<BasicFile>(filePath, keyFile, ivFile);
        retVal = file->read(buf, size, offset);

        if(Err == retVal)
        {
            return -errno;
        }

        return size;
    }

    int efs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
    {
        std::string filePath = getRelPath(path);
        auto file = std::make_unique<BasicFile>(filePath, keyFile, ivFile);
        file->write(buf, size);

        return size;
    }

    int efs_release(const char *path, struct fuse_file_info *fi)
    {
        int retVal = close(fi->fh);
        if(Err == retVal)
        {
            return -errno;
        }
        return 0;
    }

    int efs_unlink(const char *path)
    {
        int retVal = 0;
        const char *fullPath = getRelPath(path);
        retVal = unlink(fullPath);
        if(Err == retVal)
        {
            return -errno;
        }

        return retVal; 
    }

    int efs_truncate(const char *path, off_t newsize)
    {
        const char *fullPath = getRelPath(path);
        int retVal = truncate(fullPath, newsize);
        if(Err == retVal)
        {
            return -errno; 
        }
        return retVal;
    }

    int efs_utime(const char *path, struct utimbuf *ubuf)
    {
        const char *fullPath = getRelPath(path);
        int retVal = utime(fullPath, ubuf);
        if(Err == retVal)
        {
            return -errno;
        }
        return retVal;
    }

    int efs_utimens(const char *path, const struct timespec ts[2])
    {
        int retVal = 0;
        const char *fullPath = getRelPath(path); 

        retVal = utimensat(AT_FDCWD, fullPath, ts, AT_SYMLINK_NOFOLLOW);
        if(Err == retVal)
        {
            return -errno;
        } 
        return 0;
    }

    int efs_mkdir(const char *path, mode_t mode)
    {
        int retVal = 0;
        const char *fullPath = getRelPath(path);
        retVal = mkdir(fullPath, mode);

        if(Err == retVal)
        {
            return -errno;
        }
        return 0;
    }

    int efs_rmdir(const char *path)
    {
        int retVal = 0;
        const char *fullPath = getRelPath(path);
        retVal = rmdir(fullPath);
        if(Err == retVal)
        {
            return -errno;
        }
        return retVal;
    }

    int efs_rename(const char *path, const char *npath)
    {
        int retVal = 0;
        const char *fullPath = getRelPath(path);
        const char *newFullPath = getRelPath(npath);
        retVal = rename(fullPath, newFullPath);
        if(Err == retVal)
        {
            return -errno; 
        }
        return 0;
    }

    int efs_fsync(const char *path, int datasync, struct fuse_file_info *fi)
    {
        int retVal = fsync(fi->fh);
        if(Err == retVal)
        {
            return -errno;
        }
        return retVal;
    }
}
