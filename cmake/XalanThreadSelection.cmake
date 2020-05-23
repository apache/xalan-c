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

# thread selection

set(xalan_thread_default std)

set(THREADS_PREFER_PTHREAD_FLAG ON)
add_definitions(-D_THREAD_SAFE=1)
find_package(Threads)

include(CheckCXXSourceCompiles)

function(thread_test outvar)
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

  check_cxx_source_compiles(
"#include <thread>
#include <mutex>
#include <iostream>

namespace
{

  std::mutex m1;
  std::recursive_mutex m2;

  void
  threadmain()
  {
    std::lock_guard<std::mutex> lock1(m1);
    std::lock_guard<std::recursive_mutex> lock2(m2);
    std::cout << \"In thread\" << std::endl;
  }

}

int main() {
  std::thread foo(threadmain);
  foo.join();

  return 0;
}"
${outvar})

  set(${outvar} ${${outvar}} PARENT_SCOPE)
endfunction(thread_test)

thread_test(XALAN_HAVE_STD_THREAD)
if(XALAN_HAVE_STD_THREAD)
  list(APPEND threads standard)
endif()

if(TARGET Threads::Threads)
  if(WIN32)
    list(APPEND threads windows)
  else()
    list(APPEND threads posix)
    set(HAVE_PTHREAD 1)
  endif()
endif()

string(REPLACE ";" "|" thread_help "${threads}")
list(GET threads 0 xalan_thread_default)
set(thread "${xalan_thread_default}" CACHE STRING "Thread implementation (${thread_help})")

list(FIND threads "${thread}" thread_found)
if(thread_found EQUAL -1)
  message(FATAL_ERROR "${thread} thread unavailable")
endif()

set(XALAN_USE_THREAD_STD 0)
set(XALAN_USE_THREAD_POSIX 0)
set(XALAN_USE_THREAD_WINDOWS 0)
if(thread STREQUAL "standard")
  set(XALAN_USE_THREAD_STD 1)
elseif(thread STREQUAL "posix")
  set(XALAN_USE_THREAD_POSIX 1)
elseif(thread STREQUAL "windows")
  set(XALAN_USE_THREAD_WINDOWS 1)
else()
  message(FATAL_ERROR "Invalid thread implementation: \"${thread}\"")
endif()
