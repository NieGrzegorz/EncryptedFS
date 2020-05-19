# EncryptedFS
Encrypted filesystem implemented on user space using FUSE
The filesystem is encrypted with AES cipher using OpenSsl library

## Prerequisites
- C++ compiler which supports C++14 features
- libfuse and fuse.ko 
- OpenSsl library 
- Cmake 

## Get repository 
## Building
### Create build directory and build with Cmake
Run following commands
* mkdir build 
* cd build 
* cmake ../
* make

## Running filesystem
Run encryptedFilesystem.sh from project's main directory
* encryptedFilesystem.sh [mountPoint] [Aes key file] [Aes iv file]
