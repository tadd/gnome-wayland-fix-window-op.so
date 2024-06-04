#include <stdbool.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <dlfcn.h>

#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>
#include <meta/window.h>
#include <wayland-server-protocol.h>

static void (*orig_raise)(GdkWindow *window);
static void (*meta_raise)(MetaWindow *window);
static void *libmutter;

__attribute__ ((constructor))
static void ctor(void)
{
    orig_raise = dlsym(RTLD_NEXT, "gdk_window_raise");

    libmutter = dlopen("libmutter-11.so.0", RTLD_NOW);
    if (libmutter == NULL)
        return;
    meta_raise = dlsym(libmutter, "meta_window_raise");
}

__attribute__ ((destructor))
static void dtor(void)
{
    if (libmutter)
        dlclose(libmutter);
}

static bool is_managable(GdkWindow *window)
{
    return orig_raise != NULL &&
        meta_raise != NULL &&
        window != NULL;
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
    if (mwindow && !mwindow)
        (*meta_raise)(mwindow);
}

//Overriding function
void gdk_window_raise(GdkWindow *window)
{
    if (is_managable(window))
        my_raise(window);
    else
        (*orig_raise)(window);
}
