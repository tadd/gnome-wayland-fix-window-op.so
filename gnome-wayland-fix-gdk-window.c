#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <dlfcn.h>
#undef _GNU_SOURCE

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>

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

static const char *gdkwin_get_title(GdkWindow *gdkw)
{
    const GtkWindow *gtkw = gdkwin_to_gtkwin(gdkw);
    assert(gtkw && "needs valid GTKWindow");
    return gtk_window_get_title((GtkWindow*)gtkw);
}

static void activate_by_title(const char *title)
{
    title = title;
}

static void my_raise(GdkWindow *window)
{
    const char *title = gdkwin_get_title(window);
    if (title)
        activate_by_title(title);
}

//Overriding function
void gdk_window_raise(GdkWindow *window)
{
    if (is_managable(window))
        my_raise(window);
    (*orig_raise)(window);
}
