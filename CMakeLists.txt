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

    option(ENABLE_GCOV "Coverage support" false)
    if(ENABLE_GCOV)
        set(CMAKE_CXX_FLAGS "-coverage ${CMAKE_CXX_FLAGS}")
    endif()

    add_definitions(-DFUNC_NAME=__PRETTY_FUNCTION__)
else()
    add_definitions(-DFUNC_NAME=__func__)
endif()
add_definitions(-DTESTDATA="${PROJECT_SOURCE_DIR}/test/data")

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
    ${PROJECT_SOURCE_DIR}/gmock/gtest/include
    ${PROJECT_SOURCE_DIR}/src/exporter
    ${PROJECT_SOURCE_DIR}/src/importer
    ${PROJECT_SOURCE_DIR}/src/model
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/src/view
    ${Qt5Widgets_INCLUDES}
)

# exporter
file(GLOB_RECURSE exporter_src ${PROJECT_SOURCE_DIR}/src/exporter/*)
add_library(exporter STATIC ${exporter_src})
qt5_use_modules(exporter Widgets)

# importer
file(GLOB_RECURSE importer_src ${PROJECT_SOURCE_DIR}/src/importer/*)
add_library(importer STATIC ${importer_src})
qt5_use_modules(importer Widgets)

# ui
file(GLOB ui_src ${PROJECT_SOURCE_DIR}/src/ui/*)
qt5_wrap_ui(ui ${ui_src})

# util
file(GLOB util_src ${PROJECT_SOURCE_DIR}/src/util/*)
add_library(util STATIC ${util_src})
qt5_use_modules(util Widgets)

# chewing-editor
file(GLOB chewing-editor_src
    ${PROJECT_SOURCE_DIR}/src/*
    ${PROJECT_SOURCE_DIR}/src/model/*
    ${PROJECT_SOURCE_DIR}/src/ui/*
    ${PROJECT_SOURCE_DIR}/src/view/*
    ${ui}
)
add_executable(chewing-editor ${chewing-editor_src})
target_link_libraries(chewing-editor
    ${CHEWING_LIBRARY}
    exporter
    importer
    util
)
qt5_use_modules(chewing-editor Widgets)

# i18n
find_program(LUPDATE lupdate)
if (LUPDATE)
    foreach(ui_file ${ui})
        get_filename_component(ui_file_name "${ui_file}" NAME_WE)

        foreach(tr "zh_TW")
            set(ts "${PROJECT_SOURCE_DIR}/ts/${tr}/${ui_file_name}.ts")

            add_custom_command(TARGET chewing-editor POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/ts/${tr}
                COMMAND ${LUPDATE} ${ui_file} -ts ${ts}
            )

        endforeach()
    endforeach()
endif()

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
    gmock

    exporter
    importer
    util
)
qt5_use_modules(run-test Widgets)

add_test(test run-test)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check run-test)

install(PROGRAMS ${CMAKE_BINARY_DIR}/chewing-editor DESTINATION bin)
