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

static int cmp_gtkw_gdkw(const void *t, const void *d)
{
    const GtkWindow *gtkw = t;
    const GdkWindow *gdkw = d;
    g_autoptr(GdkWindow) gdkw2 = gtk_widget_get_window(GTK_WIDGET(gtkw));
    return !(gdkw == gdkw2); // return 0 if found
}

static const GtkWindow *gdkwin_to_gtkwin(GdkWindow *gdkw)
{
    g_autoptr(GList) all = gtk_window_list_toplevels();
    if (all == NULL)
        return NULL;
    GList *found = g_list_find_custom(all, gdkw, cmp_gtkw_gdkw);
    GtkWindow *w = found ? found->data : NULL;
    return w;
}

static MetaWindow *gdkwin_to_metawin(GdkWindow *gdkw)
{
    const GtkWindow *w = gdkwin_to_gtkwin(gdkw);
    assert(w);
    fprintf(stderr, "(%p).title: %s\n", w, gtk_window_get_title((GtkWindow*)w));
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
