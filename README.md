```shell
#python packfs.py -i dist-native -o packfs.h
cc -shared -fPIC packfs.c -o packfs.so -ldl # @packfs.h.txt
```

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
- http://devlib.symbian.slions.net/s3/GUID-A4ECB450-02DA-5E85-91AC-4C8BDE0B9B58.html
- https://stackoverflow.com/questions/78694331/intercept-open-syscall-in-c-when-called-via-fopen
- https://man7.org/linux/man-pages/man2/memfd_create.2.html
- https://stackoverflow.com/questions/55704758/is-there-anything-like-shm-open-without-filename
- https://www.reddit.com/r/kernel/comments/7wma5q/simulating_memfd_create_in_26/
- https://github.com/jart/cosmopolitan/blob/13b9ecd5379ee32448f9f05dbc1b7c5f7f68b72b/libc/runtime/zipos-open.c
- https://github.com/guardianproject/libsqlfs
- https://nanovms.com/dev/tutorials/introducing-memfd_create-anonymous-files-for-nanos-unikernel

# Useful ones
- https://github.com/fritzw/ld-preload-open/blob/master/path-mapping.c
- https://git.musl-libc.org/cgit/musl/tree/src/stdio/fmemopen.c
- https://stackoverflow.com/questions/5053664/dlopen-from-memory
- https://github.com/m1m1x/memdlopen
- https://sourceware.org/bugzilla/show_bug.cgi?id=11767
- https://sourceware.org/bugzilla/show_bug.cgi?id=30100
- https://0x00sec.org/t/super-stealthy-droppers/3715
- https://0x00sec.org/t/running-binaries-without-leaving-tracks/2166
- https://hackage.haskell.org/package/memfd
- https://github.com/a-darwish/memfd-examples
- https://github.com/m1m1x/memdlopen/blob/master/main.c
 

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
asmlinkage long sys_utimensat(int dfd, const char __user *filename, const char __user *filename,
asmlinkage long sys_execve(const char __user *filename, const char  __user *pathname);
asmlinkage long sys_execveat(int dfd, const char __user *filename,
asmlinkage long sys_statx(int dfd, const char __user *path, unsigned flags,
asmlinkage long sys_open_tree(int dfd, const char __user *path, unsigned flags);
asmlinkage long sys_move_mount(int from_dfd, const char __user *from_path, int to_dfd, const char __user *to_path,
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
asmlinkage long sys_futimesat(int dfd, const char __user *filename, const char __user *filename,
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
static inline long ksys_lchown(const char __user *filename, uid_t user,
extern long do_sys_truncate(const char __user *pathname, loff_t length);
static inline long ksys_truncate(const char __user *pathname, loff_t length)
```

# More
```
asmlinkage long sys_setxattr(const char __user *path, const char __user *name,                       const void __user *value, size_t size, int flags);
asmlinkage long sys_lsetxattr(const char __user *path, const char __user *name,                       const void __user *value, size_t size, int flags);
asmlinkage long sys_fsetxattr(int fd, const char __user *name,                        const void __user *value, size_t size, int flags);
asmlinkage long sys_getxattr(const char __user *path, const char __user *name,                       void __user *value, size_t size);
asmlinkage long sys_lgetxattr(const char __user *path, const char __user *name,                       void __user *value, size_t size);
asmlinkage long sys_fgetxattr(int fd, const char __user *name,                        void __user *value, size_t size);
asmlinkage long sys_listxattr(const char __user *path, char __user *list,                             size_t size);
asmlinkage long sys_llistxattr(const char __user *path, char __user *list,                             size_t size);
asmlinkage long sys_removexattr(const char __user *path,                                const char __user *name);
asmlinkage long sys_lremovexattr(const char __user *path,                                const char __user *name);
asmlinkage long sys_fremovexattr(int fd, const char __user *name);
asmlinkage long sys_inotify_add_watch(int fd, const char __user *path,                                  u32 mask);
asmlinkage long sys_mknodat(int dfd, const char __user * filename, umode_t mode,                            unsigned dev);
asmlinkage long sys_mkdirat(int dfd, const char __user * pathname, umode_t mode);
asmlinkage long sys_unlinkat(int dfd, const char __user * pathname, int flag);
asmlinkage long sys_symlinkat(const char __user * oldname,                            int newdfd, const char __user * newname);
asmlinkage long sys_linkat(int olddfd, const char __user *oldname,                         int newdfd, const char __user *newname, int flags);
asmlinkage long sys_renameat(int olddfd, const char __user * oldname,                        int newdfd, const char __user * newname);
asmlinkage long sys_umount(char __user *name, int flags);
asmlinkage long sys_mount(char __user *dev_name, char __user *dir_name,                         char __user *type, unsigned long flags,                    void __user *data);
asmlinkage long sys_statfs(const char __user * path,                            struct statfs __user *buf);
asmlinkage long sys_statfs64(const char __user *path, size_t sz,                                struct statfs64 __user *buf);
asmlinkage long sys_truncate(const char __user *path, long length);
asmlinkage long sys_truncate64(const char __user *path, loff_t length);
asmlinkage long sys_faccessat(int dfd, const char __user *filename, int mode);
asmlinkage long sys_faccessat2(int dfd, const char __user *filename, int mode,                         int flags);
asmlinkage long sys_chdir(const char __user *filename);
asmlinkage long sys_chroot(const char __user *filename);
asmlinkage long sys_fchmodat(int dfd, const char __user *filename,                           umode_t mode);
asmlinkage long sys_fchmodat2(int dfd, const char __user *filename,                          umode_t mode, unsigned int flags);
asmlinkage long sys_fchownat(int dfd, const char __user *filename, uid_t user,                       gid_t group, int flag);
asmlinkage long sys_openat(int dfd, const char __user *filename, int flags,                        umode_t mode);
asmlinkage long sys_openat2(int dfd, const char __user *filename,                           struct open_how *how, size_t size);
asmlinkage long sys_sendfile64(int out_fd, int in_fd,                          loff_t __user *offset, size_t count);
asmlinkage long sys_readlinkat(int dfd, const char __user *path, char __user *buf,                             int bufsiz);
asmlinkage long sys_newfstatat(int dfd, const char __user *filename,                           struct stat __user *statbuf, int flag);
asmlinkage long sys_fstatat64(int dfd, const char __user *filename,                            struct stat64 __user *statbuf, int flag);
asmlinkage long sys_sync_file_range2(int fd, unsigned int flags,                                     loff_t offset, loff_t nbytes);
asmlinkage long sys_sync_file_range(int fd, loff_t offset, loff_t nbytes,                                       unsigned int flags);
asmlinkage long sys_utimensat(int dfd, const char __user *filename,                             struct __kernel_timespec __user *utimes,                   int flags);
asmlinkage long sys_utimensat_time32(unsigned int dfd,                          const char __user *filename,                            struct old_timespec32 __user *t, int flags);
asmlinkage long sys_acct(const char __user *name);
asmlinkage long sys_delete_module(const char __user *name_user,                         unsigned int flags);
asmlinkage long sys_newuname(struct new_utsname __user *name);
asmlinkage long sys_sethostname(char __user *name, int len);
asmlinkage long sys_setdomainname(char __user *name, int len);
asmlinkage long sys_mq_open(const char __user *name, int oflag, umode_t mode, struct mq_attr __user *attr);
asmlinkage long sys_mq_unlink(const char __user *name);
asmlinkage long sys_getsockname(int, struct sockaddr __user *, int __user *);
asmlinkage long sys_getpeername(int, struct sockaddr __user *, int __user *);
asmlinkage long sys_setsockopt(int fd, int level, int optname,                          char __user *optval, int optlen);
asmlinkage long sys_getsockopt(int fd, int level, int optname,                          char __user *optval, int __user *optlen);
asmlinkage long sys_execve(const char __user *filename,         const char __user *const __user *argv,          const char __user *const __user *envp);
asmlinkage long sys_swapon(const char __user *specialfile, int swap_flags);
asmlinkage long sys_swapoff(const char __user *specialfile);
asmlinkage long sys_remap_file_pages(unsigned long start, unsigned long size,                   unsigned long prot, unsigned long pgoff,                   unsigned long flags);
asmlinkage long sys_fanotify_mark(int fanotify_fd, unsigned int flags,                            u64 mask, int fd,                               const char  __user *pathname);
asmlinkage long sys_name_to_handle_at(int dfd, const char __user *name,                               struct file_handle __user *handle,
     int __user *mnt_id, int flag);
asmlinkage long sys_open_by_handle_at(int mountdirfd,                                 struct file_handle __user *handle,
  int flags);
asmlinkage long sys_renameat2(int olddfd, const char __user *oldname,                         int newdfd, const char __user *newname,
  unsigned int flags);
asmlinkage long sys_memfd_create(const char __user *uname_ptr, unsigned int flags);
asmlinkage long sys_execveat(int dfd, const char __user *filename,                      const char __user *const __user *argv,                  const char __user *const __user *envp, int flags);
asmlinkage long sys_copy_file_range(int fd_in, loff_t __user *off_in,                               int fd_out, loff_t __user *off_out,
   size_t len, unsigned int flags);
asmlinkage long sys_statx(int dfd, const char __user *path, unsigned flags,                       unsigned mask, struct statx __user *buffer);
asmlinkage long sys_open_tree(int dfd, const char __user *path, unsigned flags);
asmlinkage long sys_move_mount(int from_dfd, const char __user *from_path,                             int to_dfd, const char __user *to_path,
      unsigned int ms_flags);
asmlinkage long sys_mount_setattr(int dfd, const char __user *path,                               unsigned int flags,                             struct mount_attr __user *uattr, size_t usize);
asmlinkage long sys_fsopen(const char __user *fs_name, unsigned int flags);
asmlinkage long sys_fspick(int dfd, const char __user *path, unsigned int flags);
asmlinkage long sys_spu_create(const char __user *name,         unsigned int flags, umode_t mode, int fd);
asmlinkage long sys_open(const char __user *filename,                           int flags, umode_t mode);
asmlinkage long sys_link(const char __user *oldname,                            const char __user *newname);
asmlinkage long sys_unlink(const char __user *pathname);
asmlinkage long sys_mknod(const char __user *filename, umode_t mode,                            unsigned dev);
asmlinkage long sys_chmod(const char __user *filename, umode_t mode);
asmlinkage long sys_chown(const char __user *filename,                          uid_t user, gid_t group);
asmlinkage long sys_mkdir(const char __user *pathname, umode_t mode);
asmlinkage long sys_rmdir(const char __user *pathname);
asmlinkage long sys_lchown(const char __user *filename,                         uid_t user, gid_t group);
asmlinkage long sys_access(const char __user *filename, int mode);
asmlinkage long sys_rename(const char __user *oldname,                          const char __user *newname);
asmlinkage long sys_stat64(const char __user *filename,                         struct stat64 __user *statbuf);
asmlinkage long sys_lstat64(const char __user *filename,                                struct stat64 __user *statbuf);
asmlinkage long sys_sendfile(int out_fd, int in_fd,                          off_t __user *offset, size_t count);
asmlinkage long sys_newstat(const char __user *filename,                                struct stat __user *statbuf);
asmlinkage long sys_newlstat(const char __user *filename,                               struct stat __user *statbuf);
asmlinkage long sys_utime(char __user *filename,                                struct utimbuf __user *times);
asmlinkage long sys_utimes(char __user *filename,                               struct __kernel_old_timeval __user *utimes);
asmlinkage long sys_futimesat(int dfd, const char __user *filename,                           struct __kernel_old_timeval __user *utimes);
asmlinkage long sys_futimesat_time32(unsigned int dfd,                               const char __user *filename,                                    struct old_timeval32 __user *t);
asmlinkage long sys_utime32(const char __user *filename,                                 struct old_utimbuf32 __user *t);
asmlinkage long sys_utimes_time32(const char __user *filename,                            struct old_timeval32 __user *t);
asmlinkage long sys_creat(const char __user *pathname, umode_t mode);
asmlinkage long sys_oldumount(char __user *name);
asmlinkage long sys_kexec_file_load(int kernel_fd, int initrd_fd,                                   unsigned long cmdline_len,
const char __user *cmdline_ptr,                             unsigned long flags);
asmlinkage long sys_chown16(const char __user *filename,                                old_uid_t user, old_gid_t group);
asmlinkage long sys_lchown16(const char __user *filename,                               old_uid_t user, old_gid_t group);
asmlinkage long sys_stat(const char __user *filename,                   struct __old_kernel_stat __user *statbuf);
asmlinkage long sys_lstat(const char __user *filename,                  struct __old_kernel_stat __user *statbuf);
asmlinkage long sys_readlink(const char __user *path,                           char __user *buf, int bufsiz);
asmlinkage long sys_gethostname(char __user *name, int len);
asmlinkage long sys_uname(struct old_utsname __user *);
asmlinkage long sys_olduname(struct oldold_utsname __user *);
int ksys_sync_file_range(int fd, loff_t offset, loff_t nbytes,                   unsigned int flags);
extern int do_fchownat(int dfd, const char __user *filename, uid_t user,                       gid_t group, int flag);
static inline long ksys_chown(const char __user *filename, uid_t user,                        gid_t group)
        return do_fchownat(AT_FDCWD, filename, user, group, 0);
static inline long ksys_lchown(const char __user *filename, uid_t user,                        gid_t group)
        return do_fchownat(AT_FDCWD, filename, user, group,                          AT_SYMLINK_NOFOLLOW);
extern long do_sys_truncate(const char __user *pathname, loff_t length);
static inline long ksys_truncate(const char __user *pathname, loff_t length)
        return do_sys_truncate(pathname, length);
long __do_semtimedop(int semid, struct sembuf *tsems, unsigned int nsops,                    const struct timespec64 *timeout,               struct ipc_namspace *ns);
int __sys_getsockopt(int fd, int level, int optname, char __user *optval,               int __user *optlen);
```
