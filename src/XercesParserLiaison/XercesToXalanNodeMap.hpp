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
#if !defined(XERCESTOXALANNODEMAP_HEADER_GUARD_1357924680)
#define XERCESTOXALANNODEMAP_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <map>



#include <dom/DOM_Node.hpp>



#include <XercesParserLiaison/XercesDOM_NodeHack.hpp>



class NodeImpl;
class XalanNode;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesToXalanNodeMap
{
public:

	XercesToXalanNodeMap();

	~XercesToXalanNodeMap();

	addAssociation(
			const DOM_Node&		theXercesNode,
			XalanNode*			theXalanNode,
			bool				fAssignIndex);

	clear();

	XalanNode*
	getNode(const DOM_Node&		theXercesNode) const
	{
		return getNode(XercesDOM_NodeHack::getImpl(theXercesNode));
	}

	XalanNode*
	getNode(const NodeImpl*		theXercesNodeImpl) const
	{
		const XercesNodeMapType::const_iterator		i =
				m_xercesMap.find(theXercesNodeImpl);

		if (i == m_xercesMap.end())
		{
			return 0;
		}
		else
		{
			return i->second;
		}
	}

	DOM_Node
	getNode(const XalanNode*	theXalanNode) const
	{
		return XercesDOM_NodeHack(getNodeImpl(theXalanNode));
	}

	NodeImpl*
	getNodeImpl(const XalanNode*	theXalanNode) const
	{
		const XalanNodeMapType::const_iterator	i =
				m_xalanMap.find(theXalanNode);

		if (i == m_xalanMap.end())
		{
			return 0;
		}
		else
		{
			return i->second.m_xercesNode;
		}
	}

	NodeImpl*
	getNodeImpl(const DOM_Node&		theXercesNode) const
	{
		return XercesDOM_NodeHack::getImpl(theXercesNode);
	}

	bool
	isNodeAfter(
			const XalanNode*	theFirstXercesNode,
			const XalanNode*	theSecondXercesNode) const;

private:

	struct XalanNodeMapEntryType
	{
		XalanNodeMapEntryType(
			NodeImpl*		theXercesNode = 0,
			unsigned long	theIndex = 0) :
			m_xercesNode(theXercesNode),
			m_index(theIndex)
		{
		}

		NodeImpl*		m_xercesNode;
		unsigned long	m_index;
	};

	typedef std::map<const XalanNode*, XalanNodeMapEntryType>	XalanNodeMapType;

	typedef std::map<const NodeImpl*, XalanNode*>		XercesNodeMapType;

	XalanNodeMapType	m_xalanMap;
	XercesNodeMapType	m_xercesMap;

	unsigned long		m_counter;
};



#endif	// !defined(XERCESTOXALANNODEMAP_HEADER_GUARD_1357924680)
