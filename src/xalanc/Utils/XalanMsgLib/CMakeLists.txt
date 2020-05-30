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

if(msgloader STREQUAL "inmemory")

  set(msglib_sources
    XalanMsgLib.cpp)

  set(msglib_headers
    XalanMsgLib.hpp
    XalanMsgLibDefinitions.hpp)

  set(msglib_resources
    XalanMsgLib.rc)

  add_library(xalanMsg ${msglib_sources} ${msglib_headers} ${msglib_resources})
  add_dependencies(xalanMsg locale)
  target_compile_definitions(xalanMsg PRIVATE "${MSGLOADER_DEF}=1")
  if(BUILD_SHARED_LIBS)
    target_compile_definitions(xalanMsg PRIVATE XALAN_XALANMSGLIB_BUILD_DLL=1)
  endif()
  target_include_directories(xalanMsg PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/xalanc/PlatformSupport>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/xalanc/NLS/include>
    $<INSTALL_INTERFACE:include/xalanc>)
  target_link_libraries(xalanMsg XercesC::XercesC)

  add_library(XalanC::XalanMsg ALIAS xalanMsg)

  if(MSVC)
    # Add configuration-specific library name to resource file.
    target_compile_definitions(xalanMsg PRIVATE "XALAN_DLL_NAME=\"$<TARGET_FILE_NAME:xalanMsg>\\0\"")
    # Different naming for Windows than for Unix builds for backward
    # compatibility with the existing project files.
    set_target_properties(xalanMsg PROPERTIES OUTPUT_NAME "XalanMsgLib_${XALAN_MS_LIB_MAJOR_VER}")
    set_target_properties(xalanMsg PROPERTIES RUNTIME_OUTPUT_NAME "XalanMessages_${XALAN_MS_LIB_MAJOR_VER}_${XALAN_MS_LIB_MINOR_VER}")
    set_target_properties(xalanMsg PROPERTIES DEBUG_POSTFIX "D")
  else()
    # Not used for the common cases, though this would be the default if
    # not for libtool compatibility.
    set_target_properties(xalanMsg PROPERTIES SOVERSION ${XALAN_LIB_MAJOR_VER})
    if(NOT CYGWIN)
      # This property causes shared libraries on Linux to have the full
      # version encoded into their final filename.  We disable this on
      # Cygwin because it causes xalanMsg-${XALAN_C_FULL_VERSION}.dll
      # to be created when xalanMsg.dll seems to be the default.
      set_target_properties(xalanMsg PROPERTIES VERSION ${XALAN_LIB_MAJOR_VER}.${XALAN_LIB_MINOR_VER})
    endif()
  endif()
  set_target_properties(xalanMsg PROPERTIES FOLDER "Message Library")

  install(TARGETS xalanMsg
    EXPORT XalanCConfigInternal
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT "runtime"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

endif()
