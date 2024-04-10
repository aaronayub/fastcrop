// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Event callbacks that should run when the user moves an input device, and
 * the structs that are passed as user_data. */

#include <gtk/gtk.h>
#include <math.h>

#include "motion-events.h"

void select_edges (GtkGestureDrag *self, gdouble x, gdouble y, gpointer user_data) {
  MotionParams *mp = user_data;
  GtkWidget *da = GTK_WIDGET (mp->draw_area);

  double width_scale = (double) gtk_widget_get_width (da)
    / gdk_pixbuf_get_width (mp->pixbuf);
  double height_scale = (double) gtk_widget_get_height (da)
    / gdk_pixbuf_get_height (mp->pixbuf);

  int distance = 30; // Distance from which a user can select an edge.
  mp->move_edges = FC_MV_NULL;
  if (fabs(x - mp->right * width_scale) < distance) {
    mp->move_edges |= FC_MV_RIGHT;
  } else if (fabs(x - mp->left * width_scale) < distance) {
    mp->move_edges |= FC_MV_LEFT;
  }
  if (fabs(y - mp->bottom * height_scale) < distance) {
    mp->move_edges |= FC_MV_BOTTOM;
  } else if (fabs(y - mp->top * height_scale) < distance) {
    mp->move_edges |= FC_MV_TOP;
  }
}
void update_edges (GtkGestureDrag *self, gdouble x, gdouble y, gpointer user_data) {

}
