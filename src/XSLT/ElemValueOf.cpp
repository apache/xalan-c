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
#include "ElemValueOf.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPath.hpp>



#include "Constants.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemValueOf::ElemValueOf(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_VALUE_OF),
	m_selectPattern(0)
{
	bool	isSelectCurrentNode = false;

	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_SELECT))
		{
			const XalanDOMChar* const	avalue = atts.getValue(i);
			assert(avalue != 0);

			if (avalue[0] == XalanUnicode::charFullStop && avalue[1] == 0)
			{
				isSelectCurrentNode = true;
			}
			else
			{
				m_selectPattern =
						constructionContext.createXPath(
							getLocator(),
							avalue,
							*this);
			}
		}
		else if (equals(aname, Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING))
		{
			disableOutputEscaping(
				getStylesheet().getYesOrNo(
					aname,
					atts.getValue(i),
					constructionContext));
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					"xsl:value-of has an illegal attribute",
					0,
					this);
		}
	}

	if(isSelectCurrentNode == false && m_selectPattern == 0)
	{
		constructionContext.error(
			"xsl:value-of requires a 'select' attribute",
			0,
			this);
	}
}



ElemValueOf::~ElemValueOf()
{
}



const XalanDOMString&
ElemValueOf::getElementName() const
{
	return Constants::ELEMNAME_VALUEOF_WITH_PREFIX_STRING;
}



class FormatterListenerAdapater : public FormatterListener
{
public:

	FormatterListenerAdapater(StylesheetExecutionContext&	executionContext) :
		FormatterListener(OUTPUT_METHOD_NONE),
		m_executionContext(executionContext)
	{
	}

	~FormatterListenerAdapater()
	{
	}

	void
	setDocumentLocator(const LocatorType* const	/* locator */)
	{
	}

	void
	startDocument()
	{
	}

	void
	endDocument()
	{
	}

	void
	startElement(
				const	XMLCh* const	/* name */,
				AttributeListType&		/* attrs */)
	{
	}

	void
	endElement(const	XMLCh* const	/* name */)
	{
	}

	void
	characters(
				const XMLCh* const	chars,
				const unsigned int	length)
	{
		m_executionContext.characters(chars, 0, length);
	}

	void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length)
	{
		m_executionContext.charactersRaw(chars, 0, length);
	}

	void
	entityReference(const XMLCh* const	/* name */)
	{
	}

	void
	ignorableWhitespace(
				const XMLCh* const	/* chars */,
				const unsigned int	/* length */)
	{
	}

	void
	processingInstruction(
				const XMLCh* const	target,
				const XMLCh* const	data)
	{
		m_executionContext.processingInstruction(target, data);
	}

	void
	resetDocument()
	{
	}

	void
	comment(const XMLCh* const	data)
	{
		m_executionContext.comment(data);
	}

	void
	cdata(
				const XMLCh* const	/* ch */,
				const unsigned int 	/* length */)
	{
	}

private:

	StylesheetExecutionContext&		m_executionContext;

};



void
ElemValueOf::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	XalanNode* const	sourceNode = executionContext.getCurrentNode();
	assert(sourceNode != 0);

	if (m_selectPattern == 0)
	{
		if (disableOutputEscaping() == false)
		{
			executionContext.characters(*sourceNode);
		}
		else
		{
			executionContext.charactersRaw(*sourceNode);
		}

		if(0 != executionContext.getTraceListeners())
		{
			const StylesheetExecutionContext::GetAndReleaseCachedString		theString(executionContext);

			DOMServices::getNodeData(*sourceNode, theString.get());

			fireSelectionEvent(executionContext, sourceNode, theString.get());
		}
	}
	else
	{
		FormatterListenerAdapater	theAdapter(executionContext);

		XPath::MemberFunctionPtr	theFunction = disableOutputEscaping() == false ?
			&FormatterListener::characters : &FormatterListener::charactersRaw;

		m_selectPattern->execute(*this, executionContext, theAdapter, theFunction);

		if(0 != executionContext.getTraceListeners())
		{
			const XObjectPtr	value(m_selectPattern->execute(sourceNode, *this, executionContext));

			if (value.null() == false)
			{
				fireSelectionEvent(executionContext, sourceNode, value);
			}
		}
	}
}



void
ElemValueOf::fireSelectionEvent(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceNode,
			const XalanDOMString&			theValue) const
{
	const XObjectPtr value(executionContext.getXObjectFactory().createStringReference(theValue));

	fireSelectionEvent(executionContext, sourceNode, value);
}



void
ElemValueOf::fireSelectionEvent(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceNode,
			const XObjectPtr				theValue) const
{
	if (m_selectPattern != 0)
	{
		fireSelectionEvent(
			executionContext,
			sourceNode,
			theValue,
			m_selectPattern->getExpression().getCurrentPattern());
	}
	else
	{
		const StylesheetExecutionContext::GetAndReleaseCachedString		thePattern(executionContext);

		thePattern.get() = XALAN_STATIC_UCODE_STRING(".");

		fireSelectionEvent(
			executionContext,
			sourceNode,
			theValue,
			thePattern.get());
	}

}



void
ElemValueOf::fireSelectionEvent(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceNode,
			const XObjectPtr				theValue,
			const XalanDOMString&			thePattern) const
{
	executionContext.fireSelectEvent(
		SelectionEvent(
			executionContext,
			sourceNode,
			*this,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
			thePattern,
			theValue));
}



XALAN_CPP_NAMESPACE_END
