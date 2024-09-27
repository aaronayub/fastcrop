// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** All functions which depend on ImageMagick */

#include <MagickWand/MagickWand.h>

/** Print out the MagickWand error message */
static void onMagickError (MagickWand *wand) {
  ExceptionType exception;
  char *error = MagickGetException (wand, &exception);
  fprintf (stderr, "%s\n", error);
  MagickRelinquishMemory (error);
}

int crop_magick (int width, int height, int x, int y,
    char *input_path, char *output_path)
{
  MagickBooleanType status;
  MagickWand *wand;

  // Initialize MagickWand environment
  MagickWandGenesis ();
  wand = NewMagickWand ();
  status = MagickReadImage (wand, input_path);
  if (status == MagickFalse) {
    onMagickError (wand);
    return 1;
  };

  // Ignore previous page canvas if available, as fastcrop does not display the canvas.
  status = MagickResetImagePage (wand, NULL);
  if (status == MagickFalse) {
    onMagickError (wand);
    return 1;
  };

  // Crop the image
  MagickCropImage (wand, width, height, x, y);
  status = MagickSetCompressionQuality (wand, 100);
  if (status == MagickFalse) {
    onMagickError (wand);
    return 1;
  };

  // Avoid writing a page canvas for consistency
  status = MagickResetImagePage (wand, NULL);
  if (status == MagickFalse) {
    onMagickError (wand);
    return 1;
  };

  // Write the cropped image
  status = MagickWriteImage (wand, output_path);
  if (status == MagickFalse) {
    onMagickError (wand);
    return 1;
  };
  wand = DestroyMagickWand (wand);
  MagickWandTerminus (); // End MagickWand environment

  return 0;
}
