GNOME on Wayland: fix `gdk_window_*`
====================================

This preloaded library fixes `gdk_window_raise()`/`lower()` to be effective with GNOME
Shell on Wayland.

## Usage

1. `make`
2. Launch any GTK3 application with `LD_PRELOAD=$PWD/gnome-wayland-fix-gdk-window.so`
3. Enjoy your ordinary and normal desktop life
4. Wait for the canonical specs and impls to remove this hack

## Tricks

TBA

## Development

https://github.com/tadd/gnome-wayland-fix-gdk-window.so

## License

LGPL 2.1. See COPYING.md.
