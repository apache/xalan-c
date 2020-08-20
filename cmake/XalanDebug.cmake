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

set(xalan_debug_default OFF)
if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(xalan_debug_default ON)
endif()

set(XALAN_DEBUG ${xalan_debug_default} CACHE BOOL "Build with additional debug checks")


set(xalan_crt_debug_default OFF)
if(xalan_debug_default AND MSVC)
  set(xalan_crt_debug_default ON)
endif()

set(XALAN_CRT_DEBUG ${xalan_crt_debug_default} CACHE BOOL "Build with additional MSVC CRT debug checks (Windows only)")
