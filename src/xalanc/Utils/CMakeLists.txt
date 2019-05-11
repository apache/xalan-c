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

set(GEN_SUPPORTDIR "${PROJECT_BINARY_DIR}/src/xalanc/PlatformSupport")
file(MAKE_DIRECTORY "${GEN_SUPPORTDIR}")

set(XSL_NLS_SRC_DIR "${PROJECT_SOURCE_DIR}/src/xalanc/NLS")
set(XSL_NLS_BIN_DIR "${PROJECT_BINARY_DIR}/src/xalanc/NLS")
set(XSL_NLS_INC_DIR "${PROJECT_BINARY_DIR}/src/xalanc/NLS/include")
set(XSL_NLS_GEN_DIR "${PROJECT_BINARY_DIR}/src/xalanc/NLS/gen")
file(MAKE_DIRECTORY "${XSL_NLS_LIB_DIR}")
file(MAKE_DIRECTORY "${XSL_NLS_GEN_DIR}")
file(MAKE_DIRECTORY "${XSL_NLS_INC_DIR}")

set(MSGFILENAME XalanMsg_${LOCALE})

set(XLIFF_FILE_NAME ${XSL_NLS_SRC_DIR}/${LOCALE}/${MSGFILENAME}.xlf)

set(GENMSGINDEX "${XSL_NLS_GEN_DIR}/LocalMsgIndex.hpp")
set(GENMSGDATA "${XSL_NLS_GEN_DIR}/LocalMsgData.hpp")
set(INCLUDEMSGDATA "${XSL_NLS_INC_DIR}/LocalMsgData.hpp")
set(SUPPORTMSGINDEX "${GEN_SUPPORTDIR}/LocalMsgIndex.hpp")

add_custom_target(locale ALL)
set_target_properties(locale PROPERTIES FOLDER "Message Library")

if(msgloader STREQUAL "inmemory")
  add_custom_command(
    OUTPUT "${GENMSGINDEX}"
           "${GENMSGDATA}"
    COMMAND "$<TARGET_FILE:MsgCreator>"
           "${XLIFF_FILE_NAME}"
           -TYPE inmem
           -LOCALE ${LOCALE}
    WORKING_DIRECTORY "${XSL_NLS_GEN_DIR}"
    COMMENT "Generating inmemory locale message data"
    DEPENDS MsgCreator "${XLIFF_FILE_NAME}")

  add_custom_command(
    OUTPUT "${INCLUDEMSGDATA}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${GENMSGDATA}" "${INCLUDEMSGDATA}"
    DEPENDS "${GENMSGDATA}"
    COMMENT "Creating ${INCLUDEMSGDATA}")

  add_custom_command(
    OUTPUT "${SUPPORTMSGINDEX}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${GENMSGINDEX}" "${SUPPORTMSGINDEX}"
    DEPENDS "${GENMSGINDEX}"
    COMMENT "Creating ${SUPPORTMSGINDEX}")

  add_custom_target(inmemory-dependencies
    DEPENDS "${INCLUDEMSGDATA}" "${SUPPORTMSGINDEX}")
  add_dependencies(locale inmemory-dependencies)
  set_target_properties(inmemory-dependencies PROPERTIES FOLDER "Message Library")
endif()

