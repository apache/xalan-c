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



#include <util/XMLURL.hpp>



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



static DOM_Document
getDoc(
			XPathExecutionContext&	executionContext,
			const DOMString&		uri,
			const DOMString&		base)
{
	DOMString		localURI(uri);

	DOM_Document	newDoc = executionContext.getSourceDocument(localURI);

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
			DOMString	theMessage("Cannot load requested doc: ");

			theMessage += base;
			theMessage += localURI;

			executionContext.warn(theMessage);
		}
		else
		{
			executionContext.setSourceDocument(localURI, newDoc);
		}
    }

	return newDoc;
}



XObject*
FunctionDocument::execute(
			XPathExecutionContext&			executionContext,
			const DOM_Node&					context,
			int								/* opPos */,
			const std::vector<XObject*>&	args)
{
	if (args.size() == 0)
	{
		executionContext.error("The document() function requires at least one argument!",
							   context);

		return 0;
	}
	else
	{
		DOM_Document	docContext = DOM_Node::DOCUMENT_NODE == context.getNodeType() ?
										static_cast<const DOM_Document&>(context) :
											context.getOwnerDocument();

		const XObject* const	arg = args[0];
		assert(arg != 0);

		DOMString				base;

		if(args.size() > 1)
		{
			const XObject* const	arg2 = args[1];
			assert(arg2 != 0);

			if(XObject::eTypeNodeSet == arg2->getType())
			{
				const DOM_Node		baseNode(arg2->nodeset().item(0));
				assert(baseNode != 0);

				const DOM_Document	baseDoc = DOM_Node::DOCUMENT_NODE == baseNode.getNodeType() ?
												static_cast<const DOM_Document&>(baseNode) :
													baseNode.getOwnerDocument();

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

		// Chop off the file name part of the URI, this includes the
		// trailing separator
		DOMString newBase;
		{
			int indexOfSlash = lastIndexOf(base, '/');
#if defined(WIN32)				
			const int indexOfBackSlash = lastIndexOf(base, '\\');
			if(indexOfBackSlash > indexOfSlash)
				indexOfSlash = indexOfBackSlash;
#endif				
				newBase = substring(base, 0, indexOfSlash+1);
		}

		MutableNodeRefList		mnl(executionContext.createMutableNodeRefList());

		const int				nRefs = XObject::eTypeNodeSet == arg->getType() ?
													arg->nodeset().getLength()
													: 1;

		for(int i = 0; i < nRefs; i++)
		{
			const DOMString		ref = XObject::eTypeNodeSet == arg->getType() ?
													executionContext.getNodeData(arg->nodeset().item(i)) :
													arg->str();

			if(length(ref) > 0)
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
				const int indexOfColon = indexOf(ref, ':');
				int indexOfSlash = indexOf(ref, '/');
#if defined(WIN32)				
				const int indexOfBackSlash = indexOf(ref, '\\');
				if(indexOfBackSlash > indexOfSlash)
					indexOfSlash = indexOfBackSlash;
#endif				

				if(indexOfColon != -1 && indexOfSlash != -1 && indexOfColon < indexOfSlash)
				{
					// The url (or filename, for that matter) is absolute.
					newBase = DOMString();
				}

				const DOM_Document	newDoc = getDoc(executionContext, ref, newBase);

				if(newDoc != 0)
				{
					mnl.addNodeInDocOrder(newDoc, true);
				}
			}
		}

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
