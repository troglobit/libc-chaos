ERROR_SETTING = LD_PRELOAD=$(PWD)/libchaos.so ERROR_RATE=100
all:
	gcc -Wall -Werror -shared -fPIC src/chaos.c -o libchaos.so -ldl

test: all
	gcc src/test_pread_hang.c -o test_pread_hang
	gcc src/test_pread_intr.c -o test_pread_intr
	gcc src/test_pread_io.c -o test_pread_io
	gcc src/test_pread_wrong_byte.c -o test_pread_wrong_byte
	gcc src/test_pwrite_hang.c -o test_pwrite_hang
	gcc src/test_pwrite_io.c -o test_pwrite_io
	gcc src/test_pwrite_no_space.c -o test_pwrite_no_space
	gcc src/test_pwrite_intr.c -o test_pwrite_intr
	$(ERROR_SETTING) PREAD_ERROR=hang READ_ERROR=hang HANG_TIME=2 ./test_pread_hang
	$(ERROR_SETTING) PREAD_ERROR=intr ./test_pread_intr
	$(ERROR_SETTING) PREAD_ERROR=io ./test_pread_io
	$(ERROR_SETTING) PREAD_ERROR=wrong_byte ./test_pread_wrong_byte
	$(ERROR_SETTING) PWRITE_ERROR=hang HANG_TIME=2 ./test_pwrite_hang
	$(ERROR_SETTING) PWRITE_ERROR=intr   ./test_pwrite_intr
	$(ERROR_SETTING) PWRITE_ERROR=io   ./test_pwrite_io
	$(ERROR_SETTING) PWRITE_ERROR=no_space   ./test_pwrite_no_space

clean:
	rm libchaos.so test_pread_* test_pwrite_*
