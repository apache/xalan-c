/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_ELEMELEMENT_HEADER_GUARD)
#define XALAN_ELEMELEMENT_HEADER_GUARD 

/**
 * $Id$
 * 
 * 
 * $State$
 * 
 * @author Myriam Midy (Myriam_Midy @lotus.com 
 */

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemUse.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class AVT;



class ElemElement: public ElemUse
{
public:

	/**
	 * Construct an object corresponding to an "xsl:element" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

	virtual
	~ElemElement();

	// These methods are inherited from ElemUse ...

	virtual const XalanDOMString&
	getElementName() const;

	virtual void 
	execute(StylesheetExecutionContext&		executionContext) const;

protected:

	/** 
	 * Process the children of a template.
	 * 
	 * @param executionContext The current execution context
	 * @param skipAttributeChildren If true, attribute children will not be executed.
	 */
	virtual void
	doExecuteChildren(
			StylesheetExecutionContext&		executionContext,
			bool							skipAttributeChildren) const;

	virtual void
	namespacesPostConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler,
			NamespacesHandler&				theHandler);

private:

	/** 
	 * Do any necessary fixup for the default namespace
	 * 
	 * @param executionContext The current execution context
	 */
	void
	fixupDefaultNamespace(StylesheetExecutionContext&	executionContext) const;

	/** 
	 * Get the default namespace from the parent of this element.  Returns
	 * an empty string if there's no default namespace.
	 *
	 * @return The default namespace of the parent, if any.
	 */
	const XalanDOMString&
	getParentDefaultNamespace() const;


	// not implemented
	ElemElement(const ElemElement &);

	ElemElement&
	operator=(const ElemElement &);

	// Data members...
	const AVT*				m_nameAVT;

	const AVT*				m_namespaceAVT;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMELEMENT_HEADER_GUARD
