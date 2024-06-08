base=gnome-wayland-fix-gdk-window
SRC=$(base).c
LIB=$(base).so

CC=gcc
pkgs=gtk+-3.0 libmutter-11
pkg_cflags=$(shell pkg-config --cflags $(pkgs))
CFLAGS=-O0 -ggdb3 -fPIC -Wall -Wextra $(pkg_cflags)
LDFLAGS=$(shell pkg-config --libs $(pkgs)) -ldl

all: $(LIB)

$(LIB): $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o *.so

.PHONY: all clean
