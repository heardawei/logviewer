# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(logviewer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "D:/install")
endif()

set(INSTALL_DIR "${INSTALL_DIR}/logviewer")

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Concurrent Widgets PrintSupport)

set(INC_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/ui/"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/observer/"
)

file(GLOB_RECURSE INC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

qt_add_executable(logviewer
    ${INC_FILES}
    ${SRC_FILES}
)

set_target_properties(logviewer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_include_directories(logviewer PRIVATE
  ${INC_DIRS}
)

target_link_libraries(logviewer PUBLIC
    Qt::Charts
    Qt::Core
    Qt::Gui
    Qt::Concurrent
    Qt::Widgets
    Qt::PrintSupport
)

target_compile_features(logviewer PRIVATE cxx_std_23)

install(TARGETS logviewer
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
