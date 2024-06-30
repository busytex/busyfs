import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--input-path', '-i')
args = parser.parse_args()

dirs = []
for (dirpath, dirnames, filenames) in os.walk(input_path):
    dirs.extend(os.path.join(dirpath, dirname) for dirname in dirnames)

print('const char* dirs[] = {\n' + ',\n'.join('"' + repr(dirname)[1:-1] + '"' for dirname in dirs) + '\n};')
