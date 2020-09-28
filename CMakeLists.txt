cmake_minimum_required(VERSION 2.8)

project(pushNode)

include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

set(PROJECT_NAME pushNode)
set(OpenGL_PATH /System/Library/Frameworks/OpenGL.framework)

set(SOURCE_FILES
    pushNode.h
    pushNode.cpp
    mainPlugin.cpp

)

set(LIBRARIES
    OpenMaya
    Foundation
    OpenMayaAnim
)

build_plugin()
