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
#if !defined(OS400DEFINITIONS_HEADER_GUARD_1357924680)
#define OS400DEFINITIONS_HEADER_GUARD_1357924680



// ---------------------------------------------------------------------------
//  A define in the build for each project is also used to control whether
//  the export keyword is from the project's viewpoint or the client's.
//  These defines provide the platform specific keywords that they need
//  to do this.
// ---------------------------------------------------------------------------

#define XALAN_PLATFORM_EXPORT
#define XALAN_PLATFORM_IMPORT
#define XALAN_PLATFORM_EXPORT_FUNCTION(T) T XALAN_PLATFORM_EXPORT
#define XALAN_PLATFORM_IMPORT_FUNCTION(T) T XALAN_PLATFORM_IMPORT


#define XALAN_LSTRSUPPORT
#if !defined(XML_BITSTOBUILD_64)
#define XALAN_USE_WCHAR_CAST_HACK
#endif
#define XALAN_POSIX2_AVAILABLE
#define XALAN_XALANDOMCHAR_USHORT_MISMATCH

#if __IBMCPP__ > 366

#define XALAN_EXPLICIT_SCOPE_IN_TEMPLATE_BUG
#define XALAN_NEW_STD_ALLOCATOR
#define XALAN_HAS_CPP_NAMESPACE

#else

#define XALAN_OLD_STYLE_CASTS
#define XALAN_CLASSIC_IOSTREAMS
#define XALAN_NO_STD_NAMESPACE
#define XALAN_NO_MUTABLE
#define XALAN_SGI_BASED_STL
#define XALAN_NO_MEMBER_TEMPLATES
#define XALAN_AMBIGUOUS_EVEN_IF_NOT_CALLED
#define XALAN_CANNOT_DELETE_CONST
#define XALAN_BOOL_AS_INT
#define XALAN_NO_STD_ALLOCATORS
#define XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION
#define XALAN_NO_ALGORITHMS_WITH_BUILTINS
#define XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS
#define XALAN_NO_COVARIANT_RETURN_TYPE
#define XALAN_AUTO_PTR_REQUIRES_DEFINITION
#define XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION
#define XALAN_STLPORT_STL
#define XALAN_TEMPLATE_FUNCTION_NO_DEFAULT_PARAMETERS
#define XALAN_NO_USING_DECLARATION

// STL Port Definitions
#define _REENTRANT
#define __STL_NO_SGI_IOSTREAMS
#include <stl/_config.h>

#endif


#endif	// OS400DEFINITIONS_HEADER_GUARD_1357924680
