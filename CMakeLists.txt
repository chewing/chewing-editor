cmake_minimum_required(VERSION 2.8)
project(chewing-editor)

find_package(Qt4 REQUIRED)

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
include_directories(${PROJECT_BINARY_DIR})

add_executable(chewing-editor
	${SOURCES}
	${HEADERS_MOC}
	${FORMS_MOC}
)
target_link_libraries(chewing-editor ${QT_LIBRARIES})