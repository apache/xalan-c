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
#if !defined(XLOCATOR_HEADER_GUARD_1357924680)
#define XLOCATOR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



class DOM_Node;
class XObject;
class XPath;
class XPathExecutionContext;
class XNodeSet;



class XALAN_XPATH_EXPORT XLocator
{
public:

	XLocator();

	virtual
	~XLocator();

#if defined(XALAN_NO_NAMESPACES)
	typedef	vector<XObject*>		ConnectArgsVectorType;
#else
	typedef	std::vector<XObject*>	ConnectArgsVectorType;
#endif

	virtual XObject*
	connectToNodes(
			const XPath&					xpath,
			XPathExecutionContext&			executionContext,
			const DOM_Node&					context, 
            int								opPos,
			const ConnectArgsVectorType&	connectArgs) = 0;
  
	/**
	 * Execute a location path.  Normally, this method simply 
	 * moves past the OP_LOCATIONPATH and it's length member, 
	 * and calls the Step function, which will recursivly process 
	 * the rest of the LocationPath, and then wraps the NodeList result
	 * in an XNodeSet object.
	 * @param xpath The xpath that is executing.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the xpath.m_opMap array.
	 * @returns the result of the query in an XNodeSet object.
	 */
	virtual XObject*
	locationPath(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node&			context, 
            int						opPos) = 0;

	/**
	 * Execute a a location path pattern.  This will return a score
	 * of MATCH_SCORE_NONE, MATCH_SCORE_NODETEST, 
	 * MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
	 * @param xpath The xpath that is executing.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the xpath.m_opMap array.
	 * @returns score, one of MATCH_SCORE_NODETEST, 
	 * MATCH_SCORE_NONE, MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
	 */
	virtual double
	locationPathPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node&			context, 
            int						opPos) = 0;
};



#endif	// XLOCATOR_HEADER_GUARD_1357924680
