CC = gcc
pkgs = gio-2.0 gtk+-3.0
pkg_cflags != pkg-config --cflags $(pkgs)
pkg_ldflags  != pkg-config --libs $(pkgs)
CFLAGS = -O2 -ggdb3 -fPIC -Wall -Wextra $(pkg_cflags)
LDFLAGS = $(pkg_libs) -ldl

base = gnome-wayland-fix-window-op
SRC = $(base).c
LIB = $(base).so

all: $(LIB)

$(LIB): $(SRC)
	$(CC) $(CFLAGS) -shared -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o *.so

.PHONY: all clean
