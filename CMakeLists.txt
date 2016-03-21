# chewing-editor: Chewing userphrase editor
# Copyright (C) 2014 Chewing Development Team
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

# override default c/c++ flags, add /MT to link VC++ runtime statically.
set(CMAKE_USER_MAKE_RULES_OVERRIDE	${CMAKE_CURRENT_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX	${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)

cmake_minimum_required(VERSION 3.0.0)
project(chewing-editor VERSION 0.0.2)

cmake_policy(SET CMP0020 NEW)

include(GNUInstallDirs)

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -g -O2 ${CMAKE_CXX_FLAGS}")

    option(ENABLE_GCOV "Coverage support" false)
    if(ENABLE_GCOV)
        set(CMAKE_CXX_FLAGS "-coverage ${CMAKE_CXX_FLAGS}")
    endif()

    find_program(VALGRIND valgrind)
    if (VALGRIND)
        option(USE_VALGRIND "Use valgrind when testing" true)
    endif()
elseif(MSVC)
    # MBCS is the default in msvc, overwrite to unicode
    add_definitions(-DUNICODE -D_UNICODE)
endif()

add_definitions(-DTESTDATA="${PROJECT_SOURCE_DIR}/test/data")

find_package(PkgConfig)
find_package(Qt5Widgets REQUIRED)

find_program(LUPDATE lupdate)
if (NOT LUPDATE)
    message(SEND_ERROR "Cannot find lupdate")
endif()
find_program(LRELEASE lrelease)
if (NOT LRELEASE)
    message(SEND_ERROR "Cannot find lrelease")
endif()

# libchewing
# MODIFY IF NEEDED, e.g. ${CMAKE_CURRENT_SOURCE_DIR}/libchewing/lib/*.lib
set(CHEWING_LIBRARIES )
# MODIFY IF NEEDED, e.g. ${CMAKE_CURRENT_SOURCE_DIR}/libchewing/include/chewing
set(CHEWING_INCLUDE_DIRS )

if (CHEWING_INCLUDE_DIRS AND CHEWING_LIBRARIES)
    # if all listed variables are TRUE
    find_package_handle_standard_args(chewing DEFAULT_MSG CHEWING_LIBRARIES CHEWING_INCLUDE_DIRS)
    mark_as_advanced(CHEWING_LIBRARIES CHEWING_INCLUDE_DIRS)
else()
    pkg_check_modules(CHEWING REQUIRED chewing>=0.4.0)
endif()

# Qt library
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


configure_file(
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/include/config.h
)

configure_file(
    ${PROJECT_SOURCE_DIR}/chewing-editor.desktop.in
    ${PROJECT_BINARY_DIR}/chewing-editor.desktop
)

install(
    FILES ${PROJECT_BINARY_DIR}/chewing-editor.desktop
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/application
)

include_directories(
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/gmock/gtest/include
    ${PROJECT_SOURCE_DIR}/src/exporter
    ${PROJECT_SOURCE_DIR}/src/importer
    ${PROJECT_SOURCE_DIR}/src/model
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/src/view
    ${CHEWING_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDES}
)

# i18n
set(TRANSLATION_LIST en_US zh_TW)

set(TS_DIR "${PROJECT_SOURCE_DIR}/ts")
set(QM_DIR "${PROJECT_SOURCE_DIR}/ts")

add_custom_target(prepare_lupdate
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TS_DIR}
)
add_custom_target(lupdate)

add_custom_target(prepare_lrelease
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DIR}
)

foreach(TRANSLATION ${TRANSLATION_LIST})
    set(TS_FILE "${TS_DIR}/${CMAKE_PROJECT_NAME}_${TRANSLATION}.ts")
    set(QM_FILE "${QM_DIR}/${CMAKE_PROJECT_NAME}_${TRANSLATION}.qm")

    add_custom_target("${TRANSLATION}-ts"
        COMMAND ${LUPDATE} ${PROJECT_SOURCE_DIR}/src -ts ${TS_FILE}
        DEPENDS prepare_lupdate
    )
    add_dependencies(lupdate "${TRANSLATION}-ts")

    add_custom_command(
        OUTPUT
            ${QM_FILE}
        COMMAND ${LRELEASE} ${TS_FILE} -qm ${QM_FILE}
        DEPENDS
            prepare_lrelease
            ${TS_FILE}
    )
    list(APPEND QM_FILES ${QM_FILE})
endforeach()

add_custom_target(lrelease ALL DEPENDS ${QM_FILES})
QT5_ADD_RESOURCES(qt_resources ${PROJECT_SOURCE_DIR}/ts/ts.qrc)

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
add_executable(chewing-editor WIN32 ${chewing-editor_src} ${qt_resources} ${PROJECT_SOURCE_DIR}/rc/chewing-editor.rc) # on Windows, remove "WIN32" for a console window
target_link_libraries(chewing-editor
    ${CHEWING_LIBRARIES}
    exporter
    importer
    util
)
if(MSVC)
    target_link_libraries(chewing-editor  # append libraries
        Qt5::Widgets
        "${_qt5Widgets_install_prefix}/lib/qtpcre.lib"
        "${_qt5Widgets_install_prefix}/lib/qtfreetype.lib"
        "${_qt5Widgets_install_prefix}/lib/qtharfbuzzng.lib"
        "${_qt5Widgets_install_prefix}/lib/Qt5PlatformSupport.lib"
        "${_qt5Widgets_install_prefix}/plugins/platforms/qwindows.lib"
        imm32.lib
        winmm.lib
        Ws2_32.lib
    )
endif()

qt5_use_modules(chewing-editor Widgets)
install(PROGRAMS ${CMAKE_BINARY_DIR}/chewing-editor DESTINATION ${CMAKE_INSTALL_BINDIR})

# icon
install(FILES ${CMAKE_SOURCE_DIR}/share/icons/chewing-editor.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)

# manpage
find_program(HELP2MAN help2man)
find_program(GZIP gzip)

if (HELP2MAN)
    set(manpage ${PROJECT_BINARY_DIR}/chewing-editor.1)
    set(h2m ${PROJECT_SOURCE_DIR}/chewing-editor.h2m)

    add_custom_command(
        OUTPUT
            ${manpage}
        COMMAND ${HELP2MAN}
            --include=${h2m}
            --section=1
            --no-info
            --no-discard-stderr
            --output=${manpage}
            ${PROJECT_BINARY_DIR}/chewing-editor
        DEPENDS
            chewing-editor
            ${h2m}
    )

    if (GZIP)
        add_custom_command(
            OUTPUT
                ${manpage}.gz
            COMMAND ${GZIP}
                ${manpage}
            DEPENDS
                ${manpage}
        )

        set(manpage ${manpage}.gz)
    endif()

    add_custom_target(manpage ALL DEPENDS ${manpage})

    install(FILES ${manpage} DESTINATION "${CMAKE_INSTALL_MANDIR}/man1/")
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
    pthread
)
qt5_use_modules(run-test Widgets)

add_test(test run-test)

if (USE_VALGRIND)
    add_test("valgrind-run-test" ${VALGRIND} --error-exitcode=255 --leak-check=full ${PROJECT_BINARY_DIR}/run-test)
endif()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check chewing-editor run-test)

# package
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
    "^${PROJECT_SOURCE_DIR}/.git"
)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

include(CPack)
