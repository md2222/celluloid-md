/*
 * Fork by MD
 * 2025
 */

#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
//#include <stdlib.h>

#include "celluloid-shortcuts-window.h"


enum
{
  SHORTCUT_COLUMN = 0,
  ACTION_COLUMN,
  NUM_COLUMNS
};


struct _CelluloidShortcutsWindow
{
    GtkWindow  parent;
};

struct _CelluloidShortcutsWindowClass
{
    GtkWindow  parent;
};

struct ShortcutEntry
{
    const gchar *accel;
    const gchar *title;
};

struct ShortcutGroup
{
    const gchar *title;
    const struct ShortcutEntry *entries;
};

typedef struct ShortcutEntry ShortcutEntry;
typedef struct ShortcutGroup ShortcutGroup;

G_DEFINE_TYPE(CelluloidShortcutsWindow, celluloid_shortcuts_window, GTK_TYPE_WINDOW)


static gchar* strreplace(const char* str, const char* from, const char* to)
{
    if (!str)  return NULL;
    
    size_t resLen = strlen(str) + 1;
    char* res = g_malloc(resLen);
    *res = 0;
    
    if (!from || !strlen(from))  return strcpy(res, str);
    
    size_t fromLen = strlen(from);
    size_t toLen = strlen(to);
    size_t incLen = toLen > fromLen ? toLen - fromLen : 0;

    const char *p1 = str;
    char *p2 = 0;
    
    while ((p2 = strstr(p1, from))) 
    {
        size_t len = p2 - p1;
        strncat(res, p1, len);

        if (incLen)  
        {
            resLen += incLen;
            res = g_realloc(res, resLen);
        }
        strcat(res, to);
        
        p1 = p2 + fromLen;
    }
    
    strcat(res, p1);
    
    return res;
}

/*
static gboolean
visible_func (GtkTreeModel *model,
              GtkTreeIter  *iter,
              gpointer      data)
{
    gchar *str;

    gtk_tree_model_get (model, iter, ACTION_COLUMN, &str, -1); 
    
    gboolean res = !*str || strstr(str, "by") > 0;  // toplevel mast be visible - !*str
    g_free(str);
    
    return res;
}
*/

