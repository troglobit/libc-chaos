CFLAGS   = -g -fPIC
CPPFLAGS = -W -Wall -Wextra
OBJS     = chaos.o
LIBS     = -ldl
LIBNAME  = libchaos.so

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) $(CFLAGS) -shared $(OBJS) -o $@ $(LIBS)

clean:
	$(RM) $(LIBNAME)
