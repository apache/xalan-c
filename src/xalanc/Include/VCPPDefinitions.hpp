/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(VCPPDEFINITIONS_HEADER_GUARD_1357924680)
#define VCPPDEFINITIONS_HEADER_GUARD_1357924680


#pragma warning(disable: 4127 4251 4511 4512 4514 4702 4710 4711 4786 4097 4503 4786; error: 4150 4172 4238 4239 4715)


// ---------------------------------------------------------------------------
//  A define in the build for each project is also used to control whether
//  the export keyword is from the project's viewpoint or the client's.
//  These defines provide the platform specific keywords that they need
//  to do this.
// ---------------------------------------------------------------------------
#define XALAN_PLATFORM_EXPORT     __declspec(dllexport)
#define XALAN_PLATFORM_IMPORT     __declspec(dllimport)
#define XALAN_PLATFORM_EXPORT_FUNCTION(T) XALAN_PLATFORM_EXPORT T
#define XALAN_PLATFORM_IMPORT_FUNCTION(T) XALAN_PLATFORM_IMPORT T


#if _MSC_VER >= 1300
  #define XALAN_INLINE_INITIALIZATION
  #if !defined(__INTEL_COMPILER)
    #define XALAN_INLINE_INITIALIZATION_IS_DEFINITION_BUG
   #endif
  #define XALAN_NEW_STD_ALLOCATOR
  #define XALAN_HAS_STD_ITERATORS
  #if _MSC_VER == 1300
    #define XALAN_VCPP_USE_PTRIT
  #endif
#else
  #define XALAN_REQUIRES_QUALIFIED_DESTRUCTOR
  #define XALAN_NO_COVARIANT_RETURN_TYPE
#endif

#define XALAN_LSTRSUPPORT
#define XALAN_NEWLINE_IS_CRLF
#define XALAN_HAS_CPP_NAMESPACE
#define XALAN_HAS_STD_DISTANCE
#define XALAN_NO_REENTRANT_TIME_FUNCTIONS


#endif	// VCPPDEFINITIONS_HEADER_GUARD_1357924680
