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
#include "NamespaceResolver.hpp"



#include <vector>



#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>



#include "DOMServices.hpp"



NamespaceResolver::NamespaceResolver() :
	Resettable(),
	m_NSInfos()
{
}



NamespaceResolver::~NamespaceResolver()
{
}


void
NamespaceResolver::reset()
{
	m_NSInfos.clear();
}



static NSInfo	theNSInfoUnProcWithXMLNS(false, true);
static NSInfo	theNSInfoUnProcWithoutXMLNS(false, false);
static NSInfo	theNSInfoUnProcNoAncestorXMLNS(false, false, NSInfo::ANCESTORNOXMLNS);
static NSInfo	theNSInfoNullWithXMLNS(true, true);
static NSInfo	theNSInfoNullWithoutXMLNS(true, false);
static NSInfo	theNSInfoNullNoAncestorXMLNS(true, false, NSInfo::ANCESTORNOXMLNS);



void
NamespaceResolver::updateNamespace(
			const XalanNode*	theNode,
			const NSInfo&		theNamespace) const
{
#if defined(XALAN_NO_MUTABLE)
#if defined(XALAN_OLD_STYLE_CASTS)
	((NamespaceResolver*)this)->m_NSInfos[theNode] = theNamespace;
#else
	const_cast<NamespaceResolver*>(this)->m_NSInfos[theNode] = theNamespace;
#endif
#else
	m_NSInfos[theNode] = theNamespace;
#endif
}



XalanDOMString
NamespaceResolver::getNamespaceOfNode(const XalanNode&	theNode) const
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::make_pair;
	using std::pair;
	using std::vector;
