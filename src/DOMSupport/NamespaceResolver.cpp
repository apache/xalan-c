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



#include <PlatformSupport/XalanUnicode.hpp>



#include "DOMServices.hpp"



NamespaceResolver::NamespaceResolver() :
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


// Manefest constants: Prebuilt NSInfo objects for standard combinations
static NSInfo	theNSInfoUnProcWithXMLNS(false, true);
static NSInfo	theNSInfoUnProcWithoutXMLNS(false, false);
static NSInfo	theNSInfoUnProcNoAncestorXMLNS(false, false, NSInfo::ANCESTORNOXMLNS);
static NSInfo	theNSInfoNullWithXMLNS(true, true);
static NSInfo	theNSInfoNullWithoutXMLNS(true, false);
static NSInfo	theNSInfoNullNoAncestorXMLNS(true, false, NSInfo::ANCESTORNOXMLNS);


// Set (or reset) the cached NSInfo associated with a Node
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


// Find the name of the namespace bound to the current node. This is done by searching
// upward through the model for a Namespace Declaration attribute. Once resolved, the
// namespace is cached in the m_NSInfos table for faster retrieval. 
//
// Note that DOM Level 2 binds this value at the time the node is built, which should
// be considerably more efficient.
const XalanDOMString&
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

	const XalanDOMString*	namespaceOfPrefix = &DOMServices::s_emptyString;

	if(hasProcessedNS)
	{
		namespaceOfPrefix = &nsInfo.m_namespace;
	}
	else
	{
		const XalanDOMString&	nodeName = theLocalNode->getNodeName();

		unsigned int			indexOfNSSep = indexOf(nodeName, XalanUnicode::charColon);

		XalanDOMString			prefix;

		// JKESS CHANGE: Attributes which have no prefix have no namespace,
		// per standard Namespaces In XML behavior. They should not inherit from
		// their owning Element, nor use the default namespace.
		if(XalanNode::ATTRIBUTE_NODE == ntype && indexOfNSSep >= length(nodeName))
		{
			// Attibute nodes aren't handled by the nsInfos logic above.
			// And since this is the no-prefix case, it won't buy us anything for
			// explicit prefix lookups. Hence, I don't see any reason to register
			// this result via updateNamespace.

			// BIG UGLY RETURN HERE!!!!!!!
			return *namespaceOfPrefix;
		}

		prefix = (indexOfNSSep < length(nodeName))
				? substring(nodeName, 0, indexOfNSSep) 
				: XalanDOMString();

		bool	ancestorsHaveXMLNS = false;
		bool	nHasXMLNS = false;

		// The xml: prefix is hardcoded
		// (In the DOM, so is the xmlns: prefix... but that's DOM behavior,
		// not specified by the NS spec.)
		if(equals(prefix, DOMServices::s_XMLString) == true)
		{
			namespaceOfPrefix = &DOMServices::s_XMLNamespaceURI;
		}
		else
		{
			XalanNode::NodeType		parentType = XalanNode::UNKNOWN_NODE;
			const XalanNode*		parent = theLocalNode;

			typedef pair<const XalanNode*, NSInfo>			CandidateNoAncestorEntryType;
			typedef vector<CandidateNoAncestorEntryType>	CandidateNoAncestorVectorType;

			CandidateNoAncestorVectorType	candidateNoAncestorXMLNS;

			candidateNoAncestorXMLNS.reserve(eDefaultVectorSize);

			// Hunt upward until resolve namespace or fail to do so.
			while (0 != parent && length(*namespaceOfPrefix) == 0)
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
					
					// Elements
					if (parentType == XalanNode::ELEMENT_NODE) 
					{
						// Scan the Element's Attr's, looking for namespace declarations
						const XalanNamedNodeMap* const	nnm =
							parent->getAttributes();
						assert(nnm != 0);

						const unsigned int	theLength = nnm->getLength();

						for (unsigned int i = 0;  i < theLength;  i ++) 
						{
							const XalanNode*		attr = nnm->item(i);

							const XalanDOMString&	aname = attr->getNodeName();

							// Quick test of first character, to reduce cost of startsWith.
							if(charAt(aname, 0) == charAt(DOMServices::s_XMLNamespaceWithSeparator, 0))
							{
								// "xmlns:"* prefix declaration?
								bool isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);
							  
								// or "xmlns" default declaration?
								// JKESS: Reversed order of test; more efficient.
								if (isPrefix == true || equals(aname, DOMServices::s_XMLNamespace) == true) 
								{
									// Is this element the original node?
									if(theLocalNode == parent)
									{
										nHasXMLNS = true; // local decls exist
									}

									elementHasXMLNS = true; // decls exist on current parent
									ancestorsHaveXMLNS = true; // decls exist somewhere

									// If xmlns:, what prefix is declared?
									const XalanDOMString	p = (isPrefix == true)
											? substring(aname, length(DOMServices::s_XMLNamespaceWithSeparator)) 
											: XalanDOMString();

									// If it's the one we're looking for, resolve to NS
									if (equals(p, prefix) == true) 
									{
										namespaceOfPrefix = &attr->getNodeValue();
										break;
									}
								}
							}
						}
					}

					// Fallthough for unresolved Elements, 
					// plus anything else except Attr's
					if((XalanNode::ATTRIBUTE_NODE != parentType) &&
						(theIterator == m_NSInfos.end()) &&
						(theLocalNode != parent))
					{
						// Record whether this node defines any namespaces
						// (_not_ whether it defines its own or what that is)
						nsInfo = elementHasXMLNS 
							? theNSInfoUnProcWithXMLNS 
							: theNSInfoUnProcWithoutXMLNS;
						updateNamespace(parent, nsInfo);
					}
				}

				// Attr nodes need to look to their owning Element for their NS
				// declaration (if any). Note that we're using the XPath data model,
				// getParentOfNode(); in DOM terms, that's Attr.getOwningElement().
				if(XalanNode::ATTRIBUTE_NODE == parentType)
				{
					parent = DOMServices::getParentOfNode(*parent);
				}

				// Any other node gets queued for annotation pass,
				// along with our current nsInfo context
				else
				{
					candidateNoAncestorXMLNS.push_back(make_pair(parent, nsInfo));

					parent = parent->getParentNode();
				}

				// If we haven't run out of ancestors
				if(0 != parent)
				{
					// Try to retrieve cached NS info for the newly selected parent
					// for use in next pass through loop.
					// If not found, continue using previous value
					theIterator = m_NSInfos.find(parent);

					if (theIterator != m_NSInfos.end())
					{
						nsInfo = (*theIterator).second;
					}
				}
			}

			// Anotation pass over the "any other node" queue
			const unsigned int	nCandidates = candidateNoAncestorXMLNS.size();

			if(nCandidates > 0)
			{
				// If no inherited declarations (and we checked all the way to the root)
				// then record nodes with no local declarations in the nsInfo cache,
				// to avoid repeatedly searching this branch of the tree.
				// ????? This feels overcomplicated, somehow... 
				if(false == ancestorsHaveXMLNS && 0 == parent)
				{
					for(unsigned int i = 0; i < nCandidates; i++)
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

		// NOTE that attibute nodes aren't handled by the nsInfos logic,
		// so it's unclear that the updateNamespace buys us anything directly.
		// But I'm guessing it may be useful for getNamespaceForPrefix.
		//
		// ????? This seems like a lot of code for something which should be a set of
		// bit-masks...
		if(XalanNode::ATTRIBUTE_NODE != ntype)
		{
			assert(namespaceOfPrefix != 0);

			// If Attribute's prefix wasn't resolved
			if(0 == length(*namespaceOfPrefix))
			{
				// In context where other prefixes are defined
				if(ancestorsHaveXMLNS == true)
				{
					// Local definitions exist
					if(nHasXMLNS == true)
					{
						updateNamespace(theLocalNode, theNSInfoNullWithXMLNS);
					}
				
					// Only inherited definitions exist
					else
					{
						updateNamespace(theLocalNode, theNSInfoNullWithoutXMLNS);
					}
				}
			  
				// No definitions exist
				else 
				{
					updateNamespace(theLocalNode, theNSInfoNullNoAncestorXMLNS);
				}
			}
			else // Attribute's prefix was resolved, at least that one is declared
			{
				updateNamespace(theLocalNode, NSInfo(*namespaceOfPrefix, nHasXMLNS));
			}
		}
	}

	return *namespaceOfPrefix;
}
