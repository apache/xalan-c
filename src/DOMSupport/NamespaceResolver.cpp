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



#include <Include/DOMHelper.hpp>
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



DOMString
NamespaceResolver::getNamespaceOfNode(const DOM_Node&	theNode)
{
	using std::make_pair;
	using std::pair;
	using std::vector;

	DOM_Node						theLocalNode(theNode);

	bool							hasProcessedNS;
    
	NSInfo							nsInfo;

    const int						ntype = theLocalNode.getNodeType();

	NSInfoMapType::const_iterator	theIterator = m_NSInfos.end();

	// Find the node in the map of cached DOM_Nodes, if it's not an
	// attribute node.
	if(DOM_Node::ATTRIBUTE_NODE != ntype)
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

	DOMString	namespaceOfPrefix;

	if(hasProcessedNS)
	{
		namespaceOfPrefix = nsInfo.m_namespace;
	}
	else
	{
		DOMString	nodeName = theLocalNode.getNodeName();

		int			indexOfNSSep = indexOf(nodeName, ':');

		DOMString	prefix;

		// If we have an attribute node without a prefix, then 
		// we should use the prefix of the element parent.
		if(DOM_Node::ATTRIBUTE_NODE == ntype)
		{
			if(indexOfNSSep >= 0)
			{
				prefix = substring(nodeName, 0, indexOfNSSep);
			}
			else
			{
				theLocalNode = DOMServices::getParentOfNode(theLocalNode);

				nodeName = theLocalNode.getNodeName();

				indexOfNSSep = indexOf(nodeName, ':');

				prefix = (indexOfNSSep >= 0) ? substring(nodeName, 0, indexOfNSSep) : DOMString();
			}
		}
		else
		{
			prefix = (indexOfNSSep >= 0) ? substring(nodeName, 0, indexOfNSSep) : DOMString();
		}

		bool	ancestorsHaveXMLNS = false;
		bool	nHasXMLNS = false;

		if(equals(prefix, "xml") == true)
		{
			namespaceOfPrefix = DOMServices::s_XMLNamespaceURI;
		}
		else
		{
			int					parentType = 0;
			DOM_Node			parent = theLocalNode;

			typedef pair<DOM_Node, NSInfo>					CandidateNoAncestorEntryType;
			typedef vector<CandidateNoAncestorEntryType>	CandidateNoAncestorVectorType;

			CandidateNoAncestorVectorType	candidateNoAncestorXMLNS;

			while (0 != parent && length(namespaceOfPrefix) == 0)
			{
				if(theIterator != m_NSInfos.end()
				   && nsInfo.m_ancestorHasXMLNSAttrs == nsInfo.ANCESTORNOXMLNS)
				{
					break;
				}

				parentType = parent.getNodeType();

				if(theIterator == m_NSInfos.end() ||
				   nsInfo.m_hasXMLNSAttrs == true)
				{
					bool	elementHasXMLNS = false;
					
					if (parentType == DOM_Node::ELEMENT_NODE) 
					{
						// $$$ TODO: DOM_NamedNodeMap::getLength() should be const.
						DOM_NamedNodeMap	nnm = parent.getAttributes();

						for (int i = 0;  i < nnm.getLength();  i ++) 
						{
							DOM_Node			attr = nnm.item(i);

							const DOMString		aname = attr.getNodeName();

							if(charAt(aname, 0) == 'x')
							{
								const char* const	theXMLNS = "xmlns:";

								bool isPrefix = startsWith(aname, theXMLNS);
							  
								if (equals(aname, "xmlns") == true || isPrefix == true) 
								{
									if(theLocalNode == parent)
									{
										nHasXMLNS = true;
									}

									elementHasXMLNS = true;
									ancestorsHaveXMLNS = true;

									const DOMString	p = isPrefix == true ?
										substring(aname, strlen(theXMLNS)) : DOMString();

									if (equals(p, prefix) == true) 
									{
										namespaceOfPrefix = attr.getNodeValue();
										break;
									}
								}
							}
						}
					}

					if((DOM_Node::ATTRIBUTE_NODE != parentType) &&
						(theIterator == m_NSInfos.end()) &&
						(theLocalNode != parent))
					{
						nsInfo = elementHasXMLNS ? theNSInfoUnProcWithXMLNS :
										theNSInfoUnProcWithoutXMLNS;

						m_NSInfos[parent] = nsInfo;
					}
				}

				if(DOM_Node::ATTRIBUTE_NODE == parentType)
				{
					parent = DOMServices::getParentOfNode(parent);
				}
				else
				{
					candidateNoAncestorXMLNS.push_back(make_pair(parent, nsInfo));

					parent = parent.getParentNode();
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
							m_NSInfos[parent] = candidateInfo;
						}
					}
				}

				candidateNoAncestorXMLNS.clear();
			}
		}

		if(DOM_Node::ATTRIBUTE_NODE != ntype)
		{
			if(0 == length(namespaceOfPrefix))
			{
				if(ancestorsHaveXMLNS == true)
				{
					if(nHasXMLNS == true)
					{
						m_NSInfos[theLocalNode] = theNSInfoNullWithXMLNS;
					}
				
					else
					{
						m_NSInfos[theLocalNode] = theNSInfoNullWithoutXMLNS;
					}
				}
			  
				else
				{
					m_NSInfos[theLocalNode] = theNSInfoNullNoAncestorXMLNS;
				}
			}
			else
			{
				m_NSInfos[theLocalNode] = NSInfo(namespaceOfPrefix, nHasXMLNS);
			}
		}
	}

	return namespaceOfPrefix;
}
