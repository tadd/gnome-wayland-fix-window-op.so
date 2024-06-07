base=gnome-wayland-fix-gdk-window
SRC=$(base).c
OBJ=$(base).o
LIB=$(base).so

CC=gcc
pkgs=gtk+-3.0 libmutter-11
PKG_CFLAGS=$(shell pkg-config --cflags $(pkgs))
CFLAGS=-O0 -ggdb3 -fPIC -Wall -Wextra $(PKG_CFLAGS)
LDFLAGS=$(shell pkg-config --libs $(pkgs))

all: $(LIB)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $<

$(LIB): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $<

clean:
	rm -f *.o *.so

.PHONY: all clean
