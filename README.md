# EncryptedFS
Encrypted filesystem implemented on user space using FUSE
The filesystem is encrypted with AES cipher using OpenSsl library

## Prerequisites
- C++ compiler which supports C++14 features
- libfuse and fuse.ko 
- OpenSsl library 
- Cmake 

## Get repository 
* git clone https://github.com/NieGrzegorz/EncryptedFS.git

## Building
### Create build directory and build with Cmake
Run following commands
* mkdir build 
* cd build 
* cmake ../
* make

## Running filesystem
Filesystem requires two files: 
* file with aes key 
* file with aes iv 

Generate aes key and iv: 
* openssl enc -nosalt -aes-256-cbc -k hello-aes -P

Save the output in two seperate files. 

Run encryptedFilesystem.sh from project's main directory
* encryptedFilesystem.sh [mountPoint] [Aes key file] [Aes iv file]

## Unmount filesystem 
Use fusermount utility: 
* fusermount -u [mountPoint]
