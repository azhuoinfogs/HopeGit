#include <fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>
#include <libgen.h>
#ifdef __cplusplus
extern "C" {
#endif
extern int ufs_getattr (const char *path, struct stat *statbuf);
extern int ufs_readlink(const char *path, char *buf, size_t size);
extern int ufs_getdir(const char *path, fuse_dirh_t dir, fuse_dirfil_t filler);
extern int ufs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
extern int ufs_mknod(const char *path, mode_t mode, dev_t rdev);
extern int ufs_mkdir(const char *path, mode_t mode);
extern int ufs_unlink(const char *path);
extern int ufs_rmdir(const char *path);
extern int ufs_symlink(const char *from, const char *to);
extern int ufs_rename(const char *from, const char *to);
extern int ufs_link(const char *from, const char *to);
extern int ufs_chmod(const char *path, mode_t mode);
extern int ufs_chown(const char *path, uid_t uid, gid_t gid);
extern int ufs_utimens(const char *path, const struct timespec ts[2]);
extern int ufs_truncate(const char *path, off_t size);
extern int ufs_open(const char *path, struct fuse_file_info *fi);
extern int ufs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
extern int ufs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
extern int ufs_release(const char *path, struct fuse_file_info *fi);

extern int ufs_create();
extern int ufs_destroy();
#ifdef __cplusplus
}
#endif