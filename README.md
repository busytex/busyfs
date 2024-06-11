```shell
wget https://raw.githubusercontent.com/coreutils/coreutils/master/src/ls.c
wget https://raw.githubusercontent.com/coreutils/coreutils/master/src/cat.c
```

### Virtual and LD_PRELOAD readonly file system (reading ISO / TAR / ZIP)
- LD_PRELOAD FS that could work with package zip archvies (CTAN? ftp://tug.org/texlive/Contents/live/texmf-dist/)
- LD_PRELOAD FS that could work with texlive iso files containing zip archives
- virtual FS for reading ISO/TAR/ZIP texlive TDS
- virtual FS for reading Perl scripts
- virtual FS for embedding small TDS into binaries
- https://kevinpt.github.io/evfs/
- https://github.com/erincandescent/lib9660/blob/master/tb9660.c
- https://github.com/jacereda/fsatrace
- https://github.com/fritzw/ld-preload-open/blob/master/path-mapping.c
- https://github.com/mikix/deb2snap/blob/master/src/preload.c#L84
- https://lists.ubuntu.com/archives/snappy-devel/2015-February/000282.html
- https://www.tomaz.me/2014/01/08/detecting-which-process-is-creating-a-file-using-ld-preload-trick.html
- https://github.com/AppImage/AppImageKit/issues/267
- https://avf.sourceforge.net/
- https://gist.github.com/przemoc/571086
- https://adared.ch/unionfs_by_intercept/ https://adared.ch/wp-content/uploads/2012/01/unionfs_by_intercept.c
- https://arxiv.org/abs/1908.10740
- http://ordiluc.net/fs/libetc/
- https://pages.stern.nyu.edu/~marriaga/software/libtrash/
- https://www.madlab.it/old-school/codes/uml.c
- https://github.com/pytorch/pytorch/pull/123002/files - self-mmap
