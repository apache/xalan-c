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
// Class header file.
#include "XMLParserLiaisonDefault.hpp"



// Standard header files	
#include <cassert>
#include <cstring>
#include <memory>



// Xerces header files
#include <dom/DOM_Attr.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NamedNodeMap.hpp>



// XSL4C header files
#include <Include/DOMHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrintWriter.hpp>
#include <DOMSupport/DOMServices.hpp>



#include "FormatterToHTML.hpp"
#include "FormatterToXML.hpp"
#include "FormatterTreeWalker.hpp"
#include "XMLSupportException.hpp"


namespace
{

static const char* const	theDefaultSpecialCharacters = "<>&\"\'\r\n";

}



XMLParserLiaisonDefault::XMLParserLiaisonDefault(
			DOMSupport&			theDOMSupport,
			const DOMString&	theParserDescription) :
	XMLParserLiaison(),
	Formatter(),
	m_DOMSupport(theDOMSupport),
	m_SpecialCharacters(theDefaultSpecialCharacters),
	m_Indent(-1),
	m_fShouldExpandEntityRefs(false),
	m_fUseValidation(false),
	m_ParserDescription(theParserDescription),
	m_FormatterListener(0),
	m_fOwnListener(false)
{
}



XMLParserLiaisonDefault::~XMLParserLiaisonDefault()
{
	if (m_fOwnListener == true)
	{
		delete m_FormatterListener;
	}
}



void
XMLParserLiaisonDefault::reset()
{
	if (m_fOwnListener == true)
	{
		delete m_FormatterListener;

		m_fOwnListener = false;
	}

	m_FormatterListener = 0;

	m_DOMSupport.reset();
}



bool
XMLParserLiaisonDefault::supportsSAX() const
{
	return false;
}



DOM_Document
XMLParserLiaisonDefault::parseXMLStream(
			InputSource&		/* reader */,
			const DOMString&	/* identifier */)
{
	throw XMLSupportException("parseXMLStream() not supported in XMLParserLiaisonDefault!");

	return DOM_Document();
}



DOM_Document
XMLParserLiaisonDefault::parseXMLStream(
			URLInputSource&		/* reader */,
			const DOMString&	/* identifier */)
{
	throw XMLSupportException("parseXMLStream() not supported in XMLParserLiaisonDefault!");

	return DOM_Document();
}



void
XMLParserLiaisonDefault::parseXMLStream(
			InputSource&		/* urlInputSource */,
			DocumentHandler&	/* handler */,
			const DOMString&	/* identifier */)
{
	throw XMLSupportException("parseXMLStream() not supported in XMLParserLiaisonDefault!");
}



void
XMLParserLiaisonDefault::parseXMLStream(
			URLInputSource&		/* urlInputSource */,
			DocumentHandler&	/* handler */,
			const DOMString&	/* identifier */)
{
	throw XMLSupportException("parseXMLStream() not supported in XMLParserLiaisonDefault!");
}



DOM_Document
XMLParserLiaisonDefault::createDocument()
{
	throw XMLSupportException("createDocument() not supported in XMLParserLiaisonDefault!");

	return DOM_Document();
}

  

DOM_Document
XMLParserLiaisonDefault::getDOMFactory()
{
	return createDocument();
}



/**
 * Returns the element name with the namespace expanded.
 */
DOMString
XMLParserLiaisonDefault::getExpandedElementName(const DOM_Element&		elem) const
{
	return m_DOMSupport.getExpandedElementName(elem);
}



/**
 * Returns the attribute name with the namespace expanded.
 */
DOMString
XMLParserLiaisonDefault::getExpandedAttributeName(const DOM_Attr&	attr) const
{
	return m_DOMSupport.getExpandedAttributeName(attr);
}



void
XMLParserLiaisonDefault::toMarkup(
			const DOM_Document&		doc,
			PrintWriter&			pw,
			const DOMString&		resultns,
			bool					format)
{
	using std::auto_ptr;

	auto_ptr<FormatterListener> 	visitor;

	if(equals(trim(resultns), "http://www.w3.org/TR/REC-html40") == true)
	{
		FormatterToHTML* const	htmlFormatVisitor =
				new FormatterToHTML(pw,
						DOMString(),
						format,
						m_Indent,
						DOMString(),	// encoding
						DOMString(),	// media type
						// @@ JMD: ??
						// was: "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n",
						DOMString(),	// doctypeSystem
						DOMString(),	// doctypePublic
						false, // xmlDecl
						DOMString(),	// standalone,
						0);
				

#if defined(XALAN_OLD_AUTO_PTR)
		visitor = auto_ptr<FormatterListener>(htmlFormatVisitor);
#else
		visitor.reset(htmlFormatVisitor);
#endif

		htmlFormatVisitor->m_attrSpecialChars = m_SpecialCharacters;
	}
	else
	{
		FormatterToXML* const 	fToXML =
				new FormatterToXML(pw,
						DOMString(),
						format,
						m_Indent,
						DOMString(),	// encoding
						DOMString(),	// media type
						DOMString(),	// doctypeSystem
						DOMString(),	// doctypePublic
						true,				// xmlDecl
						DOMString(),	// standalone,
						0);

#if defined(XALAN_OLD_AUTO_PTR)
		visitor = auto_ptr<FormatterListener>(fToXML);
#else
		visitor.reset(fToXML);
#endif

		fToXML->m_attrSpecialChars = m_SpecialCharacters;
	}

	if(visitor.get() != 0)
	{
		FormatterTreeWalker		treeTraversal(*visitor);

		treeTraversal.traverse(doc);
	}
} 



void
XMLParserLiaisonDefault::setFormatterListener(
			PrintWriter&		pw,
			const DOMString&	resultns,
			bool				format)
{
	if(equals(trim(resultns), "http://www.w3.org/TR/REC-html40") == true)
	{
		FormatterToHTML* const	htmlFormatVisitor =
			new FormatterToHTML(pw,
						DOMString(),
						format,
						m_Indent,
						DOMString(),	// encoding
						DOMString(),	// media type
						// @@ JMD: ??
						// was: "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n",
						DOMString(),	// doctypeSystem
						DOMString(),	// doctypePublic
						false, // xmlDecl
						DOMString(),	// standalone,
						0);

		if (m_fOwnListener == true)
		{
			delete m_FormatterListener;
		}

		m_FormatterListener =  htmlFormatVisitor;
		m_fOwnListener = true;

		htmlFormatVisitor->m_attrSpecialChars = m_SpecialCharacters;
	}
	else if(0 == m_FormatterListener)
	{
		FormatterToXML* const	fToXML =
			new FormatterToXML(pw,
					DOMString(),
					format,
					m_Indent,
					DOMString(),	// encoding
					DOMString(),	// media type
					DOMString(),	// doctypeSystem
					DOMString(),	// doctypePublic
					true,				// xmlDecl
					DOMString(),	// standalone,
						0);

		fToXML->m_attrSpecialChars = m_SpecialCharacters;

		m_FormatterListener = fToXML;
		m_fOwnListener = true;
	}
}



void
XMLParserLiaisonDefault::setFormatterListener(FormatterListener*	fl)
{
	if (m_fOwnListener == true)
	{
		delete m_FormatterListener;
	}

	m_fOwnListener = false;
	m_FormatterListener = fl;
}



FormatterListener*
XMLParserLiaisonDefault::getFormatterListener() const
{
	return m_FormatterListener;
}