# Note that ICU message loading is broken due to pkgdata no longer
# generating DLLs.  This support was removed some time ago; what
# support remains is broken.
if(msgloader STREQUAL "icu")
  # Do we need a configuration-specific location for the generated
  # package?
  set(XSL_NLS_LIB_DIR "${PROJECT_SOURCE_DIR}/src/xalanc/NLS/lib/$<CONFIG>")
  set(GENLOCALETXT "${XSL_NLS_GEN_DIR}/${LOCALE}.txt")
  set(GENRESFILELIST "${XSL_NLS_GEN_DIR}/res-file-list.txt")
  set(RESFILE "${LOCALE}.res")
  set(GENRESFILE "${XSL_NLS_GEN_DIR}/${RESFILE}")
  set(LOC_PROJECT_NAME xalanMsg)

  file(WRITE "${GENRESFILELIST}" "${GENRESFILE}")

  add_custom_command(
    OUTPUT "${GENMSGINDEX}"
           "${GENLOCALETXT}"
    COMMAND "$<TARGET_FILE:MsgCreator>"
            "${XLIFF_FILE_NAME}"
            -TYPE icu
            -LOCALE ${LOCALE}
    WORKING_DIRECTORY "${XSL_NLS_GEN_DIR}"
    COMMENT "Generating ICU locale message data"
    DEPENDS MsgCreator "${XLIFF_FILE_NAME}")

  add_custom_command(
    OUTPUT "${GENRESFILE}"
    COMMAND "${ICU_GENRB_EXECUTABLE}"
            -d "${XSL_NLS_GEN_DIR}"
            -i "${XSL_NLS_GEN_DIR}"
            "${GENLOCALETXT}"
    WORKING_DIRECTORY "${XSL_NLS_GEN_DIR}"
    COMMENT "Generating ICU locale message resource"
    DEPENDS "${GENLOCALETXT}")

  # Needs to use generator for lib dir name
  add_custom_command(OUTPUT "testlibxxx"
    COMMAND "${ICU_PKGDATA_EXECUTABLE}"
            --name "${LOC_PROJECT_NAME}"
            -T "${XSL_NLS_GEN_DIR}"
            -v
            -O "${ICU_MAKEFILE_INC}"
            --mode dll
            -d "${XSL_NLS_LIB_DIR}"
            "${RESFILE}"
    WORKING_DIRECTORY "${XSL_NLS_GEN_DIR}"
    COMMENT "Generating ICU locale message data package"
    DEPENDS "${GENRESFILE}")

  # Further needed ICU actions...

  # Test ICU actions to this point; it's incomplete due to ICU pkgdata
  # not working.
  add_custom_target(icutest
    DEPENDS "testlibxxx")
  set_target_properties(icutest PROPERTIES FOLDER "Message Library")
endif()

if(msgloader STREQUAL "nls")
  set(CATALOG_NAME Xalan)
  set(CATALOG_MSG "${XSL_NLS_GEN_DIR}/${MSGFILENAME}.msg")
  set(CATALOG "${XSL_NLS_GEN_DIR}/${CATALOG_NAME}.cat")
  set(locale_dependencies "${CATALOG}")

  add_custom_command(
    OUTPUT "${CATALOG_MSG}"
    COMMAND "$<TARGET_FILE:MsgCreator>"
            "${XLIFF_FILE_NAME}"
            -TYPE nls
            -LOCALE ${LOCALE}
    WORKING_DIRECTORY "${XSL_NLS_GEN_DIR}"
    COMMENT "Generating nls locale message data"
    DEPENDS MsgCreator "${XLIFF_FILE_NAME}")

  add_custom_command(
    OUTPUT "${CATALOG}"
    COMMAND gencat "${CATALOG}" "${CATALOG_MSG}"
    DEPENDS "${CATALOG_MSG}"
    WORKING_DIRECTORY "${XSL_NLS_GEN_DIR}"
    COMMENT "Generating nls message catalog")

  add_custom_command(
    OUTPUT "${SUPPORTMSGINDEX}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${GENMSGINDEX}" "${SUPPORTMSGINDEX}"
    DEPENDS "${GENMSGINDEX}"
    COMMENT "Creating ${SUPPORTMSGINDEX}")

  add_custom_target(nls-dependencies DEPENDS "${CATALOG}" "${SUPPORTMSGINDEX}")
  add_dependencies(locale nls-dependencies)
  set_target_properties(nls-dependencies PROPERTIES FOLDER "Message Library")

  if(NOT XALAN_USING_GLIBC)
    install(FILES "${CATALOG}"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/nls/${LOCALE}"
      COMPONENT "runtime")
  else()
    install(FILES "${CATALOG}"
      DESTINATION "${CMAKE_INSTALL_LOCALEDIR}/${LOCALE}/LC_MESSAGES"
      COMPONENT "runtime")
  endif()
endif()

install(FILES "${SUPPORTMSGINDEX}"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xalanc/PlatformSupport"
  COMPONENT "development")
