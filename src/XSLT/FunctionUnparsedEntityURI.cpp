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
#include "FunctionUnparsedEntityURI.hpp"


#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>


#include <dom/DOM_Document.hpp>
#include <dom/DOM_Node.hpp>


FunctionUnparsedEntityURI::FunctionUnparsedEntityURI()
{
}



FunctionUnparsedEntityURI::~FunctionUnparsedEntityURI()
{
}




/**
 * Execute an XPath function object.  The function must return 
 * a valid object.
 * @param path The executing xpath.
 * @param context The current context.
 * @param opPos The current op position.
 * @param args A list of XObject arguments.
 * @return A valid XObject.
 */
XObject*
FunctionUnparsedEntityURI::execute(
			XPathExecutionContext&			executionContext,
			const DOM_Node&					context,
			int								/* opPos */,
			const std::vector<XObject*>&	args)
{    
	if(args.size() != 1)
	{
		executionContext.error("The unparsed-entity-uri function should take one argument!");
	}
	DOMString name = (args[0])->str();
	DOM_Document doc =
		(DOM_Node::DOCUMENT_NODE == context.getNodeType()) ?
		static_cast<const DOM_Document&>(context) :
		context.getOwnerDocument();
	DOMString uri = executionContext.getUnparsedEntityURI(name, doc);
	return executionContext.getXObjectFactory().createString(uri);
}

