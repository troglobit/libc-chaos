ERROR_SETTING = ERROR_RATE=100 LD_PRELOAD=$(PWD)/libchaos.so
all:
	gcc -Wall -Werror -shared -fPIC src/chaos_pread.c -o libchaos.so -ldl

test: all
	gcc src/test_pread_intr.c -o test_pread_intr
	gcc src/test_pread_io.c -o test_pread_io
	gcc src/test_pread_wrong_byte.c -o test_pread_wrong_byte
	gcc src/test_pwrite_io.c -o test_pwrite_io
	gcc src/test_pwrite_nospace.c -o test_pwrite_nospace
	gcc src/test_pwrite_intr.c -o test_pwrite_intr
	$(ERROR_SETTING) PREAD_ERROR=intr ./test_pread_intr
	$(ERROR_SETTING) PREAD_ERROR=io ./test_pread_io
	$(ERROR_SETTING) PREAD_ERROR=wrong_byte ./test_pread_wrong_byte
	$(ERROR_SETTING) PWRITE_ERROR=intr   ./test_pwrite_intr
	$(ERROR_SETTING) PWRITE_ERROR=io   ./test_pwrite_io
	$(ERROR_SETTING) PWRITE_ERROR=nospace   ./test_pwrite_nospace

clean:
	rm libchaos.so test_pread_* test_pwrite_*
