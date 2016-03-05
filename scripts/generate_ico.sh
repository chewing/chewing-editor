#!/bin/sh
# require ImageMagick

# in order to get sufficient resolution from svg, oversample to 72*16 = 1152 dpi
convert -density 1152 chewing-editor.svg -define icon:auto-resize chewing-editor.ico
