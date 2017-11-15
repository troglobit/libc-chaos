all:
	@$(MAKE) -C src $@

check test: all
	@$(MAKE) -C test $@

clean distclean:
	@$(MAKE) -C src $@
	@$(MAKE) -C test $@
