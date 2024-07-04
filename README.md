GNOME on Wayland: Fix `gdk_window_*` Operations
===============================================

This preloaded library fixes some GTK3 window operations like
[`gdk_window_raise`](https://docs.gtk.org/gdk3/method.Window.raise.html)
to be effective with GNOME/Mutter on Wayland.

Unfortunately, `gdk_window_raise` is the only function we can fix at this
time, but we plan to fix others using the same mechanism.

## Requirement

GNOME extension [Activate Window by
Title](https://extensions.gnome.org/extension/5021/activate-window-by-title/)
needs to be installed.

## Usage

1. `make`
2. Launch any GTK3 application with
   `LD_PRELOAD=$PWD/gnome-wayland-fix-window-op.so <yourappnamehere>`
3. Enjoy your ordinary and normal desktop life
4. Wait for the canonical specs and impls to remove this hack

## Tricks

[`LD_PRELOAD`](https://man7.org/linux/man-pages/man8/ld.so.8.html#ENVIRONMENT)
and [D-Bus](https://www.freedesktop.org/wiki/Software/dbus/).

1. Override a target function with our implementation with `LD_LIBRARY`.
2. Call the original function when we don't need to change its behavior.
3. Otherwise, call the D-Bus method "activateByTitle" with the window title
   taken from the target function's argument.

## Development

https://github.com/tadd/gnome-wayland-fix-window-op.so

## License

LGPL 2.1. See COPYING.md.

## References

* Satoru Takabayashi, Fumitoshi Ukai, Yusuke Sato, Shinichiro Hamaji, and
  Kazuyuki Shudo. *Hack #61: Using "LD_PRELOAD" to wrap existing functions* in
  [**Binary Hacks**](https://www.oreilly.co.jp/books/4873112885/) (in Japanese).
  O'Reilly Japan, pp 228-230, Nov. 2006.
  * [English page](http://0xcc.net/binhacks/eabout.html) also available
