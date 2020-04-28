/**
 * Encryptedfs entry point
 * FS operations are assigned to fuse_operations
 *
 */

#include <memory>
#include "fsutils.h"
#include "encryptedfs.h"
#include <iostream>

//Print usage of a fs
void usage()
{
    //print fs usage
}

struct fuse_operations fops = {
    .getattr = encryptedfs::efs_getattr,
    .mknod = encryptedfs::efs_mknod,
    .mkdir = encryptedfs::efs_mkdir,
    .unlink = encryptedfs::efs_unlink,
    .rmdir = encryptedfs::efs_rmdir,
    .rename = encryptedfs::efs_rename,
    .truncate = encryptedfs::efs_truncate,
    //.utime = encryptedfs::efs_utime,
    .open = encryptedfs::efs_open,
    .read = encryptedfs::efs_read,
    .write = encryptedfs::efs_write,
    .release = encryptedfs::efs_release,
    .fsync = encryptedfs::efs_fsync,
    .readdir = encryptedfs::efs_readdir,
    .init = encryptedfs::efs_init,
    .access = encryptedfs::efs_access,
    .utimens = encryptedfs::efs_utimens
};


int main(int argc, char **argv)
{
    int ret;
    auto fsInfo = std::make_shared<FsInfo>();
    fsInfo->init(argc, argv);

    std::cout<<"hi mark \n";
    ret = fuse_main(argc, argv, &fops, (void *)fsInfo.get());


    return ret;
}
