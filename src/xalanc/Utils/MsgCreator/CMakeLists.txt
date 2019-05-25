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

set(msgcreator_sources
  ICUResHandler.cpp
  InMemHandler.cpp
  MsgCreator.cpp
  MsgFileOutputStream.cpp
  NLSHandler.cpp
  SAX2Handler.cpp)

set(msgcreator_headers
  ICUResData.hpp
  ICUResHandler.hpp
  IndexFileData.hpp
  InMemData.hpp
  InMemHandler.hpp
  MsgCreator.hpp
  MsgFileOutputStream.hpp
  NLSData.hpp
  NLSHandler.hpp
  SAX2Handler.hpp)

add_executable(MsgCreator ${msgcreator_sources} ${msgcreator_headers})
target_include_directories(MsgCreator PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>)
target_link_libraries(MsgCreator XercesC::XercesC)
if(transcoder STREQUAL "icu")
  target_link_libraries(MsgCreator ICU::uc ICU::i18n)
endif()

set_target_properties(MsgCreator PROPERTIES FOLDER "Message Library")
