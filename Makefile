all:
	gcc -W -Wall -Werror -shared -fPIC src/chaos.c -o libchaos.so -ldl

test: all
	$(MAKE) -C test $@

clean:
	$(MAKE) -C test $@
	rm libchaos.so test/pread_* test/pwrite_*
