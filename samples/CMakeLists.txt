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

add_custom_target(samples COMMENT Build sample programs)
set_target_properties(samples PROPERTIES FOLDER "Samples")

# ApacheModuleXSLT?
foreach(sample
    CompileStylesheet
    DocumentBuilder
    EntityResolver
    ExternalFunction
    ParsedSourceWrappers
    SerializeNodeSet
    SimpleTransform
    SimpleXPathAPI
    StreamTransform
    TraceListen
    TransformToXercesDOM
    UseStylesheetParam
    XalanTransform
    XalanTransformerCallback
    ThreadSafe)
  add_executable(${sample} ${sample}/${sample}.cpp)
  target_link_libraries(${sample} XalanC::XalanC)
  set_target_properties(${sample} PROPERTIES FOLDER "Samples")
  add_dependencies(samples ${sample})
endforeach()

foreach(sample
    SimpleXPathCAPI)
  add_executable(${sample} ${sample}/${sample}.c)
  target_link_libraries(${sample} XalanC::XalanC)
  set_target_properties(${sample} PROPERTIES FOLDER "Samples")
  add_dependencies(samples ${sample})
endforeach()

add_test(NAME CompileStylesheet
        COMMAND $<TARGET_FILE:CompileStylesheet>
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/CompileStylesheet")

foreach(sample
        CompileStylesheet)
  if(msgloader STREQUAL "nls")
    set_tests_properties(${sample} PROPERTIES ENVIRONMENT "NLSPATH=${PROJECT_BINARY_DIR}/src/xalanc/NLS/gen/Xalan.cat")
  endif()
  if(WIN32)
    string(REPLACE ";" "\\;" OLDPATH "$ENV{PATH}")
    set_tests_properties(${sample} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:xalan-c>>\;$<SHELL_PATH:$<TARGET_FILE_DIR:xalanMsg>>\;${OLDPATH}")
  endif()
endforeach()
