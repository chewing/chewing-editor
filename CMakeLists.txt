cmake_minimum_required(VERSION 2.8.9)
project(chewing-editor)

cmake_policy(SET CMP0020 NEW)

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -g -O2 ${CMAKE_CXX_FLAGS}")
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
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/userphrase
    ${PROJECT_SOURCE_DIR}/gmock/gtest/include
)

qt5_wrap_ui(UI
    ${PROJECT_SOURCE_DIR}/src/ui/ChewingEditor.ui
)

add_executable(chewing-editor
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/ChewingEditor.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/ChewingEditor.h
    ${PROJECT_SOURCE_DIR}/src/ui/UserphraseView.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/UserphraseView.h
    ${PROJECT_SOURCE_DIR}/src/userphrase/DefaultUserphraseData.cpp
    ${PROJECT_SOURCE_DIR}/src/userphrase/DefaultUserphraseData.h
    ${PROJECT_SOURCE_DIR}/src/userphrase/UserphraseData.cpp
    ${PROJECT_SOURCE_DIR}/src/userphrase/UserphraseData.h
    ${PROJECT_SOURCE_DIR}/src/userphrase/UserphraseModel.cpp
    ${PROJECT_SOURCE_DIR}/src/userphrase/UserphraseModel.h
    ${PROJECT_SOURCE_DIR}/src/userphrase/UserphraseModelFactory.cpp
    ${PROJECT_SOURCE_DIR}/src/userphrase/UserphraseModelFactory.h
    ${UI}
)
target_link_libraries(chewing-editor ${CHEWING_LIBRARY})
qt5_use_modules(chewing-editor Widgets)

enable_testing()
add_subdirectory(gmock)

add_executable(run-test
    ${PROJECT_SOURCE_DIR}/test/testmain.cpp

    ${PROJECT_SOURCE_DIR}/test/testUserphrase.cpp
)
target_link_libraries(run-test gmock)

add_test(test run-test)

install(PROGRAMS ${CMAKE_BINARY_DIR}/chewing-editor DESTINATION bin)
