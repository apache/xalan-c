# CMake build for xalan-c
#
# Written by Roger Leigh <rleigh@codelibre.net>
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Run "cmake" to generate the build files for your platform

cmake_minimum_required(VERSION 3.2.0)

# Use new variable expansion policy.
if (POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW)
endif(POLICY CMP0053)
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif(POLICY CMP0054)
if (POLICY CMP0067)
  cmake_policy(SET CMP0067 NEW)
endif(POLICY CMP0067)

# Try C++14, then fall back to C++11 and C++98.  Used for feature tests
# for optional features.
set(CMAKE_CXX_STANDARD 14)

# Use folders (for IDE project grouping)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Source additional modules from the "cmake" directory
list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Project version
project(xalan-c
        VERSION 1.12.0
        LANGUAGES C CXX)

# Library version
set(XALAN_LIB_MAJOR_VER 112)
set(XALAN_LIB_MINOR_VER 0)
set(XALAN_MS_LIB_MAJOR_VER 1)
set(XALAN_MS_LIB_MINOR_VER 12)

message(STATUS "Configuring Apache Xalan-C++ version ${xalan-c_VERSION}")

enable_testing()
find_package(Threads REQUIRED)
include(Doxygen)
include(GNUInstallDirs)
include(XalanDLL)
include(XalanWarnings)
include(XalanFunctions)
include(XalanHeaders)
include(XalanWindows)
include(XalanICU)
include(XalanXerces)
include(XalanSystemEncodingSelection)
include(XalanThreadSelection)
include(XalanMsgLoaderSelection)
include(XalanTranscoderSelection)
include(XalanOperatorDelete)
include(XalanPathMax)
include(XalanLFS)

# Generate pkg-config file
set(pkgconfig-dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig" CACHE STRING "pkg-config installation directory (default ${CMAKE_INSTALL_LIBDIR}/pkgconfig)")
set(PKGCONFIGDIR "${pkgconfig-dir}")

set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "${CMAKE_INSTALL_PREFIX}")
set(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/xalan-c.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/xalan-c.pc)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/xalan-c.pc
  DESTINATION "${PKGCONFIGDIR}"
  COMPONENT "development")

# Process subdirectories
add_subdirectory(src/xalanc/Utils/MsgCreator)
add_subdirectory(src/xalanc/Utils)
add_subdirectory(src/xalanc/Utils/XalanMsgLib)
add_subdirectory(src/xalanc)
add_subdirectory(src/xalanc/TestXSLT)
add_subdirectory(src/xalanc/TestXPath)
add_subdirectory(samples)
add_subdirectory(Tests)
add_subdirectory(docs/doxygen)

# Display configuration summary
message(STATUS "")
message(STATUS "Xalan-C++ configuration summary")
message(STATUS "-------------------------------")
message(STATUS "")
message(STATUS "  Version:                     ${xalan-c_VERSION}")
if(MSVC)
message(STATUS "  Library major version:       ${XALAN_MS_LIB_MAJOR_VER}")
message(STATUS "  Library minor version:       ${XALAN_MS_LIB_MINOR_VER}")
else()
message(STATUS "  Library major version:       ${XALAN_LIB_MAJOR_VER}")
message(STATUS "  Library minor version:       ${XALAN_LIB_MINOR_VER}")
endif()
message(STATUS "")
message(STATUS "  Installation directory:      ${prefix}")
message(STATUS "  C compiler:                  ${CMAKE_C_COMPILER}")
message(STATUS "  C++ compiler:                ${CMAKE_CXX_COMPILER}")
message(STATUS "")
message(STATUS "  Build shared libraries:      ${BUILD_SHARED_LIBS}")
message(STATUS "  Thread implementation:       ${thread}")
message(STATUS "  Transcoder:                  ${transcoder}")
message(STATUS "  Message Loader:              ${msgloader}")
message(STATUS "  Message Loader Locale:       ${message-locale}")
