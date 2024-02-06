#

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
	
