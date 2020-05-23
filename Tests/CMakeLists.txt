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

add_executable(Performance
  Performance/Logger.cpp
  Performance/Logger.hpp
  Performance/Parameters.cpp
  Performance/Parameters.hpp
  Performance/perf.cpp
  Performance/TestHarness.cpp
  Performance/TestHarness.hpp
  Performance/Timer.hpp
  Performance/Utils.cpp
  Performance/Utils.hpp
  Performance/XalanCProcessor.cpp
  Performance/XalanCProcessor.hpp
  )
target_link_libraries(Performance XalanC::XalanC)
set_target_properties(Performance PROPERTIES FOLDER "Tests")

add_executable(Threads
  Threads/ThreadTest.cpp)
target_link_libraries(Threads XalanC::XalanC Threads::Threads)
set_target_properties(Threads PROPERTIES FOLDER "Tests")

add_executable(Conf
  Conf/conf.cpp)
target_link_libraries(Conf XalanC::XalanC)
set_target_properties(Conf PROPERTIES FOLDER "Tests")

foreach(test Threads)
  add_test(
    NAME ${test}
    COMMAND $<TARGET_FILE:${test}>
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${test}")
  if(msgloader STREQUAL "nls")
    set_tests_properties(${test} PROPERTIES ENVIRONMENT "NLSPATH=${PROJECT_BINARY_DIR}/src/xalanc/NLS/gen/Xalan.cat")
  endif()
  if(WIN32)
    string(REPLACE ";" "\\;" OLDPATH "$ENV{PATH}")
    set_tests_properties(${test} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:xalan-c>>\;$<SHELL_PATH:$<TARGET_FILE_DIR:xalanMsg>>\;${OLDPATH}")
  endif()
endforeach()
