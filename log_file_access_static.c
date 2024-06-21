#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

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
