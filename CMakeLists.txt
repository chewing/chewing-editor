cmake_minimum_required(VERSION 2.8)
project(chewing-editor)

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")

    set(CMAKE_CXX_FLAGS "-std=c++11 -g -O2 ${CMAKE_CXX_FLAGS}")
endif()

find_package(Qt4 REQUIRED)

find_library(CHEWING_LIBRARY NAMES chewing)
# FIXME: How to find chewing header?
set(CHEWING_INCLUDE_DIR "/usr/include/chewing")

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/chewingeditor.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/src/chewingeditor.h
)

set(FORMS
    ${PROJECT_SOURCE_DIR}/src/chewingeditor.ui
)

qt4_wrap_cpp(HEADERS_MOC ${HEADERS})
qt4_wrap_ui(FORMS_MOC ${FORMS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(
    ${PROJECT_BINARY_DIR}
    ${CHEWING_INCLUDE_DIR}
)

add_executable(chewing-editor
    ${SOURCES}
    ${HEADERS_MOC}
    ${FORMS_MOC}
)
target_link_libraries(chewing-editor ${QT_LIBRARIES} ${CHEWING_LIBRARY})
