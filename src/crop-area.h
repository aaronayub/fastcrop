// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Position and size of the region of an image to be cropped or displayed */

#pragma once

typedef struct {
  int x;
  int y;
  int width;
  int height;
} CropArea;
