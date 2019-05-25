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

# Option for selection of shared or static libraries, exported as
# cache variable

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

# Add a D postfix to Debug libraries on Windows

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "D")
endif()

# To prevent a lot of spam warnings about standard POSIX functions and
# unsafe use of the standard library.

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_SCL_SECURE_NO_WARNINGS=1)
endif()

# Versioning information

file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/../version.incl" xalan_verinfo
  REGEX "^[^#].*=.*")

foreach(item IN LISTS xalan_verinfo)
  string(REGEX REPLACE "^([^= ]+) *= *(.*)"
    "\\1" meta_key "${item}")
  string(REGEX REPLACE "^([^= ]+) *= *(.*)"
    "\\2" meta_value "${item}")
  set(XALAN_META_${meta_key} "${meta_value}")
endforeach()

string(REGEX REPLACE "^([0-9]+)_(.*)"
  "\\1"
  XALAN_META_MS_LIB_MAJOR_VER
  "${XALAN_META_MS_VER}")
string(REGEX REPLACE "^([0-9]+)_(.*)"
  "\\2"
  XALAN_META_MS_LIB_MINOR_VER
  "${XALAN_META_MS_VER}")
