// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include "fc-app.h"

struct _FcApp {
	GtkApplication parent;
};

G_DEFINE_TYPE (FcApp, fc_app, GTK_TYPE_APPLICATION)

static void fc_app_class_init (FcAppClass *class) {
	GApplicationClass *app_class = G_APPLICATION_CLASS (class);
}

static void fc_app_init (FcApp *app) {
}

FcApp *fc_app_new (void) {
	return g_object_new (FC_APP_TYPE, "application-id", "com.github.aaronayub.fastcrop", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
