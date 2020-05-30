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

if(BUILD_DOXYGEN)
  set(DOXYGEN_LOG "${CMAKE_CURRENT_BINARY_DIR}/api.log")
  set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/api")
  set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/api.dox)
  set(DOXYFILE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/api.dox.in)
  set(XERCESC_TAGFILE_DEFAULT "xerces-c.tag")
  set(XERCESC_HTML_DEFAULT "http://xerces.apache.org/xerces-c/apiDocs-3/")

  set(XERCESC_DOXYGEN_TAGFILE "${XERCESC_TAGFILE_DEFAULT}" CACHE STRING "Tagfile for xerces-c documentation")
  set(XERCESC_DOXYGEN_HTML_LOCATION "${XERCESC_HTML_DEFAULT}" CACHE STRING "Base directory for xerces-c HTML documentation")

  if(EXISTS "${XERCESC_DOXYGEN_TAGFILE}")
    set(DOXYGEN_XERCESC_TAGFILE "${XERCESC_DOXYGEN_TAGFILE}=${XERCESC_DOXYGEN_HTML_LOCATION}")
  endif()
  configure_file(${DOXYFILE_TEMPLATE} ${DOXYFILE} @ONLY)

  add_custom_command(OUTPUT ${DOXYGEN_LOG}
                     COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOXYGEN_OUTPUT_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
                     DEPENDS ${DOXYFILE} ${DOC_DEPENDENCIES})

  if(NOT TARGET doc-check)
    add_custom_target(doc-check)
  endif()
  add_custom_target(${PROJECT_NAME}-doc-check
                    COMMAND ${CMAKE_COMMAND} -Dlogfile=${DOXYGEN_LOG} -P ${PROJECT_SOURCE_DIR}/cmake/DoxygenCheck.cmake
                    DEPENDS ${DOXYGEN_LOG})
  add_dependencies(doc-check ${PROJECT_NAME}-doc-check)

  set_target_properties(${PROJECT_NAME}-doc-check PROPERTIES FOLDER "Documentation")
  set_target_properties(doc-check PROPERTIES FOLDER "Documentation")

  if(NOT TARGET doc-api)
    add_custom_target(doc-api)
  endif()
  add_custom_target(${PROJECT_NAME}-doc-api ALL DEPENDS ${DOXYGEN_LOG} ${PROJECT_NAME}-doc-check)
  add_dependencies(doc-api ${PROJECT_NAME}-doc-api)

  set_target_properties(${PROJECT_NAME}-doc-api PROPERTIES FOLDER "Documentation")
  set_target_properties(doc-api PROPERTIES FOLDER "Documentation")

  install(DIRECTORY "${DOXYGEN_OUTPUT_DIR}/"
          DESTINATION "${CMAKE_INSTALL_DOCDIR}/api"
          COMPONENT "development")
endif(BUILD_DOXYGEN)
