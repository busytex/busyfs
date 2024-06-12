#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
//'access', 'lstat', 'fstat', 'stat', 'open', 'read', 'unlink', 'mmap',  
// 'readv', 'close', 'lseek', 'writev', 'fcntl', 'munmap', 'arch_prctl', 'brk', 'execve', 'exit_group', 'getcwd', 'getdents64', 'ioctl', 'rt_sigaction', 'rt_sigprocmask', 'set_tid_address'

#ifdef LOGFILEACCESSDYNAMIC
// gcc -shared -fPIC log_file_access.c -o log_file_access.so -ldl

#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/stat.h>

FILE* fopen(const char *path, const char *mode)
{
    typedef FILE* (*orig_fopen_func_type)(const char *path, const char *mode);
    fprintf(stderr, "log_file_access_preload: fopen(\"%s\", \"%s\")\n", path, mode);
    orig_fopen_func_type orig_func = (orig_fopen_func_type)dlsym(RTLD_NEXT, "fopen");
    return orig_func(path, mode);
}

int open(const char *path, int flags)
{
    typedef int (*orig_func_type)(const char *pathname, int flags);
    fprintf(stderr, "log_file_access_preload: open(\"%s\", %d)\n", path, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "open");
    return orig_func(path, flags);
}
int open64(const char *path, int flags)
{
    typedef int (*orig_func_type)(const char *pathname, int flags);
    fprintf(stderr, "log_file_access_preload: open64(\"%s\", %d)\n", path, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "open64");
    return orig_func(path, flags);
}
//int openat(int dirfd, const char *path, int flags, mode_t mode)
int openat(int dirfd, const char *path, int flags)
{
    typedef int (*orig_func_type)(int dirfd, const char *pathname, int flags);
    fprintf(stderr, "log_file_access_preload: openat(%d, \"%s\", %d)\n", dirfd, path, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "openat");
    return orig_func(dirfd, path, flags);
}

int access(const char *path, int flags)
{
    typedef int (*orig_func_type)(const char *pathname, int flags);
    fprintf(stderr, "log_file_access_preload: access(\"%s\", %d)\n", path, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "access");
    return orig_func(path, flags);
}
int faccessat(int dirfd, const char *path, int mode, int flags)
{
    typedef int (*orig_func_type)(int dirfd, const char *pathname, int mode, int flags);
    fprintf(stderr, "log_file_access_preload: faccessat(%d, \"%s\", %d, %d)\n", dirfd, path, mode, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "faccessat");
    return orig_func(dirfd, path, mode, flags);
}


int stat(const char *restrict pathname, struct stat *restrict statbuf)
{
    typedef int (*orig_func_type)(const char *restrict pathname, struct stat *restrict statbuf);
    fprintf(stderr, "log_file_access_preload: stat(\"%s\", %p)\n", pathname, (void*)statbuf);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "stat");
    return orig_func(pathname, statbuf);
}
int lstat(const char *restrict pathname, struct stat *restrict statbuf)
{
    typedef int (*orig_func_type)(const char *restrict pathname, struct stat *restrict statbuf);
    fprintf(stderr, "log_file_access_preload: lstat(\"%s\", %p)\n", pathname, (void*)statbuf);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "lstat");
    return orig_func(pathname, statbuf);
}
int fstat(int fd, struct stat *restrict statbuf)
{
    typedef int (*orig_func_type)(int fd, struct stat *restrict statbuf);
    fprintf(stderr, "log_file_access_preload: fstat(%d, %p)\n", fd, (void*)statbuf);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "fstat");
    return orig_func(fd, statbuf);
}
int fstatat(int dirfd, const char *restrict pathname, struct stat *restrict statbuf, int flags)
{
    typedef int (*orig_func_type)(int dirfd, const char *restrict pathname, struct stat *restrict statbuf, int flags);
    fprintf(stderr, "log_file_access_preload: fstat(%d, \"%s\", %p, %d)\n", dirfd, pathname, (void*)statbuf, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "fstatat");
    return orig_func(difd, pathname, statbuf, flags);
}
// 'lstat', 'fstat', 'stat', 'unlink', 'mmap', 'rename', 'link', 'rmdir', 'mkdir'

#endif




#ifdef LOGFILEACCESSSTATIC
FILE* orig_fopen(const char *path, const char *mode);
FILE* fopen(const char *path, const char *mode)
{
    fprintf(stderr, "log_file_access: fopen(\"%s\", \"%s\")\n", path, mode);
    return orig_fopen(path, mode);
}
int orig_open(const char *path, int mode);
int open(const char *path, int mode)
{
    fprintf(stderr, "log_file_access: open(\"%s\", %d)\n", path, mode);
    return orig_open(path, mode);
}
#endif
