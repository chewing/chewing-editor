# chewing-editor
[![Build Status](https://travis-ci.org/chewing/chewing-editor.png?branch=master)](https://travis-ci.org/chewing/chewing-editor)
[![Status-Icon](https://coveralls.io/repos/chewing/chewing-editor/badge.png?branch=master)](https://coveralls.io/r/chewing/chewing-editor)

Cross platform chewing user phrase editor.

# Development Tool Requirements
*   [CMake](http://www.cmake.org/) >= 2.8.9
*   gcc >= 4.8 or Visual Studio Express 2012
*   [libchewing](https://github.com/chewing/libchewing)
*   Qt = 5
*   Editor with [EditorConfig](http://editorconfig.org/) supported

# How to Build
chewing-editor can be built by the following commands:

    cmake .
    make

However, `CHEWING_LIBRARY` and `CHEWING_INCLUDE_DIR` must be provided if CMake cannot find location of chewing library and chewing header.

If cmake cannot find Qt5, environment variable `CMAKE_PREFIX_PATH` shall be set to Qt5 location. For example, `C:\Qt\Qt5.1.1\5.1.1\msvc2012_64`.

# License
*   This project is licensed under [GPL-2.0](http://opensource.org/licenses/GPL-2.0)
*   [Google C++ Mocking Framework](https://code.google.com/p/googlemock/) is licensed under [New BSD License](http://opensource.org/licenses/BSD-3-Clause)
