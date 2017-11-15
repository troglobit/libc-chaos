all:
	$(MAKE) -C src $@

test: all
	$(MAKE) -C test $@

clean:
	$(MAKE) -C src $@
	$(MAKE) -C test $@
