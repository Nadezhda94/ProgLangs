project(task1)
cmake_minimum_required(VERSION 3.0)
find_package(Qt5 COMPONENTS Core Widgets)
set (CMAKE_CXX_STANDARD 11)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    widgets.cpp main.c
    )

set(HEADERS widgets.h widgets_defs.h)

add_executable(
    task1
    main.c widgets.cpp
    )

add_library(task1_lib SHARED widgets.cpp)
add_definitions(-DQT_NO_KEYWORDS)

target_link_libraries(task1
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    )

target_link_libraries(task1_lib
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    )

#set(PYTHON_INCLUDE_DIR /Users/hope/anaconda2/envs/py36/include/python3.6m)
#set(PYTHON_LIBRARY /Users/hope/anaconda2/envs/py36/lib/libpython3.6m.dylib)
#set(PYTHON_PATH, /Users/hope/anaconda2/envs/py36/bin/python)
find_package(PythonLibs)

include_directories(${PYTHON_INCLUDE_DIRS})
python_add_module(
    _pywidgets
    py_widgets.cpp
    )

target_link_libraries(_pywidgets ${PYTHON_LIBRARIES} task1_lib)

python_add_module(
    pywidgets
    py_widgets_classes.cpp
    )
target_link_libraries(pywidgets ${PYTHON_LIBRARIES} task1_lib)
