# Copyright (c) 2024 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

project('fastcrop', 'c',
  version : '0.3.0',
  license: 'GPL-3.0-only',
  default_options : ['warning_level=3'])

gtk = dependency('gtk4', version : '>=4')

# Optional dependencies
magick = dependency('MagickWand', version : '>=7', required : get_option('MagickWand'))
conf_data = configuration_data()
conf_data.set('DEP_MAGICK', magick.found())
configure_file(output: 'config.h', configuration: conf_data)

subdir('src')

executable('fastcrop',
  sources,
  dependencies : [gtk, magick],
  install : true)
