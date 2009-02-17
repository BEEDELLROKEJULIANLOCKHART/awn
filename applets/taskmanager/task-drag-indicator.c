/*
 * Copyright (C) 2008 Neil Jagdish Patel <njpatel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as 
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by Neil Jagdish Patel <njpatel@gmail.com>
 *
 */

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include "task-drag-indicator.h"
#include "task-settings.h"

G_DEFINE_TYPE (TaskDragIndicator, task_drag_indicator, AWN_TYPE_ICON);

static const GtkTargetEntry drop_types[] = 
{
  { "awn/task-icon", 0, 0 }
};
static const gint n_drop_types = G_N_ELEMENTS (drop_types);

/* FORWARDS */
static gboolean task_drag_indicator_expose (GtkWidget *widget, GdkEventExpose *event);
static void     task_drag_indicator_drag_data_received (GtkWidget      *widget,
                                                        GdkDragContext *context,
                                                        gint            x,
                                                        gint            y,
                                                        GtkSelectionData *data,
                                                        guint           info,
                                                        guint           time);

enum
{
  PROP_0,
};

/* GObject stuff */
static void
task_drag_indicator_finalize (GObject *object)
{
  G_OBJECT_CLASS (task_drag_indicator_parent_class)->finalize (object);
}

static void
task_drag_indicator_constructed (GObject *object)
{

}

static void
task_drag_indicator_class_init (TaskDragIndicatorClass *klass)
{
  GObjectClass   *obj_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *wid_class = GTK_WIDGET_CLASS (klass);

  obj_class->constructed  = task_drag_indicator_constructed;
  obj_class->finalize     = task_drag_indicator_finalize;

  wid_class->expose_event         = task_drag_indicator_expose;
  wid_class->drag_data_received   = task_drag_indicator_drag_data_received;
}

static void
task_drag_indicator_init (TaskDragIndicator *drag_indicator)
{
  TaskSettings *settings;

  settings = task_settings_get_default ();

  awn_icon_set_orientation (AWN_ICON (drag_indicator), AWN_ORIENTATION_BOTTOM);
  awn_icon_set_custom_paint (AWN_ICON (drag_indicator), 10, settings->panel_size);

  /* D&D accept dragged objs */
  gtk_widget_add_events (GTK_WIDGET (drag_indicator), GDK_ALL_EVENTS_MASK);
  gtk_drag_dest_set (GTK_WIDGET (drag_indicator), 
                     GTK_DEST_DEFAULT_MOTION,
                     drop_types, n_drop_types,
                     GDK_ACTION_LINK);
  /*gtk_drag_dest_add_uri_targets (GTK_WIDGET (icon));
  gtk_drag_dest_add_text_targets (GTK_WIDGET (icon));*/
}

GtkWidget *
task_drag_indicator_new ()
{
  GtkWidget *drag_indicator = NULL;

  drag_indicator = g_object_new (TASK_TYPE_DRAG_INDICATOR, NULL);

  return drag_indicator;
}

static gboolean
task_drag_indicator_expose (GtkWidget *widget, GdkEventExpose *event)
{
  cairo_t      *cr;
  TaskSettings *settings;

  settings = task_settings_get_default ();
  cr = gdk_cairo_create (widget->window);

  cairo_set_line_width (cr, 1.0);

  if(settings->orient == AWN_ORIENTATION_TOP)
    cairo_translate (cr, 0.0, settings->offset);
  else if(settings->orient == AWN_ORIENTATION_LEFT)
    cairo_translate (cr, settings->offset, 0.0);
  else if(settings->orient == AWN_ORIENTATION_BOTTOM)
    cairo_translate (cr, 0.0, settings->panel_size-settings->offset+2);
  else
    cairo_translate (cr, settings->panel_size-settings->offset+2, 0.0);

  if(settings->orient == AWN_ORIENTATION_TOP || settings->orient == AWN_ORIENTATION_BOTTOM)
  {
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.6);

    cairo_move_to (cr, 0, 0);
    cairo_line_to (cr, 10, 0);
    cairo_line_to (cr, 5, 15);
    cairo_close_path (cr);

    cairo_move_to (cr, 0,  settings->panel_size);
    cairo_line_to (cr, 10, settings->panel_size);
    cairo_line_to (cr, 5,  settings->panel_size-15);
    cairo_close_path (cr);

    cairo_fill_preserve (cr);

    cairo_set_line_width (cr, 0.5);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.6);
    cairo_stroke(cr);
  }
  else
  {
    cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.6);

    cairo_move_to (cr, 0, 0);
    cairo_line_to (cr, 0, 10);
    cairo_line_to (cr, 15, 5);
    cairo_close_path (cr);

    cairo_move_to (cr, settings->panel_size, 0);
    cairo_line_to (cr, settings->panel_size, 10);
    cairo_line_to (cr, settings->panel_size-15, 5);
    cairo_close_path (cr);

    cairo_fill_preserve (cr);

    cairo_set_line_width (cr, 0.5);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.6);
    cairo_stroke(cr);
  }

  cairo_destroy (cr);

  return FALSE;
}

void
task_drag_indicator_refresh (TaskDragIndicator      *drag_indicator)
{
  TaskSettings *settings;

  g_return_if_fail (TASK_IS_DRAG_INDICATOR (drag_indicator));

  settings = task_settings_get_default ();

  if(settings->orient == AWN_ORIENTATION_TOP || settings->orient == AWN_ORIENTATION_BOTTOM)
  {
    awn_icon_set_custom_paint (AWN_ICON (drag_indicator), 10, settings->panel_size+settings->offset);
  }
  else
  {
    awn_icon_set_custom_paint (AWN_ICON (drag_indicator), settings->panel_size+settings->offset, 10);
  }
}

static void
task_drag_indicator_drag_data_received (GtkWidget      *widget,
                                        GdkDragContext *context,
                                        gint            x,
                                        gint            y,
                                        GtkSelectionData *sdata,
                                        guint           info,
                                        guint           time)
{
  gtk_drag_finish (context, TRUE, TRUE, time);
}