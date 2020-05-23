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

# encoding selection

set(xalan_encodings ascii ebcdic)
set(xalan_encoding_default ascii)

if(CMAKE_SYSTEM_NAME STREQUAL "OS390")
    set(xalan_encoding_default ebcdic)
endif()

string(REPLACE ";" "|" xalan_encoding_help "${xalan_encodings}")
list(GET xalan_encodings 0 xalan_encoding_default)
set(system-encoding "${xalan_encoding_default}" CACHE STRING "System character encoding (${xalan_encoding_help})")

list(FIND xalan_encodings "${system-encoding}" xalan_encoding_found)
if(xalan_encoding_found EQUAL -1)
    message(FATAL_ERROR "${system-encoding} encoding unavailable")
endif()

set(XALAN_NON_ASCII_PLATFORM 0)
if(system-encoding STREQUAL "ebcdic")
    set(XALAN_NON_ASCII_PLATFORM 1)
endif()
