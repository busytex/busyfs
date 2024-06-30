import os
import argparse
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--input-path', '-i')
args = parser.parse_args()

assert os.path.exists(args.input_path) and os.path.isdir(args.input_path)

dirs = []
for (dirpath, dirnames, filenames) in os.walk(args.input_path):
    dirs.extend(os.path.join(dirpath, dirname) for dirname in dirnames)
print('const char* packfs_dirs[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in dirs) + '\n};\n\n')

files = []
for (dirpath, dirnames, filenames) in os.walk(args.input_path):
    for basename in filenames:
        files.append(os.path.join(dirpath, basename))
print('const char* packfs_files[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in filenames) + '\n};\n\n')

# extern char _binary_updmap_pl_start[];
# extern char _binary_updmap_pl_end[];
# ld -r -b binary -o updmap.o  updmap.pl
# int iSize = (int)(_binary_updmap_pl_end - _binary_updmap_pl_start);
