# Copyright (c) 2024 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

sources = files([
  'fc-app.c',
  'fc-app-window.c',
  'main.c',
  'motion-events.c',
])

# Sources for optional dependencies
if magick.found()
  sources += files(['./lib/magick.c'])
endif
