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

cmake_minimum_required(VERSION 3.10.0...3.31.6)
project(chewing-editor VERSION 0.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-g -O2 ${CMAKE_CXX_FLAGS}")

    option(ENABLE_GCOV "Coverage support" false)
    if(ENABLE_GCOV)
        set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}")
    endif()

    find_program(VALGRIND valgrind)
    if (VALGRIND)
        option(USE_VALGRIND "Use valgrind when testing" true)
    endif()
elseif(MSVC)
    # MBCS is the default in msvc, overwrite to unicode
    add_definitions(-DUNICODE -D_UNICODE)
endif()

add_definitions(-DTESTDATA="${PROJECT_SOURCE_DIR}/test/data" -DQT_DISABLE_DEPRECATED_BEFORE=0x050e00)

find_package(PkgConfig)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools)

# libchewing
# MODIFY IF NEEDED, e.g. ${CMAKE_CURRENT_SOURCE_DIR}/libchewing/lib/
set(CHEWING_LIBRARY_DIRS )
# MODIFY IF NEEDED, e.g. ${CMAKE_CURRENT_SOURCE_DIR}/libchewing/lib/*.lib
set(CHEWING_LIBRARIES )
# MODIFY IF NEEDED, e.g. ${CMAKE_CURRENT_SOURCE_DIR}/libchewing/include/chewing
set(CHEWING_INCLUDE_DIRS )

if (CHEWING_INCLUDE_DIRS AND CHEWING_LIBRARIES AND CHEWING_LIBRARY_DIRS)
    # if all listed variables are TRUE
    find_package_handle_standard_args(chewing DEFAULT_MSG CHEWING_LIBRARIES CHEWING_INCLUDE_DIRS)
    mark_as_advanced(CHEWING_LIBRARIES CHEWING_INCLUDE_DIRS)
else()
    pkg_check_modules(CHEWING REQUIRED chewing>=0.4.0)
endif()

pkg_check_modules(GTEST REQUIRED gtest>=1.7.0 IMPORTED_TARGET)

# Qt library
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_VERSION 5)

# Git describe
find_package(Git)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git/HEAD")
    execute_process(
        COMMAND
            ${GIT_EXECUTABLE}
            describe
            --tags
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
        RESULT_VARIABLE
            git_describe_result
        OUTPUT_VARIABLE
            PROJECT_GIT_VERSION
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

configure_file(
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/include/config.h
)

install(
    FILES ${PROJECT_SOURCE_DIR}/chewing-editor.desktop
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)

include_directories(
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/exporter
    ${PROJECT_SOURCE_DIR}/src/importer
    ${PROJECT_SOURCE_DIR}/src/model
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/src/view
    ${CHEWING_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDES}
)

link_directories(${CHEWING_LIBRARY_DIRS})

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform_apple.cmake)

# resources
QT5_ADD_RESOURCES(qt_resources ${PROJECT_SOURCE_DIR}/rc/resources.qrc)

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
        COMMAND ${Qt5_LUPDATE_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src -ts ${TS_FILE}
        DEPENDS prepare_lupdate
    )
    add_dependencies(lupdate "${TRANSLATION}-ts")

    add_custom_command(
        OUTPUT
            ${QM_FILE}
        COMMAND ${Qt5_LRELEASE_EXECUTABLE} ${TS_FILE} -qm ${QM_FILE}
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
target_link_libraries(exporter Qt5::Widgets)

# importer
file(GLOB_RECURSE importer_src ${PROJECT_SOURCE_DIR}/src/importer/*)
add_library(importer STATIC ${importer_src})
target_link_libraries(importer Qt5::Widgets)

# ui
file(GLOB ui_src ${PROJECT_SOURCE_DIR}/src/ui/*)
qt5_wrap_ui(ui ${ui_src})

# util
file(GLOB util_src ${PROJECT_SOURCE_DIR}/src/util/*)
add_library(util STATIC ${util_src})
target_link_libraries(util Qt5::Widgets)

# chewing-editor
file(GLOB chewing-editor_src
    ${PROJECT_SOURCE_DIR}/src/*
    ${PROJECT_SOURCE_DIR}/src/model/*
    ${PROJECT_SOURCE_DIR}/src/ui/*
    ${PROJECT_SOURCE_DIR}/src/view/*
    ${ui}
)
add_executable(chewing-editor WIN32 ${MACOSX_APP} ${chewing-editor_src} ${qt_resources} ${PROJECT_SOURCE_DIR}/rc/chewing-editor.rc) # on Windows, remove "WIN32" for a console window
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

target_link_libraries(chewing-editor Qt5::Widgets)
install(PROGRAMS ${CMAKE_BINARY_DIR}/chewing-editor DESTINATION ${CMAKE_INSTALL_BINDIR})

# icon
install(FILES ${CMAKE_SOURCE_DIR}/share/icons/chewing-editor.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
install(FILES ${CMAKE_SOURCE_DIR}/rc/chewing-editor.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps)

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
                -n -f
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

file(GLOB run-test_src
    test/*.cpp
)
add_executable(run-test
    ${run-test_src}
)
target_link_libraries(run-test
    PkgConfig::GTEST

    exporter
    importer
    util
    pthread
)
target_link_libraries(run-test Qt5::Widgets)

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
    "^${PROJECT_SOURCE_DIR}/.jj"
    "^${PROJECT_SOURCE_DIR}/build"
)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

include(CPack)
