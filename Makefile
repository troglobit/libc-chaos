all:
	$(MAKE) -C src $@

check test: all
	$(MAKE) -C test $@

clean:
	$(MAKE) -C src $@
	$(MAKE) -C test $@
