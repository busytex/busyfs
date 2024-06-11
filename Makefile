URL_busybox ?= https://busybox.net/downloads/busybox-1.33.0.tar.bz2

busybox:
	mkdir -p source
	wget -nc "$(URL_busybox)" -O source/$@.tar.bz2
	-mkdir -p build && rm -rf build/*
	tar -xf source/busybox.tar.bz2 --strip-components=1 --directory=build
	cp .config build
	$(CC) -DLOGFILEACCESSDYNAMIC -shared -fPIC log_file_access.c -o log_file_access.so -ldl
	$(MAKE) -C build SKIP_STRIP=y

busyfs.a:
	$(CC) -o build/native/log_file_access.lo -c log_file_access.c -DLOGFILEACCESSSTATIC
	cp $(shell $(CC) -print-file-name=libc.a) build/native/libc.a
	$(NM) build/native/libc.a
	$(AR) x build/native/libc.a fopen.lo open.lo
	$(OBJCOPY) --redefine-sym fopen=orig_fopen fopen.lo
	$(OBJCOPY) --redefine-sym open=orig_open open.lo
	$(AR) rs build/native/libc.a fopen.lo open.lo
	$(AR) rbs aio.lo build/native/libc.a build/native/log_file_access.lo
	$(NM) build/native/libc.a
	
