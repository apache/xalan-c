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
#include "FunctionDocument.hpp"



#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanDocument.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/PrefixResolver.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>



FunctionDocument::FunctionDocument()
{
}



FunctionDocument::~FunctionDocument()
{
}



static XalanDocument*
getDoc(
			XPathExecutionContext&	executionContext,
			const XalanDOMString&	uri,
			const XalanDOMString&	base)
{
	XalanDOMString	localURI(uri);

    XalanDocument*	newDoc = executionContext.getSourceDocument(localURI);

	if(newDoc == 0)
	{
		if(length(localURI) == 0)
		{
			assert(executionContext.getPrefixResolver() != 0);

			localURI = executionContext.getPrefixResolver()->getURI();
		}

		try
		{
			newDoc = executionContext.parseXML(localURI, base);

		}
		catch(...)
		{
			newDoc = 0;
		}

		if(newDoc == 0)
		{
			XalanDOMString	theMessage("Cannot load requested doc: ");

			theMessage += localURI;

			if (length(base) > 0)
			{
				theMessage += " (Base URI: ";
				theMessage += base;
				theMessage += ")";
			}

			executionContext.warn(theMessage);
		}
    }

	return newDoc;
}



XObject*
FunctionDocument::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
{
	if (args.size() == 0)
	{
		executionContext.error("The document() function requires at least one argument!",
							   context);

		return 0;
	}
	else if (context == 0)
	{
		executionContext.error("The document() function requires a non-null context node!",
							   context);

		return 0;
	}
	else
	{
		XalanDocument* const	docContext = XalanNode::DOCUMENT_NODE == context->getNodeType() ?
										static_cast<XalanDocument*>(context) :
											context->getOwnerDocument();

		const XObject* const	arg = args[0];
		assert(arg != 0);

		XalanDOMString				base;

		if(args.size() > 1)
		{
			const XObject* const	arg2 = args[1];
			assert(arg2 != 0);

			if(XObject::eTypeNodeSet == arg2->getType())
			{
				XalanNode* const		baseNode =
							arg2->nodeset().item(0);
				assert(baseNode != 0);

				XalanDocument* const	baseDoc = XalanNode::DOCUMENT_NODE == baseNode->getNodeType() ?
												static_cast<XalanDocument*>(baseNode) :
													baseNode->getOwnerDocument();

				base = executionContext.findURIFromDoc(baseDoc);
			}
			else
			{
				base = arg2->str();
			}
		}
		else
		{
			assert(executionContext.getPrefixResolver() != 0);

			base = executionContext.getPrefixResolver()->getURI();
		}

		MutableNodeRefList		mnl(executionContext.createMutableNodeRefList());

		const int				nRefs = XObject::eTypeNodeSet == arg->getType() ?
													arg->nodeset().getLength()
													: 1;

		for(int i = 0; i < nRefs; i++)
		{
			assert(XObject::eTypeNodeSet != arg->getType() ||
								arg->nodeset().item(i) != 0);

			XalanDOMString	ref = XObject::eTypeNodeSet == arg->getType() ?
													executionContext.getNodeData(*arg->nodeset().item(i)) :
													arg->str();

			// This is the case where the function was called with
			// an empty string, which refers to the stylesheet itself.
			if (nRefs == 1 && isEmpty(ref) == true && args.size() == 1)
			{
				ref = base;
			}

			if(!isEmpty(ref))
			{
				if(docContext == 0)
				{
					executionContext.error("The context node does not have an owner document!");
				}

				// From http://www.ics.uci.edu/pub/ietf/uri/rfc1630.txt
				// A partial form can be distinguished from an absolute form in that the
				// latter must have a colon and that colon must occur before any slash
				// characters. Systems not requiring partial forms should not use any
				// unencoded slashes in their naming schemes.  If they do, absolute URIs
				// will still work, but confusion may result.
				const unsigned int	theLength = length(ref);

				const unsigned int	indexOfColon = indexOf(ref, ':');
				unsigned int		indexOfSlash = indexOf(ref, '/');

#if defined(WIN32)				
				const unsigned int	indexOfBackSlash = indexOf(ref, '\\');

				if(indexOfBackSlash > indexOfSlash && indexOfBackSlash < theLength)
				{
					indexOfSlash = indexOfBackSlash;
				}
#endif				

				if(indexOfColon < theLength && indexOfSlash < theLength && indexOfColon < indexOfSlash)
				{
					// The url (or filename, for that matter) is absolute.
					clear(base);
				}

				XalanDocument* const	newDoc = getDoc(executionContext, ref, base);

				if(newDoc != 0)
				{
					mnl.addNodeInDocOrder(newDoc, true);
				}
			}
		}

		assert(mnl.checkForDuplicates() == false);

		return executionContext.getXObjectFactory().createNodeSet(mnl);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionDocument*
#endif
FunctionDocument::clone() const
{
	return new FunctionDocument(*this);
}
