#define _GNU_SOURCE
#include <dlfcn.h>

#include <gdk/gdk.h>

static void (*orig_raise)(GdkWindow *window);

__attribute__ ((constructor))
static void ctor(void)
{
    orig_raise = dlsym(RTLD_NEXT, "gdk_window_raise");
}

void gdk_window_raise(GdkWindow *window)
{
    fprintf(stderr, "raised!\n");
    orig_raise(window);
}
