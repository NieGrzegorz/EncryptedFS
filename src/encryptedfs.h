/**
 * Declarations of filesystem specific operations
 *
 */

namespace encryptedfs {

	//Essential 
	//Check out stat() function and stat struct  
	efs_getattr(const char *path, struct stat *stbuf);
       
	// To list directory, check fuse filler(!) 
	efs_readdir(); 
	
	//Open redirected? Should it be defined? 
	efs_open(const char *path); 
	
	//Read file's data and decrypt it
	efs_read(); 

	//Write data and encrypt it 
	efs_write(); 
}
