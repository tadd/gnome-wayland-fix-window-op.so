base=gnome-wayland-fix-window-op
SRC=$(base).c
LIB=$(base).so

CC=gcc
pkgs=gio-2.0 gtk+-3.0
pkg_cflags=$(shell pkg-config --cflags $(pkgs))
CFLAGS=-O2 -ggdb3 -fPIC -Wall -Wextra $(pkg_cflags)
LDFLAGS=$(shell pkg-config --libs $(pkgs)) -ldl

all: $(LIB)

$(LIB): $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o *.so

.PHONY: all clean
