/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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

#if !defined(PLATFORMDEFINITIONS_HEADER_GUARD_1357924680)
#define PLATFORMDEFINITIONS_HEADER_GUARD_1357924680

#if defined(__cplusplus)
# include "xercesc/util/XercesDefs.hpp"
#else
/* Include xercesc autoconf to define XMLCh when using a 'C' compiler. */
# include "xercesc/util/Xerces_autoconf_config.hpp"
# include "xercesc/util/XercesVersion.hpp"
#endif

#if defined(_MSC_VER)
#include "VCPPDefinitions.hpp"
#elif defined(__GNUC__)
#include "GCCDefinitions.hpp"
#elif defined(_AIX)
#include "AIXDefinitions.hpp"
#elif defined(__hpux)
#include "HPUXDefinitions.hpp"
#elif defined(SOLARIS)
#include "SolarisDefinitions.hpp"
#elif defined(OS390)
#include "OS390Definitions.hpp"
#elif defined(OS400)
#include "OS400Definitions.hpp"
#elif defined(__DECCXX)
#include "TRU64Definitions.hpp"
#elif defined(__INTEL_COMPILER)
#include "IntelDefinitions.hpp"
#elif defined(__IBMCPP__) && defined(__TOS_LINUX__)
// This is for Linux PPC using the IBM xlC compiler.
#include "AIXDefinitions.hpp"
#else
#error Unknown compiler!
#endif

#include "XalanVersion.hpp"

#if defined(__cplusplus)

// ---------------------------------------------------------------------------
// Define namespace symbols if the compiler supports it.
// ---------------------------------------------------------------------------
namespace XALAN_CPP_NAMESPACE { }
namespace xalanc = XALAN_CPP_NAMESPACE;


#include <cstddef>


namespace XALAN_CPP_NAMESPACE {

typedef XMLSize_t       XalanSize_t;
typedef XMLFilePos      XalanFilePos;
typedef XMLFileLoc      XalanFileLoc;

typedef XMLCh           XalanDOMChar;
typedef unsigned int    XalanUnicodeChar;
typedef XMLSSize_t      XalanSSize_t;

template<bool value>
struct XalanCompileErrorBoolean
{
    char    foo[value];
};
 
}


#define XALAN_STATIC_ASSERT(expr) xalanc::XalanCompileErrorBoolean<bool(expr)>()



#endif // __cplusplus



#endif  // PLATFORMDEFINITIONS_HEADER_GUARD_1357924680
