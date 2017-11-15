CFLAGS   = -g -fPIC
CPPFLAGS = -W -Wall -Wextra
OBJS     = chaos.o
LIBS     = -ldl
LIBNAME  = libchaos.so

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	@printf "  LIB     $@\n"
	@$(CC) $(CFLAGS) -shared $(OBJS) -o $@ $(LIBS)

clean:
	@$(RM) $(LIBNAME) *.o

distclean: clean
	@$(RM) *~
