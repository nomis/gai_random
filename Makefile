.PHONY: all clean install

CFLAGS=-Wall -Wextra -Wshadow -Werror -O2 -ggdb -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_ISOC99_SOURCE -fPIC
LDFLAGS=-shared
LIBS=-ldl
INSTALL=install

all: gai_random.so
clean:
	rm -f gai_random.so

prefix=/usr
exec_prefix=$(prefix)
libdir=$(exec_prefix)/lib

install: all
	$(INSTALL) -m 755 -D gai_random.so $(DESTDIR)$(libdir)/gai_random.so

gai_random.so: gai_random.c Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)
