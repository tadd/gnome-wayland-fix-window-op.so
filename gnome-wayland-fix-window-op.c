#include <stdbool.h>

#define _GNU_SOURCE
#include <dlfcn.h>
#undef _GNU_SOURCE
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>

static void (*orig_raise)(GdkWindow *window);
static GDBusProxy *proxy;

static bool dlsym_exist(const char *sym)
{
    return dlsym(RTLD_DEFAULT, sym) != NULL;
}

static bool check_gtk3_funcs(void)
{
    return dlsym_exist("gdk_wayland_window_get_type") &&
        dlsym_exist("gdk_window_get_window_type") &&
        dlsym_exist("gtk_widget_get_window") &&
        dlsym_exist("gtk_window_list_toplevels") &&
        dlsym_exist("gtk_window_get_title");
}

__attribute__ ((constructor))
static void ctor(void)
{
    if (!check_gtk3_funcs())
        return;
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

static bool is_managable(GdkWindow *window)
{
    return proxy != NULL && GDK_IS_WAYLAND_WINDOW(window) &&
        gdk_window_get_window_type(window) == GDK_WINDOW_TOPLEVEL;
}

static int cmp_gtkw_gdkw(const void *t, const void *d)
{
    const GtkWindow *gtkw = t;
    const GdkWindow *gdkw = d;
    g_autoptr(GdkWindow) gdkw2 = gtk_widget_get_window(GTK_WIDGET(gtkw));
    return gdkw != gdkw2; // return 0 if found
}

static GtkWindow *gdkwin_to_gtkwin(GdkWindow *gdkw)
{
    g_autoptr(GList) all = gtk_window_list_toplevels();
    if (all == NULL)
        return NULL;
    GList *found = g_list_find_custom(all, gdkw, cmp_gtkw_gdkw);
    return found ? found->data : NULL;
}

static const char *gdkwin_get_title(GdkWindow *gdkw)
{
    GtkWindow *gtkw = gdkwin_to_gtkwin(gdkw);
    if (gtkw == NULL) {
        g_warning("GdkWindow:%p not found in the GtkWindow toplevel list", gdkw);
        return NULL;
    }
    return gtk_window_get_title(gtkw);
}

static void activate_by_title(const char *title)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GVariant) vtitle = g_variant_new("(s)", title);
    g_autoptr(GVariant) ret =
        g_dbus_proxy_call_sync(proxy, "activateByTitle", vtitle,
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
