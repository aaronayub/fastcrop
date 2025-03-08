# Fastcrop 
A very lightweight, visual, image-cropping tool.

## Usage
Run fastcrop with the file to read as the first argument, and the path to write to as the second:
```
fastcrop {INPUT} {OUTPUT}
```

The output path can be the same as the input, in which case, the image will be overwritten.

In the window that appears, the image to crop will be displayed, along with a rectangle that spans the image. By dragging the mouse near one of the sides or corners of the rectangle, you can change the area of the image to be cropped. You will see the rectangle update accordingly. Press Enter to crop the image, which will additionally close the window.

Fastcrop can optionally be run with the --dimensions command line argument, which will print the dimensions of the crop area to a user-specified format, compatible with other programs. The "%x", "%y", "%w", and "%y" instances in this format will be replaced by the image's starting x, starting y, width, and height pixels respectively. For example, to print out the dimensions of an image in an ImageMagick geometry compatible format, run the following:
```
fastcrop -d "%wx%h+%x+%y" input.png
```

If this argument is chosen, an output path is not required to run the program.

To view available application options, enter:
```
fastcrop --help
```

### Keyboard Shortcuts
|Key   |Action                  |
|------|------------------------|
|Enter |Crop the image and exit |
|q     |Quit                    |
|r     |Reset crop area         |
|t     |Toggle text display     |

## Installation
To install this project, run the following.
```
meson setup build
sudo ninja -C build install
```

If you only want to build the project, but not install it on your machine, run the following.
```
meson setup build && ninja -C build
```
This will create the executable file 'fastcrop' in the build directory.

## Build Requirements
- GTK >= 4.0
- ImageMagick >= 7.0 (Optional, allows for writing output with ImageMagick)
- meson

## License
Copyright (C) 2024 Aaron Ayub

This project and its source code is made available under the GNU GPL version 3.0 only.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
