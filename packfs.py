import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--input-path', '-i')
args = parser.parse_args()

assert os.path.exists(args.input_path) and os.path.isdir(args.input_path)

dirs = []
for (dirpath, dirnames, filenames) in os.walk(args.input_path):
    dirs.extend(os.path.join(dirpath, dirname) for dirname in dirnames)

print('const char* packfs_dirs[] = {\n' + ',\n'.join('"' + repr(dirname)[1:-1] + '"' for dirname in dirs) + '\n};')
# ld -r -b binary -o updmap.o  updmap.pl
# extern char _binary_updmap_pl_start[];
# extern char _binary_updmap_pl_end[];
# int iSize = (int)(_binary_updmap_pl_end - _binary_updmap_pl_start);
