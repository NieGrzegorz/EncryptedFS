/**
 * Declarations of filesystem specific operations
 *
 */

#ifndef ENCRYPTEDFS_H
#define ENCRYPTEDFS_H

namespace encryptedfs {

    // Fills stat stucture 
    //Essential 
    //Check out stat() function and stat struct  
    int efs_getattr(const char *path, struct stat *stbuf);
       
    // To list directory, check fuse filler(!) 
    int efs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi); 

    //Open redirected? Should it be defined? 
    int efs_open(const char *path); 

    //Read file's data and decrypt it
    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi); 

    //Write data and encrypt it 
    int efs_write(const char *path, const char *buf, size_t size, off_t offset, struc fuse_file_info *fi);

    //Release opened file 
    int efs_release(const char *path, struct fuse_file_info *fi);

    int efs_mknod(const char *path, mode_t mode, dev_t dev); 

    int efs_unlink(const char *path); 

    int efs_truncate(const char *path, off_t newSize); 

    int efs_utime(const char *path, struct utimbuf *ubuf); 

    int efs_mkdir(const char *path, mode_t mode); 

    int efs_rmdir(const char *path); 

    int efs_rename(const char *path, const char *npath); 

    int efs_fsync(const char *path, int datasync, struct fuse_file_info *fi); 
}

#endif
