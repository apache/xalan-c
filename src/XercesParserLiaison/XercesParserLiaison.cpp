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
#include "XercesParserLiaison.hpp"



#include <memory>



#include <parsers/DOMParser.hpp>
#include <parsers/SAXParser.hpp>
#include <internal/URLInputSource.hpp>



const char* const	theParserName = "Xerces";

XercesParserLiaison::XercesParserLiaison(
			DOMSupport&		theSupport,
			bool			fUseValidatingParser) :
	XMLParserLiaisonDefault(theSupport,
							theParserName),
	m_fUseValidatingParser(fUseValidatingParser)
{
}



XercesParserLiaison::~XercesParserLiaison()
{
}



bool
XercesParserLiaison::supportsSAX() const
{
	return true;
}



namespace
{

inline DOMParser*
CreateDOMParser(bool	fValidating)
{
	return new DOMParser;

/*
	if (fValidating == true)
	{
		return new ValidatingDOMParser;
	}
	else
	{
		return new NonValidatingDOMParser;
	}
*/
}



inline SAXParser*
CreateSAXParser(bool	fValidating)
{
	return new SAXParser;

/*
	if (fValidating == true)
	{
		return new ValidatingSAXParser;
	}
	else
	{
		return new NonValidatingSAXParser;
	}
*/
}
};


DOM_Document
XercesParserLiaison::parseXMLStream(
			InputSource&		reader,
			const DOMString&	/* identifier */)
{
	std::auto_ptr<DOMParser>	theParser(CreateDOMParser(m_fUseValidatingParser));

	theParser->parse(reader);

	return theParser->getDocument();
}



void
XercesParserLiaison::parseXMLStream(
			InputSource&		urlInputSource,
			DocumentHandler&	handler,
			const DOMString&	/* identifier */)
{
	std::auto_ptr<SAXParser>	theParser(CreateSAXParser(m_fUseValidatingParser));

	theParser->setDocumentHandler(&handler);

	theParser->parse(urlInputSource);
}



DOM_Document
XercesParserLiaison::parseXMLStream(
			URLInputSource&		urlInputSource,
			const DOMString&	/* identifier */)
{
	std::auto_ptr<DOMParser>	theParser(CreateDOMParser(m_fUseValidatingParser));

	theParser->parse(urlInputSource);

	return theParser->getDocument();
}



void
XercesParserLiaison::parseXMLStream(
			URLInputSource&		urlInputSource,
			DocumentHandler&	handler,
			const DOMString&	/* identifier */)
{
	std::auto_ptr<SAXParser>	theParser(CreateSAXParser(m_fUseValidatingParser));

	theParser->setDocumentHandler(&handler);

	theParser->parse(urlInputSource);
}



DOM_Document
XercesParserLiaison::createDocument()
{
	return DOM_Document::createDocument();
}
