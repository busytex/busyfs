#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "packfs.h"

// https://en.cppreference.com/w/cpp/io/c

#define packfs_prefix "dist-native/"
#define packfs_filefd_min 1000000000
#define packfs_filefd_max 1000001000
#define packfs_filefd_cnt (packfs_filefd_max - packfs_filefd_min)

int packfs_filecnt;
int packfs_filefd[packfs_filefd_cnt];
FILE* packfs_fileptr[packfs_filefd_cnt];

int packfs_open(char* path, FILE** out)
{
    char* _path = (char*)path;
    if(strlen(path) > 2 && _path[0] == '.' && _path[1] == '/')
        _path += 2;
    
    if(strncmp(packfs_prefix, _path, strlen(packfs_prefix)) != 0)
        return -1

    for(int i = 0; i < packfsfilesnum; i++)
    {
        if(0 == strcmp(_path, packfsinfos[i].path))
        {
            FILE* res = fmemopen((void*)packfsinfos[i].start, (size_t)(packfsinfos[i].end - packfsinfos[i].start), mode);
            
            for(int k = 0; k < packfs_filefd_max - packfs_filefd_min; k++)
            {
                if(packfs_filefd[k] == 0)
                {
                    packfs_filefd[k] = packfs_filefd_min + i;
                    packfs_fileptr[k] = res;
                    if(out ! = NULL)
                        *out = packfs_fileptr[k];
                    return packfs_filefd[k];
                }
            }
        }
    }

    return -1;
}

int packfs_close(int fd)
{
    if(fd < packfs_filefd_min || fd >= packfs_filefd_max)
        return -1;

    for(int k = 0; k < packfs_filefd_max - packfs_filefd_min; k++)
    {
        if(packfs_filefd[k] == fd)
        {
            packfs_filefd[k] = 0;
            fclose(packfs_fileptr[k]);
            packfs_fileptr[k] = NULL;
            return 0;
        }
    }
    return -1;
}

FILE* packfs_findptr(int fd)
{
    if(fd < packfs_filefd_min || fd >= packfs_filefd_max)
        return NULL;
    
    for(int k = 0; k < packfs_filefd_max - packfs_filefd_min; k++)
    {
        if(packfs_filefd[k] == fd)
            return packfs_fileptr[k];
    }
    return NULL;
}

int packfs_findfd(FILE* ptr)
{
    for(int k = 0; k < packfs_filefd_max - packfs_filefd_min; k++)
    {
        if(packfs_fileptr[k] == ptr)
            return packfs_filefd[k];
    }
    return -1;
}

ssize_t packfs_read(int fd, void* buf, size_t count)
{
    FILE* ptr = packfs_findptr(fd);
    if(!ptr)
        return -1;
    return (ssize_t)fread(buf, 1, count, ptr);
}

int packfs_seek(int fd, long offset, int whence)
{
    FILE* ptr = packfs_findptr(fd);
    if(!ptr)
        return -1;
    return fseek(fd, offset, whence);
}

int packfs_access(const char* path)
{
    char* _path = (char*)path;
    if(strlen(path) > 2 && _path[0] == '.' && _path[1] == '/')
        _path += 2;

    if(strncmp(packfs_prefix, _path, strlen(packfs_prefix)) == 0)
    {
        for(int i = 0; i < packfsfilesnum; i++)
        {
            if(0 == strcmp(_path, packfsinfos[i].path))
            {
                return 0;
            }
        }
        return -1;
    }
    return -2;
}

int packfs_stat(const char* path, struct stat *restrict statbuf)
{
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
                return 0;
            }
        }
        for(int i = 0; i < packfsdirsnum; i++)
        {
            if(0 == strcmp(_path, packfsdirs[i]))
            {
                *statbuf = (struct stat){0};
                statbuf->st_mode = S_IFDIR;
                return 0;
            }
        }
        return -1;
    }
    
    return -2;
}

int packfs_mirror(FILE* stream, const char* start, const char* end)
{
    int res = -1;
    if(stream == NULL)
    {
        res = memfd_create("", 0);
        write(res, start, (size_t)(end - start));
        lseek(res, 0, SEEK_SET);
        /*
        char buf[1024];
        sprintf(buf, ".tmp_%s.bin", packfsinfos[i].safe_path);
        FILE* f = fopen(buf, "w+");
        fwrite(packfsinfos[i].start, 1, (size_t)(packfsinfos[i].end - packfsinfos[i].start), f);
        fseek(f, 0, SEEK_SET);
        int res = fileno(f);
        fprintf(stderr, "packfs: open(\"%s\", %d) == %d\n", path, flags, res);
        */
    }
    else
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
        res = fileno(f);
    }
    return res;
}


