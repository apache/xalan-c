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
#if !defined(HARNESSDEFINITIONS_HEADER_GUARD_1357924680)
#define HARNESSDEFINITIONS_HEADER_GUARD_1357924680

#include <xalanc/Include/PlatformDefinitions.hpp>

#if defined(HARNESS_EXPORTS)
#	define HARNESS_API XALAN_PLATFORM_EXPORT
#	define HARNESS_API_FUNCTION(T) XALAN_PLATFORM_EXPORT_FUNCTION(T)
#else
#	define HARNESS_API XALAN_PLATFORM_IMPORT
#	define HARNESS_API_FUNCTION(T) XALAN_PLATFORM_IMPORT_FUNCTION(T)
#endif


#endif	// HARNESSDEFINITIONS_HEADER_GUARD_1357924680
