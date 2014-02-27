# chewing-editor: Chewing userphrase editor
# Copyright (C) 2014 ChangZhuo Chen
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

cmake_minimum_required(VERSION 2.8.9)
project(chewing-editor)

cmake_policy(SET CMP0020 NEW)

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -g -O2 ${CMAKE_CXX_FLAGS}")

    add_definitions(-DFUNC_NAME=__PRETTY_FUNCTION__)
else()
    add_definitions(-DFUNC_NAME=__func__)
endif()

if (NOT DEFINED CHEWING_LIBRARY)
    find_library(CHEWING_LIBRARY NAMES chewing)
endif()
if (NOT CHEWING_LIBRARY)
    message(SEND_ERROR "Please set CHEWING_LIBRARY to location of chewing library.")
endif()

if (NOT DEFINED CHEWING_INCLUDE_DIR)
    # FIXME: How to find chewing header?
    set(CHEWING_INCLUDE_DIR "/usr/include/chewing")
endif()
if (NOT CHEWING_INCLUDE_DIR)
    message(SEND_ERROR "Please set CHEWING_INCLUDE_DIR to location of chewing header.")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

include_directories(
    ${CHEWING_INCLUDE_DIR}
    ${Qt5Widgets_INCLUDES}
    ${PROJECT_SOURCE_DIR}/src/exporter
    ${PROJECT_SOURCE_DIR}/src/importer
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/userphrase
    ${PROJECT_SOURCE_DIR}/gmock/gtest/include
)

# UI

file(GLOB UI_src
    ${PROJECT_SOURCE_DIR}/src/ui/*.ui
)
qt5_wrap_ui(UI ${UI_src})

# exporter

file(GLOB_RECURSE exporter_src
    ${PROJECT_SOURCE_DIR}/src/exporter/*
)
add_library(exporter STATIC ${exporter_src})
qt5_use_modules(exporter Widgets)

# importer

file(GLOB_RECURSE importer_src
    ${PROJECT_SOURCE_DIR}/src/importer/*
)
add_library(importer STATIC ${importer_src})
qt5_use_modules(importer Widgets)

# chewing-editor

file(GLOB chewing-editor_src
    ${PROJECT_SOURCE_DIR}/src/*
    ${PROJECT_SOURCE_DIR}/src/userphrase/*
    ${PROJECT_SOURCE_DIR}/src/ui/*
)
add_executable(chewing-editor
    ${chewing-editor_src}
    ${UI}
)
target_link_libraries(chewing-editor
    exporter
    importer
    ${CHEWING_LIBRARY}
)
qt5_use_modules(chewing-editor Widgets)

# testing

enable_testing()
add_subdirectory(gmock)

file(GLOB run-test_src
    test/*.cpp
)
add_executable(run-test
    ${run-test_src}
)
target_link_libraries(run-test
    exporter
    importer
    gmock
)
qt5_use_modules(run-test Widgets)

add_test(test run-test)

install(PROGRAMS ${CMAKE_BINARY_DIR}/chewing-editor DESTINATION bin)
