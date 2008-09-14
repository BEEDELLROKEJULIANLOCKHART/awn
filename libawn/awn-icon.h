/*
 * Copyright (C) 2008 Neil Jagdish Patel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License version 
 * 2 or later as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by Neil Jagdish Patel <njpatel@gmail.com>
 *
 */

#ifndef _AWN_ICON_H_
#define _AWN_ICON_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#include "awn-effects.h"

#define AWN_TYPE_ICON (awn_icon_get_type ())

#define AWN_ICON(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
	AWN_TYPE_ICON, AwnIcon))

#define AWN_ICON_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass),\
	AWN_TYPE_ICON, AwnIconClass))

#define AWN_IS_ICON(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj),\
	AWN_TYPE_ICON))

#define AWN_IS_ICON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),\
	AWN_TYPE_ICON))

#define AWN_ICON_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj),\
	AWN_TYPE_ICON, AwnIconClass))

typedef struct _AwnIcon        AwnIcon;
typedef struct _AwnIconClass   AwnIconClass;
typedef struct _AwnIconPrivate AwnIconPrivate;
typedef enum   _AwnIconState   AwnIconState;
 
struct _AwnIcon
{
  GtkDrawingArea  parent;	

  AwnIconPrivate *priv;
};

struct _AwnIconClass
{
  GtkDrawingAreaClass parent_class;
};

GType         awn_icon_get_type                 (void) G_GNUC_CONST;

GtkWidget *   awn_icon_new                      (void);

void          awn_icon_set_effect               (AwnIcon      *icon, 
                                                 AwnEffect     effect);

void          awn_icon_set_icon_from_pixbuf     (AwnIcon      *icon,
                                                 GdkPixbuf    *pixbuf);

void          awn_icon_set_icon_from_context    (AwnIcon      *icon,
                                                 cairo_t      *ctx);

void          awn_icon_set_message              (AwnIcon      *icon,
                                                 const gchar  *message);

const gchar * awn_icon_get_message              (AwnIcon      *icon);

void          awn_icon_set_progress             (AwnIcon      *icon,
                                                 gfloat        progress);

gfloat        awn_icon_get_progress             (AwnIcon      *icon);

void          awn_icon_set_is_active            (AwnIcon      *icon,
                                                 gboolean      is_active);

gboolean      awn_icon_get_is_active            (AwnIcon      *icon);

#endif /* _AWN_ICON_H_ */
