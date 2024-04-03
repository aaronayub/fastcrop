# Copyright (c) 2024 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

project('fastcrop', 'c',
  version : '0.1',
  license: 'GPL-3.0-only',
  default_options : ['warning_level=3'])

gtkdep = dependency('gtk4', version: '>=4')

subdir('src')

executable('fastcrop',
  sources,
  dependencies : gtkdep,
  install : true)
