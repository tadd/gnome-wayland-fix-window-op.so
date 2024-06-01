#include <stdbool.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <dlfcn.h>

#include <gdk/gdk.h>
#include <meta/window.h>

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

static void my_raise(GdkWindow *window)
{
    orig_raise(window);
}

//Overriding function
void gdk_window_raise(GdkWindow *window)
{
    if (is_managable(window))
        my_raise(window);
    else
        orig_raise(window);
}
