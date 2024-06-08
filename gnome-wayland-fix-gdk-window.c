#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <dlfcn.h>
#undef _GNU_SOURCE

#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>
#include <meta/window.h>
#include <wayland-server-protocol.h>

static void (*orig_raise)(GdkWindow *window);

__attribute__ ((constructor))
static void ctor(void)
{
    orig_raise = dlsym(RTLD_NEXT, "gdk_window_raise");
    assert(orig_raise && "only GTK3 app is supported currently.");
}

static bool is_managable(GdkWindow *window)
{
    return window != NULL && GDK_IS_WAYLAND_WINDOW(window);
}

static MetaWindow *gdkwin_to_metawin(GdkWindow *gwindow)
{
    struct wl_surface *surface = gdk_wayland_window_get_wl_surface(gwindow);
    surface = surface;
    return NULL;
}

static void my_raise(GdkWindow *gwindow)
{
    MetaWindow *mwindow = gdkwin_to_metawin(gwindow);
    if (mwindow)
        meta_window_raise(mwindow);
}

//Overriding function
void gdk_window_raise(GdkWindow *window)
{
    if (is_managable(window))
        my_raise(window);
    (*orig_raise)(window);
}
