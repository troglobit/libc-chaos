all:
	gcc -shared -fPIC src/chaos_pread.c -o libchaos.so -ldl
	gcc src/test_pread.c -o test_pread
	gcc src/test_pwrite.c -o test_pwrite
	PREAD_ERROR=wrong_byte LD_PRELOAD=$(PWD)/libchaos.so ./test_pread
	PWRITE_ERROR=nospace LD_PRELOAD=$(PWD)/libchaos.so ./test_pwrite
clean:
	rm libchaos.so test_pread test_pwrite
