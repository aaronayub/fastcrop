// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <gtk/gtk.h>

#define FC_APP_TYPE (fc_app_get_type ())
G_DECLARE_FINAL_TYPE (FcApp, fc_app, FC, APP, GtkApplication)

FcApp *fc_app_new (void);
