CFLAGS    = -g
CPPFLAGS  = -W -Wall -Wextra
MAKEFLAGS = --no-print-directory

%.o: %.c
	@printf "  CC      $@\n"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%: %.c
	@printf "  CC      $@\n"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIBS)
