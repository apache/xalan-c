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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(FUNCTIONID_HEADER_GUARD_1357924680)
#define FUNCTIONID_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <set>



#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanDocument.hpp>



// Base class header files...
#include <XPath/Function.hpp>
#include <XPath/XObjectTypeCallback.hpp>



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
class XALAN_XPATH_EXPORT FunctionID : public Function
{
public:

	FunctionID() :
		Function()
	{
	}

	// These methods are inherited from Function ...

	virtual XObject*
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
	{
		if (args.size() != 1)
		{
			executionContext.error("The id() function takes one argument!",
								   context);
		}
		else if (context == 0)
		{
			executionContext.error("The id() function requires a non-null context node!",
								   context);
		}

		assert(args[0] != 0);

		// Do the callback to get the data.
		FunctionIDXObjectTypeCallback	theCallback(executionContext);

		const XalanDOMString	theResultString =
			theCallback.processCallback(*args[0]);

		// Get the context document, so we can search for nodes.
		const XalanDocument* const	theDocContext = context->getNodeType() == XalanNode::DOCUMENT_NODE ?
										reinterpret_cast<const XalanDocument*>(context) :
										context->getOwnerDocument();
		assert(theDocContext != 0);

		// This list will hold the nodes we find.
#if !defined(XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

		auto_ptr<MutableNodeRefList>	theNodeList(executionContext.createMutableNodeRefList());

		// If there is no context, we cannot continue.
		if(0 == theDocContext)
		{
			executionContext.error("The context node does not have an owner document!",
								   context);
        }
		else if (length(theResultString) > 0)
		{
#if !defined(XALAN_NO_NAMESPACES)
			using std::set;
#endif

			typedef set<XalanDOMString>		TokenSetType;

			// This set will hold tokens that we've previously found, so
			// we can avoid looking more than once.
			TokenSetType		thePreviousTokens;

			StringTokenizer		theTokenizer(theResultString);

			// Parse the result string...
			while(theTokenizer.hasMoreTokens() == true)
			{
				const XalanDOMString	theToken = theTokenizer.nextToken();

				if (length(theToken) > 0)
				{
					// See if we've already seen this one...
					TokenSetType::const_iterator	i =
						thePreviousTokens.find(theToken);

					if (i == thePreviousTokens.end())
					{
						thePreviousTokens.insert(theToken);

						XalanNode* const	theNode =
							executionContext.getElementByID(theToken, *theDocContext);

						if (theNode != 0)
						{
							theNodeList->addNodeInDocOrder(theNode, true);
						}
					}
				}
			}
		}

		return executionContext.getXObjectFactory().createNodeSet(theNodeList.release());
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


private:

	class FunctionIDXObjectTypeCallback : public XObjectTypeCallback
	{
	public:

		FunctionIDXObjectTypeCallback(XPathExecutionContext&	theExecutionContext) :
			XObjectTypeCallback(),
			m_executionContext(theExecutionContext),
			m_resultString()
		{
		}

		const XalanDOMString&
		processCallback(const XObject&	theXObject)
		{
			theXObject.ProcessXObjectTypeCallback(*this);

			return m_resultString;
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
		String(const XObject&			theXObject,
			   const XalanDOMString&	/* theValue */)
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
			const unsigned int	theNodeCount = theValue.getLength();

			for (unsigned int i = 0 ; i < theNodeCount; i++)
			{
				m_resultString += m_executionContext.getNodeData(*theValue.item(i));

				m_resultString += " ";
			}
		}

		virtual void
		Unknown(const XObject&			/* theObject */,
				const XalanDOMString&	/* theName */)
		{
		}

		virtual void
		Null(const XObject&		/* theObject */)
		{
		}

	private:

		XalanDOMString			m_resultString;

		XPathExecutionContext&	m_executionContext;
	};

	// Not implemented...
	FunctionID&
	operator=(const FunctionID&);

	bool
	operator==(const FunctionID&) const;
};



#endif	// FUNCTIONID_HEADER_GUARD_1357924680
