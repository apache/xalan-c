/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(PLATFORMDEFINITIONS_HEADER_GUARD_1357924680)
#define PLATFORMDEFINITIONS_HEADER_GUARD_1357924680

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
#elif defined(__DECCXX)
#include "TRU64Definitions.hpp"
#elif defined(__INTEL_COMPILER)
#include "IntelDefinitions.hpp"
#else
#error Unknown compiler!
#endif

#include "XalanVersion.hpp"

#if defined(__cplusplus)

// ---------------------------------------------------------------------------
// Define namespace symbols if the compiler supports it.
// ---------------------------------------------------------------------------
#if defined(XALAN_HAS_CPP_NAMESPACE)
	#define XALAN_CPP_NAMESPACE_BEGIN namespace XALAN_CPP_NAMESPACE {
	#define XALAN_CPP_NAMESPACE_END  }
	#define XALAN_CPP_NAMESPACE_USE using namespace XALAN_CPP_NAMESPACE;
	#define XALAN_CPP_NAMESPACE_QUALIFIER XALAN_CPP_NAMESPACE::
	#define XALAN_USING(NAMESPACE,NAME) using NAMESPACE :: NAME;
	#define XALAN_DECLARE_CLASS(NAMESPACE,NAME) namespace NAMESPACE { class NAME; }
	#define XALAN_DECLARE_STRUCT(NAMESPACE,NAME) namespace NAMESPACE { struct NAME; }

	namespace XALAN_CPP_NAMESPACE { }
	namespace xalanc = XALAN_CPP_NAMESPACE;
#else
	#define XALAN_CPP_NAMESPACE_BEGIN
	#define XALAN_CPP_NAMESPACE_END
	#define XALAN_CPP_NAMESPACE_USE
	#define XALAN_CPP_NAMESPACE_QUALIFIER
	#define XALAN_USING(NAMESPACE,NAME)
	#define XALAN_DECLARE_CLASS(NAMESPACE,NAME) class NAME;
	#define XALAN_DECLARE_STRUCT(NAMESPACE,NAME) struct NAME;
	#if !defined(XALAN_NO_STD_NAMESPACE)
		#define XALAN_NO_STD_NAMESPACE
	#endif
#endif


#if defined(XALAN_NO_STD_NAMESPACE)
	#define XALAN_USING_STD(NAME)
	#define XALAN_STD_QUALIFIER 
#else
	#define XALAN_USING_STD(NAME) using std :: NAME;
	#define XALAN_STD_QUALIFIER  std ::
#endif

#define XALAN_DECLARE_XALAN_CLASS(NAME) XALAN_DECLARE_CLASS(XALAN_CPP_NAMESPACE, NAME)
#define XALAN_DECLARE_XALAN_STRUCT(NAME) XALAN_DECLARE_STRUCT(XALAN_CPP_NAMESPACE, NAME)
#define XALAN_USING_XALAN(NAME) XALAN_USING(XALAN_CPP_NAMESPACE, NAME)



// Yuck!!!! Have to include this here because there's no way to handle
// the new namespace macros without it!
#include "xercesc/util/XercesDefs.hpp"

#if defined(XERCES_HAS_CPP_NAMESPACE)
	#define XALAN_USING_XERCES(NAME) XALAN_USING(XERCES_CPP_NAMESPACE, NAME)
	#define XALAN_DECLARE_XERCES_CLASS(NAME) XALAN_DECLARE_CLASS(XERCES_CPP_NAMESPACE, NAME)
	#define XALAN_DECLARE_XERCES_STRUCT(NAME) XALAN_DECLARE_STRUCT(XERCES_CPP_NAMESPACE, NAME)
#else
	#define XERCES_CPP_NAMESPACE_QUALIFIER
	#define XERCES_CPP_NAMESPACE_BEGIN
	#define XERCES_CPP_NAMESPACE_END
	#define XERCES_CPP_NAMESPACE_USE
	#define XALAN_USING_XERCES(NAME)
	#define XALAN_DECLARE_XERCES_CLASS(NAME) class NAME;
	#define XALAN_DECLARE_XERCES_STRUCT(NAME) struct NAME;
#endif



#endif // __cplusplus



#endif	// PLATFORMDEFINITIONS_HEADER_GUARD_1357924680
