GNOME Wayland: fix `gtk_window_*`
================================

This GTK3 Module fixes `gtk_window_raise()`/`lower()` to be effective with GNOME Shell on
Wayland.

## Usage

1. `./configure && make && make install`
2. Add `gnome-wayland-fix-gdk-window` in the environment variable `GTK_MODULES`
3. Launch any GTK3 application

## Tricks

TBA

## Development

https://github.com/tadd/gnome-shell-gdk-window-raise-lower.so

## License

LGPL 2.1. See COPYING.md.
