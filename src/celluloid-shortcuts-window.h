/*
 * Fork by MD
 * 2025
 */

#ifndef SHORTCUTS_WINDOW_H
#define SHORTCUTS_WINDOW_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CELLULOID_TYPE_SHORTCUTS_WINDOW (celluloid_shortcuts_window_get_type ())

G_DECLARE_FINAL_TYPE(CelluloidShortcutsWindow, celluloid_shortcuts_window, CELLULOID,  WINDOW, GtkWindow)

GtkWidget *celluloid_shortcuts_window_new(GtkWindow *parent);

G_END_DECLS

#endif
