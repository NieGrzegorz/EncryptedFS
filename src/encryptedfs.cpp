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
    constexpr int CHUNK_SIZE = 16;

    
    std::string ivFile = "";
    std::string keyFile = "";

    static FsInfo* getFsInfo()
    {
        return (FsInfo *)fuse_get_context()->private_data;
    }

    std::vector<byte> readWithFstream(const char* path, int size, int offset)
    {

        std::streampos fileSize;
        std::ifstream istream; 
        istream.open(getRelPath(path), std::ifstream::binary| std::ifstream::in);

        if(istream.is_open())
        {
            istream.seekg(offset, std::ios::end);
            fileSize = istream.tellg();
            istream.seekg(offset, std::ios::beg);

            // read the data:
            std::vector<byte> fileData(size);
            if((size+offset) <= int(fileSize))
            {
                
                istream.read((char*) fileData.data(), size);
                istream.sync();
            }
            return fileData;

        }
        else
        {
            throw std::runtime_error("readWithFstream: file is not open");
        }
    }

    void readBlocks(const char *path, std::vector<byte> &rtext, size_t size, int offset)
    {
        for(int i = 0; i < size; i += CHUNK_SIZE)
        {
            std::vector<byte> temp; 
            AesCipher::getInstance().decrypt(readWithFstream(path, CHUNK_SIZE, offset+i), temp);
            rtext.insert(rtext.end(), temp.begin(), temp.end()); 
        }
    }

    void writeWithFstream(const char* path, std::vector<byte> &v)
    {
        std::ofstream ostream; 
        ostream.open(getRelPath(path), std::ofstream::binary|std::ofstream::out|std::ofstream::app);

        if(ostream.is_open())
        {
            ostream.write((char*) v.data(), v.size());
            ostream.flush();
        }
        else
        {
            throw std::runtime_error("writeWithFstream: file is not open");
        }
    }

    void writeBlocks(const char* path, std::vector<byte> &v)
    {
        size_t vectorSize = v.size();
        std::vector<byte> ctext;

        for(int i = 0; i < vectorSize; i += CHUNK_SIZE)
        {
            ctext.clear();
            if(i+CHUNK_SIZE<= vectorSize)
            {
                std::vector<byte>::iterator begin = v.begin() + i;
                std::vector<byte>::iterator end = begin + CHUNK_SIZE;

                std::vector<byte> temp(begin, end);
                AesCipher::getInstance().encrypt(temp, ctext);
                writeWithFstream(path, ctext);
            }
        } 

        if(vectorSize % CHUNK_SIZE)
        {
            const byte nullByte = 0;
            int fillerBits = (vectorSize+16) - (vectorSize%CHUNK_SIZE);
            int bitsNum = fillerBits - vectorSize;
            for(int i = 0; i < bitsNum; i++ )
            {
                v.push_back(nullByte);
            }
                std::vector<byte>::iterator end = v.end();
                std::vector<byte>::iterator begin = v.end() - CHUNK_SIZE;
                

                std::vector<byte> temp(begin, end);
                AesCipher::getInstance().encrypt(temp, ctext);
                writeWithFstream(path, ctext);
        }
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
        if(-1 == retVal)
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
        if(-1 == retVal)
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
        std::string filePath = getRelPath(path);
        auto file = std::make_unique<BasicFile>(filePath, keyFile, ivFile);
        retVal = file->read(buf, size, offset);
        // std::vector<byte> rtext;

        // readBlocks(path, rtext, size, offset);
        
        // memcpy(buf, (char*)rtext.data(), rtext.size());
        retVal = strlen(buf);
        if(-1 == retVal)
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
        // std::vector<byte> ptext(buf, buf + size);
        // writeBlocks(path, ptext);

        return size;
    }

    int efs_release(const char *path, struct fuse_file_info *fi)
    {
        int retVal = close(fi->fh);
        if(-1 == retVal)
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
        if(-1 == retVal)
        {
            return -errno;
        }

        return retVal; 
    }

    int efs_truncate(const char *path, off_t newsize)
    {
        const char *fullPath = getRelPath(path);
        int retVal = truncate(fullPath, newsize);
        if(-1 == retVal)
        {
            return -errno; 
        }
        return retVal;
    }

    int efs_utime(const char *path, struct utimbuf *ubuf)
    {
        const char *fullPath = getRelPath(path);
        int retVal = utime(fullPath, ubuf);
        if(-1 == retVal)
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
        if(-1 == retVal)
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

        if(-1 == retVal)
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
        if(-1 == retVal)
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
        if(-1 == retVal)
        {
            return -errno; 
        }
        return 0;
    }

    int efs_fsync(const char *path, int datasync, struct fuse_file_info *fi)
    {
        int retVal = fsync(fi->fh);
        if(-1 == retVal)
        {
            return -errno;
        }
        return retVal;
    }

}
