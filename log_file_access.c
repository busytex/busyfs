#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
//'access', 'lstat', 'fstat', 'stat', 'open', 'read', 'unlink', 'mmap',  
// 'readv', 'close', 'lseek', 'writev', 'fcntl', 'munmap', 'arch_prctl', 'brk', 'execve', 'exit_group', 'getcwd', 'getdents64', 'ioctl', 'rt_sigaction', 'rt_sigprocmask', 'set_tid_address'
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

#ifdef LOGFILEACCESSDYNAMIC
// gcc -shared -fPIC log_file_access.c -o log_file_access.so -ldl

#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>

FILE* fopen(const char *path, const char *mode)
{
    fprintf(stderr, "log_file_access_preload: fopen(\"%s\", \"%s\")\n", path, mode);

    typedef FILE* (*orig_fopen_func_type)(const char *path, const char *mode);
    orig_fopen_func_type orig_func = (orig_fopen_func_type)dlsym(RTLD_NEXT, "fopen");
    return orig_func(path, mode);
}

int open(const char *path, int flags)
{
    fprintf(stderr, "log_file_access_preload: open(\"%s\", %d)\n", path, flags);

    typedef int (*orig_open_func_type)(const char *pathname, int flags);
    orig_open_func_type orig_func = (orig_open_func_type)dlsym(RTLD_NEXT, "open");
    return orig_func(path, flags);
}

int open64(const char *path, int flags)
{
    fprintf(stderr, "log_file_access_preload: open64(\"%s\", %d)\n", path, flags);

    typedef int (*orig_open_func_type)(const char *pathname, int flags);
    orig_open_func_type orig_func = (orig_open_func_type)dlsym(RTLD_NEXT, "open64");
    return orig_func(path, flags);
}

int openat(int dirfd, const char *path, int flags)
//int openat(int dirfd, const char *path, int flags, mode_t mode)
{
    fprintf(stderr, "log_file_access_preload: openat(%d, \"%s\", %d)\n", dirfd, path, flags);

    typedef int (*orig_open_func_type)(int dirfd, const char *pathname, int flags);
    orig_open_func_type orig_func = (orig_open_func_type)dlsym(RTLD_NEXT, "openat");
    return orig_func(dirfd, path, flags);
}

#endif
