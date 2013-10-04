# chewing-editor
[![Build Status](https://travis-ci.org/chewing/chewing-editor.png?branch=master)](https://travis-ci.org/chewing/chewing-editor)

Cross platform chewing user phrase editor.

# Development Tool Requirements
*   [CMake](http://www.cmake.org/) >= 2.8
*   gcc >= 4.8
*   libchewing (current master branch)
*   Qt >= 4
*   Editor with [EditorConfig](http://editorconfig.org/) supported

# How to Build
chewing-editor can be built by the following commands:

    cmake .
    make

However, `CHEWING_LIBRARY` and `CHEWING_INCLUDE_DIR` must be provided if CMake cannot find location of chewing library and chewing header.
