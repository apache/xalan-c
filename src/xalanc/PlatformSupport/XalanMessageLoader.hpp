/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(XALANMESSAGELOADER_HEADER_GUARD_1357924680)
#define XALANMESSAGELOADER_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>


#include <xalanc/XalanDOM/XalanDOMString.hpp>

#include <xalanc/Include/LocalMsgIndex.hpp>


XALAN_CPP_NAMESPACE_BEGIN



// class for calling fom outside of the module 
class  XALAN_PLATFORMSUPPORT_EXPORT XalanMessageLoader {


public:

	
	// -----------------------------------------------------------------------
	//  Public Constructors and Destructor
	// -----------------------------------------------------------------------
	virtual
	~XalanMessageLoader();

	XalanMessageLoader()
	{
	}

	static void
	createLoader();

	static void
	destroyLoader();

	static const XalanDOMString
	getMessage(XalanMessages::Codes		msgToLoad
		, const char* 			repText1 
		, const char* 			repText2 = 0
		, const char* 			repText3 = 0
		, const char* 			repText4 = 0);

	static const XalanDOMString
	getMessage(XalanMessages::Codes		msgToLoad
		, const XalanDOMChar* 			repText1 
		, const XalanDOMChar* 			repText2 = 0
		, const XalanDOMChar* 			repText3 = 0
		, const XalanDOMChar* 			repText4 = 0);

	static const XalanDOMString
	getMessage(XalanMessages::Codes		msgToLoad);

	static const XalanDOMString
	getMessage(XalanMessages::Codes		msgToLoad
		, const XalanDOMString& 				repText1 );

	static const XalanDOMString
	getMessage(XalanMessages::Codes		msgToLoad
		, const XalanDOMString& 				repText1 
		, const XalanDOMString& 				repText2 );

	static const XalanDOMString
	getMessage(XalanMessages::Codes		msgToLoad
		, const XalanDOMString& 				repText1 
		, const XalanDOMString& 				repText2 
		, const XalanDOMString& 				repText3);

protected:
	virtual bool loadMsg
		(
		  XalanMessages::Codes		msgToLoad
		, XalanDOMChar*				toFill
		, unsigned int				maxChars
		) =0;

private:

	bool load
		(
		XalanMessages::Codes		msgToLoad
		, XalanDOMChar*				toFill
		, unsigned int				maxChars
		, const XalanDOMChar* 		repText1 
		, const XalanDOMChar* 		repText2 = 0
		, const XalanDOMChar* 		repText3 = 0
		, const XalanDOMChar* 		repText4 = 0
		);
	
	bool load
		(
		XalanMessages::Codes		msgToLoad
		, XalanDOMChar*				toFill
		, unsigned int				maxChars
		, const char* 				repText1 
		, const char* 				repText2 = 0
		, const char* 				repText3 = 0
		, const char* 				repText4 = 0
		);

	XalanMessageLoader(const XalanMessageLoader&);

	XalanMessageLoader&
	operator=(const XalanMessageLoader&);

	static XalanMessageLoader*	s_msgLoader;
};



XALAN_CPP_NAMESPACE_END

#endif	// XALANMESSAGELOADER_HEADER_GUARD_1357924680