static void
celluloid_shortcuts_window_init(CelluloidShortcutsWindow *wnd)
{
    gtk_window_set_title (GTK_WINDOW (wnd), "Shortcuts");
    gtk_window_set_type_hint(GTK_WINDOW (wnd), GDK_WINDOW_TYPE_HINT_DIALOG);
    
    const ShortcutEntry general[]
        = { {"Ctrl+O", _("Open file")},
            {"Ctrl+L", _("Open location")},
            {"Ctrl+Shift+O", _("Add file to playlist")},
            {"Ctrl+Shift+L", _("Add location to playlist")},
            {"Ctrl+P", _("Show preferences dialog")},
            {"Ctrl+H", _("Toggle controls")},
            {"F9", _("Toggle playlist")},
            {"F11  /  F", _("Toggle fullscreen mode")},
            {"Escape", _("Leave fullscreen mode")},
            {"Shift+O", _("Toggle OSD states between normal and playback time / duration")},
            {"Shift+I", _("Show filename on the OSD")},
            {"O  /  Shift+P", _("Show progress, elapsed time, and duration on the OSD")},
            {NULL, NULL} };
            
    const ShortcutEntry seeking[]
        = { {"Left_arrow  /  Right_arrow", _("Seek backward / forward 5 seconds")},
            {"Ctrl+Left_arrow  /  Ctrl+Right_arrow", _("Exact seek backward / forward 15 seconds")},
            {"Shift+Left_arrow  /  Shift+Right_arrow", _("Exact seek backward / forward 1 minute")},
            //{"Shift+Left_arrow  /  Shift+Right_arrow", _("Exact seek backward / forward 1 second")},
            //{"Down_arrow  /  Up_arrow", _("Seek backward / forward 1 minute")},
            {"Shift+Down_arrow  /  Shift+Up_arrow", _("Exact seek backward / forward 5 seconds")},
            //{"Ctrl+Left_arrow  /  Ctrl+Right_arrow", _("Seek to previous / next subtitle")},
            {",  /  .", _("Step backward / forward a single frame")},
            //{"Page_Up  /  Page_Down", _("Seek to the beginning of the previous / next chapter")},
            {NULL, NULL} };
            
    const ShortcutEntry playback[]
        = { {"[  /  ]", _("Decrease / increase playback speed by 10%")},
            {"{  /  }", _("Halve / double current playback speed")},
            {"BackSpace", _("Reset playback speed to normal")},
            {"<  /  >", _("Go backward / forward in the playlist")},
            {"Delete", _("Remove selected playlist item")},
            {"Ctrl+Shift+S", _("Save playlist")},
            {"L", _("Set / clear A-B loop points")},
            {"Shift+L", _("Toggle infinite looping")},
            {"P  /  Space", _("Pause or unpause")},
            {"Ctrl+Q  /  Q", _("Quit")},
            {"Shift+Q", _("Save current playback position and quit")},
            {NULL, NULL} };
            
    const ShortcutEntry playlist[]
        = { 
            {"Page_Down  /  Page_Up", _("Play next / previous file")},
            {"Down_arrow  /  Up_arrow", _("Select next / previous file")},
            {"Enter", _("Play selected file")},
            {"Ctrl+F", _("Enter search mode")},
            {"Ctrl+G", _("Jump to next match")},
            {"Ctrl+Shift+G", _("Jump to previous match")},
            {"Escape", _("Exit search mode")},
            {NULL, NULL} 
        };
            
    const ShortcutEntry audio[]
        = { {"#", _("Cycle through audio tracks")},
            {" /   /  *", _("Decrease / increase volume")},
            {"9  /  0", _("Decrease / increase volume")},
            {"M", _("Mute or unmute")},
            {"Ctrl++ Ctrl+-", _("Adjust audio delay by + / - 0.1 seconds")},
            {NULL, NULL} };
            
    const ShortcutEntry subtitle[]
        = { {"V", _("Toggle subtitle visibility")},
            {"J  /  Shift+J", _("Cycle through available subtitles")},
            {"Z  /  Shift+Z", _("Adjust subtitle delay by + / - 0.1 seconds")},
            {"U", _("Toggle SSA / ASS subtitles style override")},
            {"R  /  Shift+R", _("Move subtitles up / down")},
            {"Shift+V", _("Toggle VSFilter aspect compatibility mode")},
            {NULL, NULL} };
            
    const ShortcutEntry video[]
        = { {"_", _("Cycle through video tracks")},
            {"W  /  Shift+W", _("Decrease / increase pan-and-scan range")},
            {"S", _("Take a screenshot")},
            {"Shift+S", _("Take a screenshot, without subtitles")},
            {"Ctrl+S", _("Take a screenshot, as the window shows it")},
            {"Alt+0", _("Resize video to half its original size")},
            {"Alt+1", _("Resize video to its original size")},
            {"Alt+2", _("Resize video to double its original size")},
            {"1  /  2", _("Adjust contrast")},
            {"3  /  4", _("Adjust brightness")},
            {"5  /  6", _("Adjust gamma")},
            {"7  /  8", _("Adjust saturation")},
            {"D", _("Activate or deactivate deinterlacer")},
            {"Shift+A", _("Cycle aspect ratio override")},
            {NULL, NULL} };
            
    const ShortcutGroup groups[]
        = { {_("User Interface"), general},
            {_("Video"), video},
            {_("Audio"), audio},
            {_("Subtitle"), subtitle},
            {_("Playback"), playback},
            {_("Seeking"), seeking},
            {_("Playlist"), playlist},
            {NULL, NULL} };
            
    GtkTreeIter iter;

    GtkTreeStore *store = gtk_tree_store_new (NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);

    for(gint i  = 0; groups[i].title; i++)
    {
        const ShortcutEntry *entries = groups[i].entries;
        
        gtk_tree_store_append (store, &iter, NULL);
        
        // for markup
        gchar* title = g_strconcat("<b>", groups[i].title, "</b>", NULL);
        
        gtk_tree_store_set (store, &iter,
                          SHORTCUT_COLUMN,  
                          //groups[i].title,
                          title,
                          ACTION_COLUMN, "",
                          -1);   
                          
        g_free(title);

        for(gint j  = 0; entries[j].accel; j++)
        {
              GtkTreeIter child_iter;

              gtk_tree_store_append (store, &child_iter, &iter);
              
              // for markup
              char* accel = strreplace(strreplace(entries[j].accel, "<", "&lt;"), ">", "&gt;");
              
              gtk_tree_store_set (store, &child_iter,
                              SHORTCUT_COLUMN, 
                              //entries[j].accel,
                              accel,
                              ACTION_COLUMN, entries[j].title,
                              -1);

              g_free(accel);
        }
    }
    
    GtkWidget *vbox;  
    
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
    gtk_container_add (GTK_CONTAINER (wnd), vbox); 
    
    GtkWidget *sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                       GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);
      
    GtkWidget *treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
    g_object_unref (store);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
    g_object_set (renderer, "xalign", 0.0, NULL);
    gint col_offset = gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
                                                            -1, "Shortcut",
                                                            renderer, 
                                                            //"text",
                                                            "markup",
                                                            SHORTCUT_COLUMN,
                                                            NULL);
    //GtkTreeViewColumn *column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), col_offset - 1);
    
    renderer = gtk_cell_renderer_text_new ();
    g_object_set (renderer, "xalign", 0.0, NULL);
    col_offset = gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
                                                            -1, "Action",
                                                            renderer, "text",
                                                            ACTION_COLUMN,
                                                            NULL);
    //column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), col_offset - 1);

    gtk_container_add (GTK_CONTAINER (sw), treeview);
    
    g_signal_connect (treeview, "realize", G_CALLBACK (gtk_tree_view_expand_all), NULL);
    
    gtk_window_set_default_size (GTK_WINDOW (wnd), 700, 500);
    
/*                      
    GtkTreeModel *filtered = gtk_tree_model_filter_new (GTK_TREE_MODEL (store), NULL);
    //GtkTreeModel *filtered = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (GTK_TREE_MODEL (model), NULL));
    
    //gtk_tree_model_filter_set_visible_func (filtered, (GtkTreeModelFilterVisibleFunc) visible_func, model, NULL);
    
    gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER (filtered), visible_func, NULL, NULL);
    //gtk_tree_model_filter_set_visible_column(GTK_TREE_MODEL_FILTER (filtered), ACTION_COLUMN);
    
    gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), filtered);   
*/    
}


static void
celluloid_shortcuts_window_class_init(CelluloidShortcutsWindowClass *klass)
{
}


GtkWidget *
celluloid_shortcuts_window_new(GtkWindow *parent)
{
    return GTK_WIDGET(g_object_new( celluloid_shortcuts_window_get_type(),
                    "transient-for", parent,
                    "modal", TRUE,
                    NULL));
}
