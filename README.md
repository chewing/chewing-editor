# Chewing Editor

[![TravisCI Status](https://travis-ci.org/chewing/chewing-editor.svg?branch=master)](https://travis-ci.org/chewing/chewing-editor)
[![Coveralls Status](https://img.shields.io/coveralls/chewing/chewing-editor.svg)](https://coveralls.io/r/chewing/chewing-editor)

chewing-editor is a cross platform chewing user phrase editor. It provides a
easy way to manage user phrase. With it, user can customize their user phrase to
increase input performance.

# Install
* Arch Linux: install from AUR: [Stable release](https://aur.archlinux.org/packages/chewing-editor/), [Development](https://aur.archlinux.org/packages/chewing-editor-git/)
* Mageia: install from urpmi: available in cauldron, Mageia 6 and later. 

# Development

## Tools Requirement

*   [CMake](https://www.cmake.org/) >= 3.0.0
*   gcc >= 4.8 or Visual Studio Express 2012
*   [libchewing](https://github.com/chewing/libchewing) >= 0.4.0
*   Qt = 5
*   Editor with [EditorConfig](http://editorconfig.org/) support
*   [lcov](http://ltp.sourceforge.net/coverage/lcov.php) for coverage testing

## Build

chewing-editor can be built by the following commands:

    cmake .
    make

If cmake cannot find Qt5, check the environment variable `CMAKE_PREFIX_PATH`
which shall point to Qt5 installation path. For example:
`C:\Qt\Qt5.1.1\5.1.1\msvc2012_64`.

## Test

A custom target `make check` is used to build and run unit test.

## Coverage Report

chewing-editor integrates with gcov to provide code coverage report. The
following commands will generate coverage report for unit test.

    cmake . -DENABLE_GCOV=yes
    make check
    scripts/lcov.sh

The coverage report will be in `coveragereport` directory.

## Translation

### Update Translations

Please use the following steps to update translations:
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

# Known Issues

## Bopomofo cannot Display Correctly

Qt5 before v5.3.0 has problem displaying bopomofo with HarfBuzz-NG. If you
encounter this issue, please upgrade your Qt5 to v5.3.0, or run chewing-editor
with `QT_HARFBUZZ=old`.

See also:
*   <https://github.com/chewing/chewing-editor/issues/7>
*   <https://codereview.qt-project.org/80505>
*   <https://qt.gitorious.org/qt/qtbase/commit/b7f013bf1919486dab56a4436a17a3878341a1d5>

# Bugs & Feature Request

Please report any bug & feature request to <https://github.com/chewing/chewing-editor/issues>

# Author & Contributors

*   ChangZhuo Chen (陳昌倬)
*   鄭鴻旗
*   Mike Tzou
*   Yao-Po Wang
*   Peter Dave Hello
*   David Kuo
*   Bruce Liu

# Licenses

*   This project is licensed under [GPL-2.0](https://opensource.org/licenses/GPL-2.0) or any later version.
*   [Google C++ Mocking Framework](https://code.google.com/p/googlemock/) is licensed under [New BSD License](https://opensource.org/licenses/BSD-3-Clause)
