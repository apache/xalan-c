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
#include "XalanSourceTreeDOMSupport.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeParserLiaison.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeDOMSupport::XalanSourceTreeDOMSupport() :
	DOMSupport(),
	m_parserLiaison(0)
{
}



XalanSourceTreeDOMSupport::XalanSourceTreeDOMSupport(const XalanSourceTreeParserLiaison&	theParserLiaison) :
	DOMSupport(),
	m_parserLiaison(&theParserLiaison)
{
}



XalanSourceTreeDOMSupport::~XalanSourceTreeDOMSupport()
{
}



void
XalanSourceTreeDOMSupport::reset()
{
}



const XalanDOMString&
XalanSourceTreeDOMSupport::getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const
{
	if (m_parserLiaison != 0)
	{
		const XalanSourceTreeDocument* const	theXSTDocument =
			m_parserLiaison->mapDocument(&theDocument);

		if (theXSTDocument != 0)
		{
			return theXSTDocument->getUnparsedEntityURI(theName);
		}
	}

	return s_emptyString;
}



bool
XalanSourceTreeDOMSupport::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	assert(node1.isIndexed() == true && node2.isIndexed() == true);

	return node1.getIndex() > node2.getIndex() ? true : false;
}



XALAN_CPP_NAMESPACE_END
