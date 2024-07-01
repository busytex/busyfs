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

f = open(args.output_path, 'w')
print('\n'.join(f'extern char _binary_{pp}_start[], _binary_{pp}_end[];' for p in objects for pp in [p.translate(translate)]), file = f)
print('\nconst char* packfsdirs[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in dirs) + '\n};\n\n', file = f)
print('\nconst char* packfsfiles[] = {\n' + ',\n'.join('"' + repr(p)[1:-1] + '"' for p in files) + '\n};\n\n', file = f)

print('struct packfsinfo { const char *path; const char* start; const char* end; } packfsinfos[] = {', file = f)
for p in files:
    pp = p.translate(translate)
    print('{ "' + repr(p)[1:-1] + f'", _binary_{pp}_start, _binary_{pp}_end },', file = f)
print('};', file = f)
