/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#if !defined(AIXDEFINITIONS_HEADER_GUARD_1357924680)
#define AIXDEFINITIONS_HEADER_GUARD_1357924680



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
#define XALAN_USE_WCHAR_CAST_HACK
#define XALAN_POSIX2_AVAILABLE
#define XALAN_BIG_ENDIAN
#define XALAN_XALANDOMCHAR_USHORT_MISMATCH

#if __IBMCPP__ > 366

#define XALAN_EXPLICIT_SCOPE_IN_TEMPLATE_BUG
#define XALAN_NEW_STD_ALLOCATOR

#else

#define XALAN_OLD_STYLE_CASTS
#define XALAN_OLD_STREAMS
#define XALAN_NO_NAMESPACES
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

// STL Port Definitions
#define _REENTRANT
#define __STL_NO_SGI_IOSTREAMS
#include <stl/_config.h>

#endif


#define XALAN_UNALIGNED


#endif	// AIXDEFINITIONS_HEADER_GUARD_1357924680
