/*
 * Copyright (c) 2014-2020 gnome-mpv
 *
 * This file is part of Celluloid.
 *
 * Celluloid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Celluloid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Celluloid.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMMON_H
#define COMMON_H

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef enum TrackType TrackType;

typedef struct _CelluloidPlaylistEntry CelluloidPlaylistEntry;
typedef struct _CelluloidMetadataEntry CelluloidMetadataEntry;
typedef struct CelluloidTrack CelluloidTrack;
typedef struct CelluloidDisc CelluloidDisc;

enum TrackType
{
	TRACK_TYPE_INVALID,
	TRACK_TYPE_AUDIO,
	TRACK_TYPE_VIDEO,
	TRACK_TYPE_SUBTITLE,
	TRACK_TYPE_N
};

struct _CelluloidPlaylistEntry
{
	gchar *filename;
	gchar *title;
	gdouble duration;
	GPtrArray *metadata;
};

struct _CelluloidMetadataEntry
{
	gchar *key;
	gchar *value;
};

struct CelluloidTrack
{
	TrackType type;
	gint64 id;
	gchar *title;
	gchar *lang;
};

struct CelluloidDisc
{
	gchar *uri;
	gchar *label;
};

CelluloidPlaylistEntry *
celluloid_playlist_entry_new(const gchar *filename, const gchar *title);

void
celluloid_playlist_entry_free(CelluloidPlaylistEntry *entry);

CelluloidMetadataEntry *
celluloid_metadata_entry_new(const gchar *key, const gchar *value);

void
celluloid_metadata_entry_free(CelluloidMetadataEntry *entry);

CelluloidTrack *
celluloid_track_new(void);

void
celluloid_track_free(CelluloidTrack *entry);

CelluloidDisc *
celluloid_disc_new(void);

void
celluloid_disc_free(CelluloidDisc *disc);

gchar *
get_config_dir_path(void);

gchar *
get_scripts_dir_path(void);

gchar *
get_watch_dir_path(void);

gchar *
get_path_from_uri(const gchar *uri);

gchar *
get_name_from_path(const gchar *path);

gboolean
extension_matches(const gchar *filename, const gchar **extensions);

gboolean
g_source_clear(guint *tag);

void *
gslist_to_array(GSList *slist);

gchar *
strnjoinv(const gchar *separator, const gchar **str_array, gsize count);

char *
format_time(gint seconds, gboolean show_hour);

void
activate_action_string(GActionMap *map, const gchar *str);

G_END_DECLS

#endif
