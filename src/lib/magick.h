// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/** Crops a file using ImageMagick
 * Returns 0 if successful, non-zero on failure */
int crop_magick (int width, int height, int x, int y,
    char *input_path, char *output_path);
