#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <assert.h>
#include <sys/stat.h>
//#include <sys/memfd.h>
#include <sys/mman.h>

#include "packfs.h"

// TODO: impl open with temp unpack

static const char packfs_prefix[] = "dist-native/";

FILE* fopen(const char *path, const char *mode)
{
    typedef FILE* (*orig_fopen_func_type)(const char *path, const char *mode);
    orig_fopen_func_type orig_func = (orig_fopen_func_type)dlsym(RTLD_NEXT, "fopen");
    
    FILE* res = NULL;
    for(int i = 0; i < packfsfilesnum; i++)
    {
        if(0 == strcmp(path, packfsinfos[i].path))
        {
            FILE* res = fmemopen((void*)packfsinfos[i].start, (size_t)(packfsinfos[i].end - packfsinfos[i].start), mode);
            fprintf(stderr, "log_file_access_preload: Fopen(\"%s\", \"%s\") == %p\n", path, mode, (void*)res);
            return res;
        }
    }
    
    res = orig_func(path, mode);
    fprintf(stderr, "log_file_access_preload: fopen(\"%s\", \"%s\") == %p\n", path, mode, (void*)res);
    return res;
}

int fileno(FILE *stream)
{
    typedef int (*orig_func_type)(FILE* stream);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "fileno");
    
    if(!stream) return -1;
    
    int res = orig_func(stream);
    fprintf(stderr, "log_file_access_preload: fileno(%p) == %d\n", (void*)stream, res);
    
    if(res < 0)
    {
        char buf[1024];
        sprintf(buf, ".tmp_%p.bin", (void*)stream);
        FILE* f = fopen(buf, "w+");
        if(!f) return -1;
        do
        {
            fwrite(buf, 1, fread(buf, 1, sizeof(buf), stream), f);
        }
        while(!feof(stream) && !ferror(stream));
        fseek(f, 0, SEEK_SET);

        res = orig_func(f);
        fprintf(stderr, "log_file_access_preload: Fileno(%p) == %d\n", (void*)stream, res);
    }
    
    return res;
}


int open(const char *path, int flags)
{
    typedef int (*orig_func_type)(const char *path, int flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "open");

    char* _path = (char*)path;
    if(strlen(path) > 2 && _path[0] == '.' && _path[1] == '/')
        _path += 2;

    int res = -1;
    if(strncmp(packfs_prefix, _path, strlen(packfs_prefix)) == 0)
    {
        for(int i = 0; i < packfsfilesnum; i++)
        {
            if(0 == strcmp(_path, packfsinfos[i].path))
            {
                res = memfd_create("", 0);
                write(res, packfsinfos[i].start, (size_t)(packfsinfos[i].end - packfsinfos[i].start));
                lseek(res, 0, SEEK_SET);
                /*
                char buf[1024];
                sprintf(buf, ".tmp_%s.bin", packfsinfos[i].safe_path);
                FILE* f = fopen(buf, "w+");
                fwrite(packfsinfos[i].start, 1, (size_t)(packfsinfos[i].end - packfsinfos[i].start), f);
                fseek(f, 0, SEEK_SET);
                int res = fileno(f);
                fprintf(stderr, "log_file_access_preload: open(\"%s\", %d) == %d\n", path, flags, res);
                return res;
                */
            }
        }
        return -1;
    }
    
    res = orig_func(path, flags);
    fprintf(stderr, "log_file_access_preload: open(\"%s\", %d) == %d\n", path, flags, res);
    return res;
}


int access(const char *path, int flags) 
{
    typedef int (*orig_func_type)(const char *path, int flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "access");
   
    char* _path = (char*)path;
    if(strlen(path) > 2 && _path[0] == '.' && _path[1] == '/')
        _path += 2;

    if(strncmp(packfs_prefix, _path, strlen(packfs_prefix)) == 0)
    {
        assert(flags == R_OK);
        for(int i = 0; i < packfsfilesnum; i++)
        {
            if(0 == strcmp(_path, packfsinfos[i].path))
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


int stat(const char *restrict path, struct stat *restrict statbuf)
{
    typedef int (*orig_func_type)(const char *restrict path, struct stat *restrict statbuf);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "stat");
    
    char* _path = (char*)path;
    if(strlen(path) > 2 && _path[0] == '.' && _path[1] == '/')
        _path += 2;
    
    if(strncmp(packfs_prefix, _path, strlen(packfs_prefix)) == 0)
    {
        for(int i = 0; i < packfsfilesnum; i++)
        {
            if(0 == strcmp(_path, packfsinfos[i].path))
            {
                *statbuf = (struct stat){0};
                statbuf->st_size = (off_t)(packfsinfos[i].end - packfsinfos[i].start);
                statbuf->st_mode = S_IFREG;
                fprintf(stderr, "log_file_access_preload: Stat(\"%s\", %p) == 0\n", path, (void*)statbuf);
                return 0;
            }
        }
        for(int i = 0; i < packfsdirsnum; i++)
        {
            if(0 == strcmp(_path, packfsdirs[i]))
            {
                *statbuf = (struct stat){0};
                statbuf->st_mode = S_IFDIR;
                fprintf(stderr, "log_file_access_preload: Stat(\"%s\", %p) == 0\n", path, (void*)statbuf);
                return 0;
            }
        }
        fprintf(stderr, "log_file_access_preload: Stat(\"%s\", %p) == -1\n", path, (void*)statbuf);
        return -1;
    }

    int res = orig_func(path, statbuf);
    fprintf(stderr, "log_file_access_preload: stat(\"%s\", %p) == %d\n", path, (void*)statbuf, res);
    return res;
}
