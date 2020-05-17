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

foreach(sample
        CompileStylesheet
        DocumentBuilder
        EntityResolver
        ExternalFunction
        ParsedSourceWrappers
        SimpleTransform
        StreamTransform
        ThreadSafe)
  add_test(NAME ${sample}
           COMMAND $<TARGET_FILE:${sample}>
           WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${sample}")
endforeach()

add_test(NAME SerializeNodeSet
         COMMAND $<TARGET_FILE:SerializeNodeSet> "foo.xml" "/doc" "name[@first='David']"
         WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SerializeNodeSet")

add_test(NAME SimpleXPathAPI-1
        COMMAND $<TARGET_FILE:SimpleXPathAPI> "foo.xml" "/foo:doc" "foo:name/@last"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SimpleXPathAPI")

add_test(NAME SimpleXPathAPI-2
        COMMAND $<TARGET_FILE:SimpleXPathAPI> "foo.xml" "/" "//foo:name[position()=\"4\"]/@first"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SimpleXPathAPI")

add_test(NAME TraceListen-1
        COMMAND $<TARGET_FILE:TraceListen> "-tt"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/TraceListen")

add_test(NAME TraceListen-2
        COMMAND $<TARGET_FILE:TraceListen> "-tg"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/TraceListen")

add_test(NAME TraceListen-3
        COMMAND $<TARGET_FILE:TraceListen> "-ts"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/TraceListen")

add_test(NAME TraceListen-4
        COMMAND $<TARGET_FILE:TraceListen> "-ttc"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/TraceListen")

add_test(NAME TransformToXercesDOM
        COMMAND $<TARGET_FILE:TransformToXercesDOM> "birds.xml" "birds.xsl"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/TransformToXercesDOM")

add_test(NAME UseStylesheetParam
        COMMAND $<TARGET_FILE:UseStylesheetParam>
          "foo.xml" "foo.xslt" "foo.out"
          "-s" "stringA" "'This is a test string value'"
          "-n" "numberA"  "123.012345"
          "-d" "parmA" "parmA.xml"
          "-d" "parmB" "parmB.xml"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/UseStylesheetParam")

add_test(NAME XalanTransform
        COMMAND $<TARGET_FILE:XalanTransform> "foo.xml" "foo.xsl"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/XalanTransform")

add_test(NAME XalanTransformerCallback
        COMMAND $<TARGET_FILE:XalanTransformerCallback> "foo.xml" "foo.xsl"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/XalanTransformerCallback")

add_test(NAME SimpleXPathCAPI
        COMMAND $<TARGET_FILE:SimpleXPathCAPI> "foo.xml" "/doc/name[3]"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/SimpleXPathCAPI")

foreach(sample
        CompileStylesheet
        DocumentBuilder
        EntityResolver
        ExternalFunction
        ParsedSourceWrappers
        SimpleTransform
        StreamTransform
        ThreadSafe
        SerializeNodeSet
        SimpleXPathAPI-1
        SimpleXPathAPI-2
        TraceListen-1
        TraceListen-2
        TraceListen-3
        TraceListen-4
        TransformToXercesDOM
        UseStylesheetParam
        XalanTransform
        XalanTransformerCallback
        SimpleXPathCAPI)
  if(msgloader STREQUAL "nls")
    set_tests_properties(${sample} PROPERTIES ENVIRONMENT "NLSPATH=${PROJECT_BINARY_DIR}/src/xalanc/NLS/gen/Xalan.cat")
  endif()
  if(WIN32)
    string(REPLACE ";" "\\;" OLDPATH "$ENV{PATH}")
    set_tests_properties(${sample} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:xalan-c>>\;$<SHELL_PATH:$<TARGET_FILE_DIR:xalanMsg>>\;${OLDPATH}")
  endif()
endforeach()
