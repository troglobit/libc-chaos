RUN      = LD_PRELOAD=../src/libchaos.so ERROR_RATE=100
CFLAGS   = -g
CPPFLAGS = -W -Wall -Wextra
EXEC     =  pread_hang  pread_io  pread_intr  pread_wrong_byte	\
	    pwrite_hang pwrite_io pwrite_intr pwrite_no_space

all: $(EXEC)

check test: all
	$(RUN) PREAD_ERROR=hang READ_ERROR=hang HANG_TIME=2 ./pread_hang
	$(RUN) PREAD_ERROR=intr                             ./pread_intr
	$(RUN) PREAD_ERROR=io                               ./pread_io
	$(RUN) PREAD_ERROR=wrong_byte                       ./pread_wrong_byte
	$(RUN) PWRITE_ERROR=hang HANG_TIME=2                ./pwrite_hang
	$(RUN) PWRITE_ERROR=intr                            ./pwrite_intr
	$(RUN) PWRITE_ERROR=io                              ./pwrite_io
	$(RUN) PWRITE_ERROR=no_space                        ./pwrite_no_space
