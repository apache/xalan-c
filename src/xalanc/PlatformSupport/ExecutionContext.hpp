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
#if !defined(EXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define EXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;



class XalanNode;
class XalanDOMString;


//
// An abstract class which provides support for execution.
//
class XALAN_PLATFORMSUPPORT_EXPORT ExecutionContext
{
public:

	explicit
	ExecutionContext();

	virtual
	~ExecutionContext();

	/**
	 * Report an error and throw an exception.
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the error occurred.  May be 0.
	 * @param locator A Locator to determine where the error occurred.  May be 0.
	 */
	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const = 0;

	/**
	 * Report an error and throw an exception.
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the error occurred.  May be 0.
	 * @param locator A Locator to determine where the error occurred.  May be 0.
	 */
	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const LocatorType* 	locator = 0) const = 0;

	/**
	 * Report a warning
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the warning occurred.  May be 0.
	 * @param locator A Locator to determine where the warning occurred.  May be 0.
	 */
	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const = 0;

	/**
	 * Report a warning
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the warning occurred.  May be 0.
	 * @param locator A Locator to determine where the warning occurred.  May be 0.
	 */
	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const LocatorType* 	locator = 0) const = 0;

	/**
	 * Output a message.
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the message occurred.  May be 0.
	 * @param locator A Locator to determine where the message occurred.  May be 0.
	 */
	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const = 0;

	/**
	 * Output a message.
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the message occurred.  May be 0.
	 * @param locator A Locator to determine where the message occurred.  May be 0.
	 */
	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const LocatorType* 	locator = 0) const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif	// EXECUTIONCONTEXT_HEADER_GUARD_1357924680
