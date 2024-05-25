base=gnome-wayland-fix-gdk-window
SRC=$(base).c
OBJ=$(base).o
LIB=$(base).so

CC=gcc
GTK_CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
CFLAGS=-Og -ggdb3 -fPIC -Wall -Wextra $(GTK_CFLAGS)

all: $(LIB)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -fPIC -c $<

$(LIB): $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $<

clean:
	rm -f *.o *.so

.PHONY: all clean
