/**
 * Declarations of filesystem specific operations
 *
 */

namespace encryptedfs {

    // Fills stat stucture 
    //Essential 
    //Check out stat() function and stat struct  
    int efs_getattr(const char *path, struct stat *stbuf);
       
    // To list directory, check fuse filler(!) 
    int efs_readdir(); 

    //Open redirected? Should it be defined? 
    int efs_open(const char *path); 

    //Read file's data and decrypt it
    int efs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi); 

    //Write data and encrypt it 
    int efs_write(); 
}
