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
#if !defined(XOBJECT_TYPE_CALLBACK_HEADER_GUARD_1357924680)
#define XOBJECT_TYPE_CALLBACK_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



class DOMString;
class MutableNodeRefList;
class NodeRefListBase;
class ResultTreeFragBase;
class XObject;



/**
 * The purpose of this class is to provide a way to get the "preferred" or
 * closest matching type for XObject-derived classes.  The idea is to pass a
 * reference to an instance of this class to an XObject instance.  It will
 * then call back to the member function that most closely matches its type.
 * This makes it possible to get preferred typing information without doing
 * RTTI or other such type introspections.
 */
class XALAN_XPATH_EXPORT XObjectTypeCallback
{
public:

	XObjectTypeCallback();

	virtual
	~XObjectTypeCallback();

	/**
	 * Call back the XObject with a numeric value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Number(const XObject&	theXObject,
		   double			theValue) = 0;

	/**
	 * Call back the XObject with a boolean value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Boolean(const XObject&	theXObject,
		    bool			theValue) = 0;

	/**
	 * Call back the XObject with a string value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	String(const XObject&		theXObject,
		   const DOMString&		theValue) = 0;

	/**
	 * Call back the XObject with a result tree fragment value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	ResultTreeFragment(const XObject&				theXObject,
					   const ResultTreeFragBase&	theValue) = 0;

	/**
	 * Call back the XObject with a result tree fragment value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	ResultTreeFragment(const XObject&		theXObject,
					   ResultTreeFragBase&	theValue) = 0;

	/**
	 * Call back the XObject with a node set value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	NodeSet(const XObject&			theXObject,
			const NodeRefListBase&	theValue) = 0;

	/**
	 * Call back the XObject with an unknown value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Unknown(const XObject&		theObject,
			const DOMString&	theName) = 0;

	/**
	 * Call back the XObject with a null value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Null(const XObject&		theObject) = 0;
};



#endif	// XOBJECT_TYPE_CALLBACK_HEADER_GUARD_1357924680
