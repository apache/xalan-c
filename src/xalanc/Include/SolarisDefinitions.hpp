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
#if !defined(SOLARISDEFINITIONS_HEADER_GUARD_1357924680)
#define SOLARISDEFINITIONS_HEADER_GUARD_1357924680



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


#if __SUNPRO_CC >= 0x520

#define XALAN_HAS_CPP_NAMESPACE
#define XALAN_STRICT_ANSI_HEADERS

#else

// Sun Workshop 4.2 _does_ support new cast syntax, but it's
// broken with extra cv-qualifiers, so we'll use old-style casts.
#define XALAN_OLD_STYLE_CASTS

#define XALAN_NO_STD_NAMESPACE
#define XALAN_STLPORT_STL
#define XALAN_NO_DEFAULT_BUILTIN_ARRAY_INITIALIZATION
#define XALAN_CLASSIC_IOSTREAMS
#define XALAN_NO_MUTABLE
#define XALAN_SGI_BASED_STL
#define XALAN_NO_MEMBER_TEMPLATES
#define XALAN_BOOL_AS_INT
#define XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS
#define XALAN_NO_COVARIANT_RETURN_TYPE
#define XALAN_NO_USING_DECLARATION

// STL Port Definitions
#define __STL_NO_SGI_IOSTREAMS
#define _REENTRANT
#include <stl/_config.h>

#endif

// Standard allocators don't seem to work correctly with this compiler.
// I don't know if this is their bug, or ours...
#define XALAN_NO_STD_ALLOCATORS
#define XALAN_RW_NO_CLASS_PARTIAL_SPEC

#define XALAN_XALANDOMCHAR_USHORT_MISMATCH
#define XALAN_POSIX2_AVAILABLE
#define XALAN_NO_STD_NUMERIC_LIMITS

#endif	// SOLARISDEFINITIONS_HEADER_GUARD_1357924680
