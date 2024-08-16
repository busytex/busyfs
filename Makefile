URL_busybox ?= https://busybox.net/downloads/busybox-1.33.0.tar.bz2

packfs.so:
	$(CC) -DPACKFS_LOG -shared -fPIC packfs.c -o packfs.so -ldl # @packfs.h.txt
    

busybox:
	mkdir -p source
	wget -nc "$(URL_busybox)" -O source/$@.tar.bz2
	-mkdir -p build && rm -rf build/*
	tar -xf source/busybox.tar.bz2 --strip-components=1 --directory=build
	cp .config build
	$(MAKE) -C build SKIP_STRIP=y
	#$(CC) -shared -fPIC log_file_access_dynamic.c -o log_file_access_dynamic.so -ldl

busyfs.a:
	$(CC) -o log_file_access_static.lo -c log_file_access_static.c
	cp $(shell $(CC) -print-file-name=libc.a) libc.a
	$(NM) libc.a
	$(AR) x libc.a fopen.lo open.lo
	$(OBJCOPY) --redefine-sym fopen=orig_fopen fopen.lo
	$(OBJCOPY) --redefine-sym open=orig_open    open.lo
	$(AR) rs libc.a fopen.lo open.lo
	$(AR) rbs aio.lo libc.a log_file_access_static.lo
	$(NM) libc.a
