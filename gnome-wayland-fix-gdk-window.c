#include <stdbool.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <dlfcn.h>
#undef _GNU_SOURCE

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>

static void (*orig_raise)(GdkWindow *window);
static GDBusProxy *proxy;

__attribute__ ((constructor))
static void ctor(void)
{
    orig_raise = dlsym(RTLD_NEXT, "gdk_window_raise");
    if (orig_raise == NULL)
        return;
    g_autoptr(GError) error = NULL;
    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE,
                                          NULL, "org.gnome.Shell",
                                          "/de/lucaswerkmeister/ActivateWindowByTitle",
                                          "de.lucaswerkmeister.ActivateWindowByTitle",
                                          NULL, &error);
    if (error)
        proxy = NULL; // ignore error and stay NULL
}

__attribute__ ((destructor))
static void dtor(void)
{
    g_object_ref(proxy);
}

static bool is_managable(GdkWindow *window)
{
    return proxy != NULL && window != NULL && GDK_IS_WAYLAND_WINDOW(window);
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
    return found ? found->data : NULL;
}

static const char *gdkwin_get_title(GdkWindow *gdkw)
{
    const GtkWindow *gtkw = gdkwin_to_gtkwin(gdkw);
    if (gtkw == NULL) {
        g_warning("GdkWindow:%p not found in the GtkWindow toplevel list", gdkw);
        return NULL;
    }
    return gtk_window_get_title((GtkWindow*)gtkw);
}

static void activate_by_title(const char *title)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GVariant) ret =
        g_dbus_proxy_call_sync(proxy, "activateByTitle", g_variant_new("(s)", title),
                               G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
    g_autoptr(GVariant) found = g_variant_get_child_value(ret, 0);
    if (!g_variant_get_boolean(found))
        g_warning("title:%s not found", title);
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
    else
        (*orig_raise)(window);
}
