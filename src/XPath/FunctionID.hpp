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
#if !defined(FUNCTIONID_HEADER_GUARD_1357924680)
#define FUNCTIONID_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <set>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>



// Base class header files...
#include <XPath/Function.hpp>
#include <XPath/XObjectTypeCallback.hpp>



#include <Include/DOMHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>



/**
 * XPath implementation of "id" function.
 */
//
// These are all inline, even though
// there are virtual functions, because we expect that they will only be
// needed by the XPath class.
class XALAN_XPATH_EXPORT FunctionID : public Function, public XObjectTypeCallback
{
public:

	FunctionID() :
		Function(),
		XObjectTypeCallback(),
		m_executionContext(0)
	{
	}

	// These methods are inherited from Function ...

	virtual XObject*
	execute(
			XPathExecutionContext&			executionContext,
			const DOM_Node&					context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
	{
		if (args.size() != 1)
		{
			executionContext.error("The id() function takes one argument!",
								   context);
		}

		// OK, reset the data members...
		m_executionContext = &executionContext;

		m_resultString = 0;

		// Do the callback to get the data.
		args[0]->ProcessXObjectTypeCallback(*this);

		// Get the context document, so we can search for nodes.
		DOM_Document		theDocContext = context.getNodeType() == DOM_Node::DOCUMENT_NODE ?
									reinterpret_cast<const DOM_Document&>(context) :
									context.getOwnerDocument();

		// This list will hold the nodes we find.
		MutableNodeRefList	theNodeList(executionContext.createMutableNodeRefList());

		// If there is no context, we cannot continue.
		if(0 == theDocContext)
		{
			executionContext.error("The context node does not have an owner document!",
								   context);
        }
		else if (length(m_resultString) > 0)
		{
#if !defined(XALAN_NO_NAMESPACES)
			using std::set;
#endif

			typedef set<DOMString>	TokenSetType;

			// This set will hold tokens that we've previously found, so
			// we can avoid looking more than once.
			TokenSetType		thePreviousTokens;

			StringTokenizer		theTokenizer(m_resultString);

			// Parse the result string...
			while(theTokenizer.hasMoreTokens() == true)
			{
				const DOMString		theToken = theTokenizer.nextToken();

				if (length(theToken) > 0)
				{
					// See if we've already seen this one...
					TokenSetType::const_iterator	i =
						thePreviousTokens.find(theToken);

					if (i != thePreviousTokens.end())
					{
						thePreviousTokens.insert(theToken);

						const DOM_Node	theNode =
							executionContext.getElementByID(theToken, theDocContext);

						if (theNode != 0)
						{
							theNodeList.addNodeInDocOrder(theNode);
						}
					}
				}
			}
		}

		return executionContext.getXObjectFactory().createNodeSet(theNodeList);
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionID*
#endif
	clone() const
	{
		return new FunctionID(*this);
	}


	// These methods are inherited from XObjectTypeCallback ...

	virtual void
	Number(const XObject&	theXObject,
		   double			/* theValue */)
	{
		m_resultString = theXObject.str();
	}

	virtual void
	Boolean(const XObject&	theXObject,
		    bool			/* theValue */)
	{
		m_resultString = theXObject.str();
	}

	virtual void
	String(const XObject&		theXObject,
		   const DOMString&		/* theValue */)
	{
		m_resultString = theXObject.str();
	}

	virtual void
	ResultTreeFragment(const XObject&				theXObject,
					   const ResultTreeFragBase&	/* theValue */)
	{
		m_resultString = theXObject.str();
	}

	virtual void
	ResultTreeFragment(const XObject&		theXObject,
					   ResultTreeFragBase&	/* theValue */)
	{
		m_resultString = theXObject.str();
	}

	virtual void
	NodeSet(const XObject&			/* theXObject */,
			const NodeRefListBase&	theValue)
	{
		assert(m_executionContext != 0);

		const unsigned int	theNodeCount = theValue.getLength();

		for (unsigned int i = 0 ; i < theNodeCount; i++)
		{
			m_resultString += m_executionContext->getNodeData(theValue.item(i));

			m_resultString += " ";
		}
	}

	virtual void
	Unknown(const XObject&		/* theObject */,
			const DOMString&	/* theName */)
	{
	}

	virtual void
	Null(const XObject&		/* theObject */)
	{
	}

private:

	// Not implemented...
	FunctionID&
	operator=(const FunctionID&);

	bool
	operator==(const FunctionID&) const;


	// Data members...
	XPathExecutionContext*		m_executionContext;

	DOMString					m_resultString;
};



#endif	// FUNCTIONID_HEADER_GUARD_1357924680
