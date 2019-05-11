# CMake build for xerces-c
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

# message loader selection

# message loaders in order of preference
list(APPEND msgloaders inmemory)

include(CheckIncludeFileCXX)
include(CheckFunctionExists)
check_include_file_cxx(nl_types.h HAVE_NLTYPES_H)
check_function_exists(catopen HAVE_CATOPEN)
check_function_exists(catclose HAVE_CATCLOSE)
check_function_exists(catgets HAVE_CATGETS)
if(HAVE_NLTYPES_H AND HAVE_CATOPEN AND HAVE_CATCLOSE AND HAVE_CATGETS)
  set(nls_available 1)
endif()
if(nls_available)
  list(APPEND msgloaders nls-broken)
endif()

# Note ICU support is broken.
if(ICU_FOUND)
  list(APPEND msgloaders icu-broken)
endif()

string(REPLACE ";" "|" msgloader_help "${msgloaders}")
list(GET msgloaders 0 xerces_msgloader_default)
set(message-loader "${xerces_msgloader_default}" CACHE STRING "Message loader (${msgloader_help})")
set(msgloader "${message-loader}")

list(FIND msgloaders "${msgloader}" msgloader_found)
if(msgloader_found EQUAL -1)
  message(FATAL_ERROR "${msgloader} message loader unavailable")
endif()

list(FIND msgloaders ${msgloader} msgloader_valid)
if(msgloader_valid EQUAL -1)
  message(FATAL_ERROR "Invalid message loader: \"${msgloader}\"")
endif()

file(GLOB locale_files
  RELATIVE "${PROJECT_SOURCE_DIR}/src/xalanc/NLS/"
  "${PROJECT_SOURCE_DIR}/src/xalanc/NLS/*/*.xlf")
foreach(locale_file IN LISTS locale_files)
  get_filename_component(fc "${locale_file}" DIRECTORY)
  list(APPEND locales "${fc}")
endforeach()

string(REPLACE ";" "|" locale_help "${locales}")
set(message-locale "en_US" CACHE STRING "Message loader locale  (${locale_help})")
set(LOCALE "${message-locale}")
list(FIND locales "${message-locale}" locale_valid)
if(locale_valid EQUAL -1)
  message(FATAL_ERROR "Invalid message locale: \"${message-locale}\"")
endif()

if(msgloader STREQUAL "inmemory")
  set(MSGLOADER_DEF XALAN_INMEM_MSG_LOADER)
elseif(msgloader STREQUAL "icu-broken")
  set(msgloader "icu")
  set(MSGLOADER_DEF XALAN_ICU_MSG_LOADER)
elseif(msgloader STREQUAL "nls-broken")
  set(msgloader "nls")
  set(MSGLOADER_DEF XALAN_NLS_MSG_LOADER)

  check_cxx_source_compiles("
#include <stdlib.h>

#ifndef __GNU_LIBRARY__
#error Not glibc
#endif

int main() {
  return 0;
}"
  XALAN_USING_GLIBC)
endif()
