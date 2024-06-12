```shell
wget https://raw.githubusercontent.com/coreutils/coreutils/master/src/ls.c
wget https://raw.githubusercontent.com/coreutils/coreutils/master/src/cat.c
```

# Virtual and LD_PRELOAD readonly file system (reading ISO / TAR / ZIP)
- LD_PRELOAD FS that could work with package zip archvies (CTAN? ftp://tug.org/texlive/Contents/live/texmf-dist/)
- LD_PRELOAD FS that could work with texlive iso files containing zip archives
- virtual FS for reading ISO/TAR/ZIP texlive TDS
- virtual FS for reading Perl scripts
- virtual FS for embedding small TDS into binaries

# Links
- https://kevinpt.github.io/evfs/
- https://github.com/erincandescent/lib9660/blob/master/tb9660.c
- https://github.com/jacereda/fsatrace
- https://github.com/mikix/deb2snap/blob/master/src/preload.c#L84
- https://lists.ubuntu.com/archives/snappy-devel/2015-February/000282.html
- https://www.tomaz.me/2014/01/08/detecting-which-process-is-creating-a-file-using-ld-preload-trick.html
- https://github.com/AppImage/AppImageKit/issues/267
- https://avf.sourceforge.net/
- https://gist.github.com/przemoc/571086
- https://adared.ch/unionfs_by_intercept/ https://adared.ch/wp-content/uploads/2012/01/unionfs_by_intercept.c
- https://arxiv.org/abs/1908.10740
- http://ordiluc.net/fs/libetc/
- https://pages.stern.nyu.edu/~marriaga/software/libtrash/
- https://www.madlab.it/old-school/codes/uml.c
- https://github.com/pytorch/pytorch/pull/123002/files - self-mmap
- https://stackoverflow.com/questions/20124898/how-to-get-the-parameter-list-and-their-type-of-a-system-call
- https://github.com/strace/strace/blob/master/src/linux/x86_64/syscallent.h
- https://github.com/torvalds/linux/blob/master/include/linux/syscalls.h

# Useful ones
- https://github.com/fritzw/ld-preload-open/blob/master/path-mapping.c

# Syscalls
```
wget https://raw.githubusercontent.com/torvalds/linux/master/include/linux/syscalls.h
grep 'path\|filename' syscalls.h
```

```shell
asmlinkage long sys_setxattr(const char __user *path, const char __user *name,
asmlinkage long sys_lsetxattr(const char __user *path, const char __user *name,
asmlinkage long sys_getxattr(const char __user *path, const char __user *name,
asmlinkage long sys_lgetxattr(const char __user *path, const char __user *name,
asmlinkage long sys_listxattr(const char __user *path, char __user *list,
asmlinkage long sys_llistxattr(const char __user *path, char __user *list,
asmlinkage long sys_removexattr(const char __user *path,
asmlinkage long sys_lremovexattr(const char __user *path,
asmlinkage long sys_inotify_add_watch(int fd, const char __user *path,
asmlinkage long sys_mknodat(int dfd, const char __user * filename, umode_t mode,
asmlinkage long sys_mkdirat(int dfd, const char __user * pathname, umode_t mode);
asmlinkage long sys_unlinkat(int dfd, const char __user * pathname, int flag);
asmlinkage long sys_statfs(const char __user * path,
asmlinkage long sys_statfs64(const char __user *path, size_t sz,
asmlinkage long sys_truncate(const char __user *path, long length);
asmlinkage long sys_truncate64(const char __user *path, loff_t length);
asmlinkage long sys_faccessat(int dfd, const char __user *filename, int mode);
asmlinkage long sys_faccessat2(int dfd, const char __user *filename, int mode,
asmlinkage long sys_chdir(const char __user *filename);
asmlinkage long sys_chroot(const char __user *filename);
asmlinkage long sys_fchmodat(int dfd, const char __user *filename,
asmlinkage long sys_fchmodat2(int dfd, const char __user *filename,
asmlinkage long sys_fchownat(int dfd, const char __user *filename, uid_t user,
asmlinkage long sys_openat(int dfd, const char __user *filename, int flags,
asmlinkage long sys_openat2(int dfd, const char __user *filename,
asmlinkage long sys_readlinkat(int dfd, const char __user *path, char __user *buf,
asmlinkage long sys_newfstatat(int dfd, const char __user *filename,
asmlinkage long sys_fstatat64(int dfd, const char __user *filename,
asmlinkage long sys_utimensat(int dfd, const char __user *filename,
                                const char __user *filename,
asmlinkage long sys_execve(const char __user *filename,
                                  const char  __user *pathname);
asmlinkage long sys_execveat(int dfd, const char __user *filename,
asmlinkage long sys_statx(int dfd, const char __user *path, unsigned flags,
asmlinkage long sys_open_tree(int dfd, const char __user *path, unsigned flags);
asmlinkage long sys_move_mount(int from_dfd, const char __user *from_path,
                               int to_dfd, const char __user *to_path,
asmlinkage long sys_mount_setattr(int dfd, const char __user *path,
asmlinkage long sys_fspick(int dfd, const char __user *path, unsigned int flags);
asmlinkage long sys_open(const char __user *filename,
asmlinkage long sys_unlink(const char __user *pathname);
asmlinkage long sys_mknod(const char __user *filename, umode_t mode,
asmlinkage long sys_chmod(const char __user *filename, umode_t mode);
asmlinkage long sys_chown(const char __user *filename,
asmlinkage long sys_mkdir(const char __user *pathname, umode_t mode);
asmlinkage long sys_rmdir(const char __user *pathname);
asmlinkage long sys_lchown(const char __user *filename,
asmlinkage long sys_access(const char __user *filename, int mode);
asmlinkage long sys_stat64(const char __user *filename,
asmlinkage long sys_lstat64(const char __user *filename,
asmlinkage long sys_newstat(const char __user *filename,
asmlinkage long sys_newlstat(const char __user *filename,
asmlinkage long sys_utime(char __user *filename,
asmlinkage long sys_utimes(char __user *filename,
asmlinkage long sys_futimesat(int dfd, const char __user *filename,
                                     const char __user *filename,
asmlinkage long sys_utime32(const char __user *filename,
asmlinkage long sys_utimes_time32(const char __user *filename,
asmlinkage long sys_creat(const char __user *pathname, umode_t mode);
asmlinkage long sys_chown16(const char __user *filename,
asmlinkage long sys_lchown16(const char __user *filename,
asmlinkage long sys_stat(const char __user *filename,
asmlinkage long sys_lstat(const char __user *filename,
asmlinkage long sys_readlink(const char __user *path,
extern int do_fchownat(int dfd, const char __user *filename, uid_t user,
static inline long ksys_chown(const char __user *filename, uid_t user,
        return do_fchownat(AT_FDCWD, filename, user, group, 0);
static inline long ksys_lchown(const char __user *filename, uid_t user,
        return do_fchownat(AT_FDCWD, filename, user, group,
extern long do_sys_truncate(const char __user *pathname, loff_t length);
static inline long ksys_truncate(const char __user *pathname, loff_t length)
        return do_sys_truncate(pathname, length);
```
