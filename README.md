GNOME Wayland: fix `gdk_window_*`
================================

This preloaded library fixes `gdk_window_raise()`/`lower()` to be effective with GNOME
Shell on Wayland.

## Usage

1. `make`
2. Launch any GTK3 application with `LD_PRELOAD=gnome-wayland-fix-gdk-window.c`
3. Wait for the canonical specs and impls

## Tricks

TBA

## Development

https://github.com/tadd/gnome-shell-gdk-window-raise-lower.so

## License

LGPL 2.1. See COPYING.md.