FILE* fopen(const char *path, const char *mode)
{
    typedef FILE* (*orig_fopen_func_type)(const char *path, const char *mode);
    orig_fopen_func_type orig_func = (orig_fopen_func_type)dlsym(RTLD_NEXT, "fopen");
    
    FILE* res = NULL;
    if(packfs_open(path, &res) >= 0)
    {
        fprintf(stderr, "packfs: Fopen(\"%s\", \"%s\") == %p\n", path, mode, (void*)res);
        return res;
    }

    res = orig_func(path, mode);
    fprintf(stderr, "packfs: fopen(\"%s\", \"%s\") == %p\n", path, mode, (void*)res);
    return res;
}

int fileno(FILE *stream)
{
    typedef int (*orig_func_type)(FILE* stream);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "fileno");
    
    if(!stream) return -1;
    
    int res = orig_func(stream);
    fprintf(stderr, "packfs: fileno(%p) == %d\n", (void*)stream, res);
    
    if(res < 0)
    {        
        res = packfs_findfd(stream);
        fprintf(stderr, "packfs: Fileno(%p) == %d\n", (void*)stream, res);
    }
    
    return res;
}

int open(const char *path, int flags)
{
    typedef int (*orig_func_type)(const char *path, int flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "open");

    int res = packfs_open(path, &res);
    if(res >= 0)
    {
        fprintf(stderr, "packfs: Open(\"%s\", %d) == %d\n", path, flags, res);
        return res;
    }
    
    res = orig_func(path, flags);
    fprintf(stderr, "packfs: open(\"%s\", %d) == %d\n", path, flags, res);
    return res;
}

// https://en.cppreference.com/w/c/io/fread
ssize_t read(int fd, void* buf, size_t count)
{
    typedef ssize_t (*orig_func_type)(int fd, void* buf, size_t count);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "read");

    ssize_t res = packfs_read(fd, buf, count);
    if(res >= 0)
    {
        fprintf(stderr, "packfs: Read(%d, %p, %zu) == %d\n", fd, buf, count, (int)res);
        return res;
    }

    res = orig_func(fd, buf, count);
    fprintf(stderr, "packfs: read(%d, %p, %zu) == %d\n", fd, buf, count, (int)res);
    return res;
}

// https://en.cppreference.com/w/c/io/fseek
off_t lseek(int fd, off_t offset, int whence)
{
    typedef off_t (*orig_func_type)(int fd, off_t offset, int whence);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "lseek");
    
    int res = packfs_seek(fd, (long)offset, whence);
    if(res >= 0)
    {
        fprintf(stderr, "packfs: Seek(%d, %d, %d) == %d\n", fd, (int)offset, whence, (int)res);
        return res;
    }

    res = orig_func(fd, buf, count);
    fprintf(stderr, "packfs: seek(%d, %d, %d) == %d\n", fd, (int)offset, whence, (int)res);
    return res;
}


int access(const char *path, int flags) 
{
    typedef int (*orig_func_type)(const char *path, int flags);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "access");
   
    int res = packfs_access(path);
    if(res >= -1)
    {
        fprintf(stderr, "packfs: Access(\"%s\", %d) == 0\n", path, flags);
        return res;
    }
    
    res = orig_func(path, flags); 
    fprintf(stderr, "packfs: access(\"%s\", %d) == %d\n", path, flags, res);
    return res;
}

int stat(const char *restrict path, struct stat *restrict statbuf)
{
    typedef int (*orig_func_type)(const char *restrict path, struct stat *restrict statbuf);
    orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "stat");
    
    int res = packfs_stat(path);
    if(res >= -1)
    {
        fprintf(stderr, "packfs: Stat(\"%s\", %p) == -1\n", path, (void*)statbuf);
        return res;
    }

    res = orig_func(path, statbuf);
    fprintf(stderr, "packfs: stat(\"%s\", %p) == %d\n", path, (void*)statbuf, res);
    return res;
}