#endif

	const XalanNode*				theLocalNode = &theNode;

	bool							hasProcessedNS;
    
	NSInfo							nsInfo;

    const XalanNode::NodeType		ntype = theLocalNode->getNodeType();

	NSInfoMapType::const_iterator	theIterator = m_NSInfos.end();

	// Find the node in the map of cached nodes, if it's not an
	// attribute node.
	if(XalanNode::ATTRIBUTE_NODE != ntype)
	{
		theIterator = m_NSInfos.find(theLocalNode);

		if (theIterator == m_NSInfos.end())
		{
			// Not found.
			hasProcessedNS = false;
		}
		else
		{
			// Ahh, we found it.
			nsInfo = (*theIterator).second;
			hasProcessedNS = nsInfo.m_hasProcessedNS;
		}
    }
    else
    {
		hasProcessedNS = false;
    }

	XalanDOMString	namespaceOfPrefix;

	if(hasProcessedNS)
	{
		namespaceOfPrefix = nsInfo.m_namespace;
	}
	else
	{
		XalanDOMString	nodeName = theLocalNode->getNodeName();

		unsigned int	indexOfNSSep = indexOf(nodeName, ':');

		XalanDOMString	prefix;

		// If we have an attribute node without a prefix, then 
		// we should use the prefix of the element parent.
		if(XalanNode::ATTRIBUTE_NODE == ntype)
		{
			if(indexOfNSSep < length(nodeName))
			{
				prefix = substring(nodeName, 0, indexOfNSSep);
			}
			else
			{
				theLocalNode = DOMServices::getParentOfNode(*theLocalNode);

				nodeName = theLocalNode->getNodeName();

				indexOfNSSep = indexOf(nodeName, ':');

				prefix = indexOfNSSep < length(nodeName) ? substring(nodeName, 0, indexOfNSSep) : XalanDOMString();
			}
		}
		else
		{
			prefix = indexOfNSSep < length(nodeName) ? substring(nodeName, 0, indexOfNSSep) : XalanDOMString();
		}

		bool	ancestorsHaveXMLNS = false;
		bool	nHasXMLNS = false;

		if(equals(prefix, DOMServices::s_XMLString) == true)
		{
			namespaceOfPrefix = DOMServices::s_XMLNamespaceURI;
		}
		else
		{
			XalanNode::NodeType		parentType = XalanNode::UNKNOWN_NODE;
			const XalanNode*		parent = theLocalNode;

			typedef pair<const XalanNode*, NSInfo>			CandidateNoAncestorEntryType;
			typedef vector<CandidateNoAncestorEntryType>	CandidateNoAncestorVectorType;

			CandidateNoAncestorVectorType	candidateNoAncestorXMLNS;

			while (0 != parent && length(namespaceOfPrefix) == 0)
			{
				if(theIterator != m_NSInfos.end()
				   && nsInfo.m_ancestorHasXMLNSAttrs == nsInfo.ANCESTORNOXMLNS)
				{
					break;
				}

				parentType = parent->getNodeType();

				if(theIterator == m_NSInfos.end() ||
				   nsInfo.m_hasXMLNSAttrs == true)
				{
					bool	elementHasXMLNS = false;
					
					if (parentType == XalanNode::ELEMENT_NODE) 
					{
						const XalanNamedNodeMap* const	nnm =
							parent->getAttributes();
						assert(nnm != 0);

						const unsigned int	theLength = nnm->getLength();

						for (unsigned int i = 0;  i < theLength;  i ++) 
						{
							const XalanNode*		attr = nnm->item(i);

							const XalanDOMString	aname = attr->getNodeName();

							if(charAt(aname, 0) == 'x')
							{
								bool isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);
							  
								if (equals(aname, DOMServices::s_XMLNamespace) == true || isPrefix == true) 
								{
									if(theLocalNode == parent)
									{
										nHasXMLNS = true;
									}

									elementHasXMLNS = true;
									ancestorsHaveXMLNS = true;

									const XalanDOMString	p = isPrefix == true ?
										substring(aname, length(DOMServices::s_XMLNamespaceWithSeparator)) : XalanDOMString();

									if (equals(p, prefix) == true) 
									{
										namespaceOfPrefix = attr->getNodeValue();
										break;
									}
								}
							}
						}
					}

					if((XalanNode::ATTRIBUTE_NODE != parentType) &&
						(theIterator == m_NSInfos.end()) &&
						(theLocalNode != parent))
					{
						nsInfo = elementHasXMLNS ? theNSInfoUnProcWithXMLNS :
										theNSInfoUnProcWithoutXMLNS;

						updateNamespace(parent, nsInfo);
					}
				}

				if(XalanNode::ATTRIBUTE_NODE == parentType)
				{
					parent = DOMServices::getParentOfNode(*parent);
				}
				else
				{
					candidateNoAncestorXMLNS.push_back(make_pair(parent, nsInfo));

					parent = parent->getParentNode();
				}

				if(0 != parent)
				{
					theIterator = m_NSInfos.find(parent);

					if (theIterator != m_NSInfos.end())
					{
						nsInfo = (*theIterator).second;
					}
				}
			}

			const int	nCandidates = candidateNoAncestorXMLNS.size();

			if(nCandidates > 0)
			{
				if(false == ancestorsHaveXMLNS && 0 == parent)
				{
					for(int i = 0; i < nCandidates; i++)
					{
						const NSInfo&	candidateInfo = candidateNoAncestorXMLNS[i].second;

						if(candidateInfo == theNSInfoUnProcWithoutXMLNS ||
						   candidateInfo == theNSInfoNullWithoutXMLNS)
						{
							updateNamespace(parent, candidateInfo);
						}
					}
				}

				candidateNoAncestorXMLNS.clear();
			}
		}

		if(XalanNode::ATTRIBUTE_NODE != ntype)
		{
			if(0 == length(namespaceOfPrefix))
			{
				if(ancestorsHaveXMLNS == true)
				{
					if(nHasXMLNS == true)
					{
						updateNamespace(theLocalNode, theNSInfoNullWithXMLNS);
					}
				
					else
					{
						updateNamespace(theLocalNode, theNSInfoNullWithoutXMLNS);
					}
				}
			  
				else
				{
					updateNamespace(theLocalNode, theNSInfoNullNoAncestorXMLNS);
				}
			}
			else
			{
				updateNamespace(theLocalNode, NSInfo(namespaceOfPrefix, nHasXMLNS));
			}
		}
	}

	return namespaceOfPrefix;
}
