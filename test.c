//#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
//#include <unistd.h>
//#include <errno.h>
//#include <dlfcn.h>
//#include <sys/stat.h>

/*
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
*/

int main()
{
    (void)fopen("test.txt", "r");
}
