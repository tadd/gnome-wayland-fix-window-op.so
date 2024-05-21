#include <stdbool.h>

#define _GNU_SOURCE
#include <dlfcn.h>

#include <gdk/gdk.h>

static void (*orig_raise)(GdkWindow *window);

__attribute__ ((constructor))
static void ctor(void)
{
    orig_raise = dlsym(RTLD_NEXT, "gdk_window_raise");
}

static bool is_managable(GdkWindow *window)
{
    window = window;
    return false;
}

static void my_raise(GdkWindow *window)
{
    orig_raise(window);
}

//Overriding function
void gdk_window_raise(GdkWindow *window)
{
    fprintf(stderr, "raised!\n");
    if (is_managable(window))
        my_raise(window);
    else
        orig_raise(window);
}
