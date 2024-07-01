import os
import argparse
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--input-path', '-i')
parser.add_argument('--output-path', '-o')
args = parser.parse_args()

assert os.path.exists(args.input_path) and os.path.isdir(args.input_path)

objects, files, dirs = [], [], []
for (dirpath, dirnames, filenames) in os.walk(args.input_path):
    dirs.extend(os.path.join(dirpath, dirname) for dirname in dirnames)
    for basename in filenames:
        if basename.endswith('.o'):
            continue
        p = os.path.join(dirpath, basename)
        files.append(p)
        objects.append(p + '.o')
        subprocess.check_call(['ld', '-r', '-b', 'binary', '-o', objects[-1], files[-1]])

translate = {ord('/') : '_', ord('.') : '_', ord('-') : '_'}
# problem: can produce the same symbol name because of this mapping

f = open(args.output_path + '.txt', 'w')
print('\n'.join(objects), file = f)

f = open(args.output_path, 'w')
print(f'''
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/stat.h>
int packfsfilesnum = {len(files)}, packfsdirsnum  = {len(dirs)};
''', file = f)

print('\n'.join(f'extern char _binary_{pp}_start[], _binary_{pp}_end[];' for p in files for pp in [p.translate(translate)]), file = f)
print('struct packfsinfo { const char *path; const char* start; const char* end; } packfsinfos[] = {', file = f)
for p in files:
    pp = p.translate(translate)
    print('{ "' + repr(p)[1:-1] + '", ', f'_binary_{pp}_start, ', f'_binary_{pp}_end ', '},', file = f)
print('};', file = f)
print('\nconst char* packfsdirs[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in dirs) + '\n};\n\n', file = f)
#print('\nconst char* packfsfiles[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in files) + '\n};\n\n', file = f)

print('''
FILE* fopen(const char *path, const char *mode) {
for(int i = 0; i < packfsfilesnum; i++) if(0 == strcmp(path, packfsinfos[i].path)) return fmemopen((void*)packfsinfos[i].start, (size_t)(packfsinfos[i].end - packfsinfos[i].start), mode);
typedef FILE* (*orig_fopen_func_type)(const char *path, const char *mode);
fprintf(stderr, "log_file_access_preload: fopen(\\"%s\\", \\"%s\\")\\n", path, mode);
orig_fopen_func_type orig_func = (orig_fopen_func_type)dlsym(RTLD_NEXT, "fopen");
return orig_func(path, mode); }

int access(const char *path, int flags) {
typedef int (*orig_func_type)(const char *pathname, int flags);
fprintf(stderr, "log_file_access_preload: access(\\"%s\\", %d)\\n", path, flags);
orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "access");
return orig_func(path, flags); }

int stat(const char *restrict pathname, struct stat *restrict statbuf) {
typedef int (*orig_func_type)(const char *restrict pathname, struct stat *restrict statbuf);
fprintf(stderr, "log_file_access_preload: stat(\\"%s\\", %p)\\n", pathname, (void*)statbuf);
orig_func_type orig_func = (orig_func_type)dlsym(RTLD_NEXT, "stat");
return orig_func(pathname, statbuf); }

int main(int argc, char* argv[]) {
char buf[1024];
if(argc < 2) return 2;
FILE* f = fopen(argv[1], "r");
if(!f) return 1;
do {
size_t cnt = fread(buf, 1, sizeof(buf), f);
fwrite(buf, 1, cnt, stdout);
}
while(!feof(f) && !ferror(f));
return 0;}
''', file = f)

# bool prefix(const char *pre, const char *str) { return strncmp(pre, str, strlen(pre)) == 0; }
