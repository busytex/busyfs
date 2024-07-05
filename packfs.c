#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <assert.h>

#include "packfs.h"

FILE* fopen(const char *path, const char *mode)
{
    for(int i = 0; i < packfsfilesnum; i++)
    {
        if(0 == strcmp(path, packfsinfos[i].path))
        {
            fprintf(stderr, "log_file_access_fmemopen: fopen(\"%s\", \"%s\")\n", path, mode);
            FILE* stream = fmemopen((void*)packfsinfos[i].start, (size_t)(packfsinfos[i].end - packfsinfos[i].start), mode);
            fprintf(stderr, "log_file_access_fmemopen: fopen(\"%s\", \"%s\") == %d\n", path, mode, stream->_fileno);
            return stream;
        }
    }
    
    typedef FILE* (*orig_fopen_func_type)(const char *path, const char *mode);
    fprintf(stderr, "log_file_access_preload: fopen(\"%s\", \"%s\")\n", path, mode);
    orig_fopen_func_type orig_func = (orig_fopen_func_type)dlsym(RTLD_NEXT, "fopen");
    return orig_func(path, mode); 
}

int fileno(FILE *stream)
{
    if(!stream) return -1;

    typedef int (*orig_func_type)(FILE* stream);
    fprintf(stderr, "log_file_access_preload: fileno(%p)\n", (void*)stream);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "fileno");
    int res = orig_func(stream);
    
    fprintf(stderr, "log_file_access_preload: fileno(%p) == %d\n", (void*)stream, res);
    
    if(res < 0)
    {
        char buf[1024];
        sprintf(buf, "tmp%p.bin", (void*)stream);
        FILE* f = fopen(buf, "w+");
        if(!f) return -1;
        do
        {
            fwrite(buf, 1, fread(buf, 1, sizeof(buf), stream), f);
        }
        while(!feof(stream) && !ferror(stream));
        fseek(f, 0, SEEK_SET);

        res = orig_func(f);
        fprintf(stderr, "log_file_access_preload: Fileno(%p) -> %p\n", (void*)stream, (void*)f);
    }
    
    return res;
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
    int res = orig_func(dirfd, path, flags);
    fprintf(stderr, "log_file_access_preload: openat(%d, \"%s\", %d) == %d\n", dirfd, path, flags, res);
    return res;
}


int access(const char *path, int flags) 
{
    typedef int (*orig_func_type)(const char *pathname, int flags);
    fprintf(stderr, "log_file_access_preload: access(\"%s\", %d)\n", path, flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "access");
   
    const char prefix[] = "dist-native/";
    if(strncmp(prefix, path, strlen(prefix)) == 0)
    {
        assert(flags == R_OK);
        for(int i = 0; i < packfsfilesnum; i++)
        {
            if(0 == strcmp(path, packfsinfos[i].path))
            {
                fprintf(stderr, "log_file_access_preload: Access(\"%s\", %d) == 0\n", path, flags);
                return 0;
            }
        }
        fprintf(stderr, "log_file_access_preload: Access(\"%s\", %d) == -1\n", path, flags);
        return -1;
    }
    
    int res = orig_func(path, flags); 
    fprintf(stderr, "log_file_access_preload: access(\"%s\", %d) == %d\n", path, flags, res);
    return res;
}


int stat(const char *restrict pathname, struct stat *restrict statbuf)
{
    typedef int (*orig_func_type)(const char *restrict pathname, struct stat *restrict statbuf);
    fprintf(stderr, "log_file_access_preload: stat(\"%s\", %p)\n", pathname, (void*)statbuf);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "stat");
    
    const char prefix[] = "dist-native/";
    if(strncmp(prefix, path, strlen(prefix)) == 0)
    {
        for(int i = 0; i < packfsfilesnum; i++)
        {
            if(0 == strcmp(pathname, packfsinfos[i].path))
            {
                *statbuf = (struct stat){0};
                statbuf->st_size = (off_t)(packfsinfos[i].end - packfsinfos[i].start);
                statbuf->st_mode = S_IRUSR | S_IRGRP | S_IROTH;
                fprintf(stderr, "log_file_access_preload: Stat(\"%s\", %p) == 0\n", pathname, (void*)statbuf);
                return 0;
            }
        }
        fprintf(stderr, "log_file_access_preload: Stat(\"%s\", %p) == -1\n", pathname, (void*)statbuf);
        return -1;
    }

    int res = orig_func(pathname, statbuf);
    fprintf(stderr, "log_file_access_preload: stat(\"%s\", %p) == %d\n", pathname, (void*)statbuf, res);
    return res;
}
