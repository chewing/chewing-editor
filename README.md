# Chewing Editor
[![Build Status](https://travis-ci.org/chewing/chewing-editor.png?branch=master)](https://travis-ci.org/chewing/chewing-editor)
[![Status-Icon](https://coveralls.io/repos/chewing/chewing-editor/badge.png?branch=master)](https://coveralls.io/r/chewing/chewing-editor)

chewing-editor is a cross platform chewing user phrase editor. It provides a
easy way to manage user phrase. With it, user can customize their user phrase to
increase input performance.

# Development

## Tools Requirement
*   [CMake](http://www.cmake.org/) >= 2.8.9
*   gcc >= 4.8 or Visual Studio Express 2012
*   [libchewing](https://github.com/chewing/libchewing)
*   Qt = 5
*   Editor with [EditorConfig](http://editorconfig.org/) supported

## Build
chewing-editor can be built by the following commands:

    cmake .
    make

If cmake cannot find Qt5, environment variable `CMAKE_PREFIX_PATH` shall be set
to Qt5 location. For example, `C:\Qt\Qt5.1.1\5.1.1\msvc2012_64`.

## Test
A custom target `make check` is used to build and run unit test.

## Coverage Report
chewing-editor integrates with gcov to provide code coverage report. The
following commands will generate coverage report for unit test.

    cmake . -DENABLE_GCOV=yes
    make check
    scripts/lcov.sh

The coverage report will be in coveragereport directory.

## Translation

### Update An Exist Translation
Please use the following steps to update an exist translation:
*   `cmake .`
*   `make lupdate`
*   ts files will be updated by `lupdate`
*   Use `linguist` to edit ts files

### Add A New Translation
Please use the following steps to add a new translation:
*   Add locale in `CMakeFiles.txt`
*   `cmake .`
*   `make lupdate`
*   ts files will be updated by `lupdate`
*   Use `linguist` to edit ts files

# License
*   This project is licensed under [GPL-2.0](http://opensource.org/licenses/GPL-2.0)
*   [Google C++ Mocking Framework](https://code.google.com/p/googlemock/) is licensed under [New BSD License](http://opensource.org/licenses/BSD-3-Clause)
