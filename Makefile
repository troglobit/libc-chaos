ERROR_SETTING = ERROR_RATE=100 LD_PRELOAD=$(PWD)/libchaos.so
all:
	gcc -Wall -Werror -shared -fPIC src/chaos_pread.c -o libchaos.so -ldl
	gcc src/test_pread.c -o test_pread
	gcc src/test_pwrite.c -o test_pwrite
	$(ERROR_SETTING) PREAD_ERROR=wrong_byte ./test_pread
	$(ERROR_SETTING) PWRITE_ERROR=nospace   ./test_pwrite
clean:
	rm libchaos.so test_pread test_pwrite
