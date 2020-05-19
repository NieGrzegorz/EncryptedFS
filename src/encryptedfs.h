/**
 * Declarations of filesystem specific operations
 *
 */

#pragma once

#define FUSE_USE_VERSION 30
extern "C"
{
#include <fuse.h>
}
namespace encryptedfs {

    int efs_getattr(const char *path, struct stat *stbuf);

    int efs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

    int efs_open(const char *path, struct fuse_file_info *fi);

    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

    int efs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

    int efs_release(const char *path, struct fuse_file_info *fi);

    int efs_mknod(const char *path, mode_t mode, dev_t dev);

    int efs_unlink(const char *path);

    int efs_truncate(const char *path, off_t newSize);

    int efs_utime(const char *path, struct utimbuf *ubuf);

    int efs_utimens(const char *path, const struct timespec ts[2]); 

    int efs_mkdir(const char *path, mode_t mode);

    int efs_rmdir(const char *path);

    int efs_rename(const char *path, const char *npath);

    int efs_fsync(const char *path, int datasync, struct fuse_file_info *fi);

    int efs_access(const char *path, int mask);

    void *efs_init(struct fuse_conn_info *conn); 
}
