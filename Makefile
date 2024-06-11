URL_busybox ?= https://busybox.net/downloads/busybox-1.33.0.tar.bz2

busybox:
	mkdir -p source
	#wget -nc "$(URL_busybox)" -O source/$@.tar.bz2
	-mkdir -p build && rm -rf build/*
	tar -xf source/busybox.tar.bz2 --strip-components=1 --directory=build
	cp .config build
	$(MAKE) -C build SKIP_STRIP=y

busyfs.a:
	$(CC_$*) -o build/native/log_file_access.lo -c log_file_access.c -DLOGFILEACCESSSTATIC
	cp $(shell $(CC_$*) -print-file-name=libc.a) build/native/libc.a
	$(NM_$*) build/native/libc.a
	$(AR_$*) x build/native/libc.a fopen.lo open.lo
	$(OBJCOPY_$*) --redefine-sym fopen=orig_fopen fopen.lo
	$(OBJCOPY_$*) --redefine-sym open=orig_open open.lo
	$(AR_$*) rs build/native/libc.a fopen.lo open.lo
	$(AR_$*) rbs aio.lo build/native/libc.a build/native/log_file_access.lo
	$(NM_$*) build/native/libc.a
	
