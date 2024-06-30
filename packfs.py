import os
import argparse
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--input-path', '-i')
parser.add_argument('--output-path', '-o')
args = parser.parse_args()

assert os.path.exists(args.input_path) and os.path.isdir(args.input_path)

dirs = []
for (dirpath, dirnames, filenames) in os.walk(args.input_path):
    dirs.extend(os.path.join(dirpath, dirname) for dirname in dirnames)
print('const char* packfs_dirs[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in dirs) + '\n};\n\n')

files = []
for (dirpath, dirnames, filenames) in os.walk(args.input_path):
    for basename in filenames:
        if basename.endswith('.o'):
            continue
        p = os.path.join(dirpath, basename)
        files.append(p)
        subprocess.check_call(['ld', '-r', '-b', 'binary', '-o', p + '.o', p])
        subprocess.check_call(['nm', p + '.o'])

print('const char* packfs_files[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in files) + '\n};\n\n')

# extern char _binary_updmap_pl_start[];
# extern char _binary_updmap_pl_end[];
# ld -r -b binary -o updmap.o  updmap.pl
# int iSize = (int)(_binary_updmap_pl_end - _binary_updmap_pl_start);
