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
// Class header file
#include "KeyTable.hpp"



#include <memory>



#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanNodeList.hpp>



#include <XPath/XPath.hpp>



#include "KeyDeclaration.hpp"
#include "StylesheetExecutionContext.hpp"


const MutableNodeRefList	KeyTable::s_dummyList;



KeyTable::KeyTable(
			XalanNode*							doc,
			XalanNode*							startNode,
			const PrefixResolver&				resolver,
			const XalanDOMString&				name,
			const KeyDeclarationVectorType&		keyDeclarations,
			StylesheetExecutionContext&			executionContext) :
	m_docKey(doc),
	m_keys()
{
    XalanNode*	pos = startNode;

	bool		fDone = false;

	const KeyDeclarationVectorType::size_type	nDeclarations =
			keyDeclarations.size();

    // Do a non-recursive pre-walk over the tree.
    while(0 != pos && fDone == false)
    {
		// We're going to have to walk the attribute list 
		// if it's an element, so get the attributes.
		const XalanNamedNodeMap*	attrs = 0;

		int							nNodes = 0;

		if(XalanNode::ELEMENT_NODE == pos->getNodeType())
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			attrs = ((const XalanElement*)pos)->getAttributes();
#else
			attrs = static_cast<const XalanElement*>(pos)->getAttributes();
#endif
			nNodes = attrs->getLength();
        
			if(0 == nNodes)
			{
				attrs = 0;
			}
		}

		// Walk the primary node, and each of the attributes.
		// This loop is a little strange... it is meant to always 
		// execute once, then execute for each of the attributes.
		XalanNode*	testNode = pos;

		for(int nodeIndex = -1; nodeIndex < nNodes && fDone == false;)
		{
			// Walk through each of the declarations made with xsl:key
			for(unsigned int i = 0; i < nDeclarations && fDone == false; i++)
			{
				const KeyDeclaration&	kd = keyDeclarations[i];

				if (equals(kd.getName(), name))
				{
					if (executionContext.getInConstruction(kd) == true)			
					{
						fDone = true;
					}
					else
					{
						executionContext.beginConstruction(kd);

						// See if our node matches the given key declaration according to 
						// the match attribute on xsl:key.
						assert(kd.getMatchPattern() != 0);

						const double	score =
							kd.getMatchPattern()->getMatchScore(testNode,
																resolver,
																executionContext);

						if(score == XPath::s_MatchScoreNone)
						{
							executionContext.endConstruction(kd);
						}
						else
						{
							// Query from the node, according the the select pattern in the
							// use attribute in xsl:key.
							assert(kd.getUse() != 0);

							const XObject* const	xuse =
								kd.getUse()->execute(testNode, resolver, NodeRefList(), executionContext);

							const NodeRefListBase*	nl = 0;
							unsigned int nUseValues;
							XalanDOMString		exprResult;
							if(xuse->getType() != xuse->eTypeNodeSet)
							{
								nUseValues = 1;
								exprResult = xuse->str();
							}
							else
							{
								nl = &xuse->nodeset();
								// Use each node in the node list as a key value that we'll be 
								// able to use to look up the given node.
								nUseValues = nl->getLength();
							}  
							// Use each node in the node list as a key value that we'll be 
							// able to use to look up the given node.
							for(unsigned int k = 0; k < nUseValues; k++)
							{

								// Use getExpr to get the string value of the given node. I hope 
								// the string assumption is the right thing... I can't see how 
								// it could work any other way.
								if(0 != nl)
								{
									XalanNode* const	useNode = nl->item(k);
									assert(useNode != 0);

									exprResult = useNode->getXSLTData();
								}

								MutableNodeRefList&		keyNodes =
										m_keys[kd.getName()][exprResult];

								// See if the matched node is already in the 
								// table set.  If it is there, we're done, otherwise 
								// add it.
								bool foundit = false;

								const unsigned int	nKeyNodes = keyNodes.getLength(); //size();

								for(unsigned int j = 0; j < nKeyNodes; j++)
								{
									if(testNode == keyNodes.item(j))
									{
										foundit = true;
										break;
									}
								} // end for j

								if(foundit == false)
								{
									keyNodes.addNode(testNode);
								}
							} // end for(int k = 0; k < nUseValues; k++)

							executionContext.endConstruction(kd);
						} // if(score != kd.getMatchPattern().s_MatchScoreNone)
					} // if (kd.getInConstruction() == true)
				} // if (equals(kd.getName(), name)
			} // end for(int i = 0; i < nDeclarations; i++)

			if (fDone == false)
			{
				nodeIndex++;

				if(0 != attrs)
				{
					testNode = attrs->item(nodeIndex);
				}
			}
		} // for(int nodeIndex = -1; nodeIndex < nNodes;)

		if (fDone == false)
		{
			// The rest of this is getting the next prewalk position in 
			// the tree.

			XalanNode*	nextNode = pos->getFirstChild();

			while(0 == nextNode)
			{
				if(startNode == pos)
				{
					break;
				}
				else
				{
					nextNode = pos->getNextSibling();

					if(0 == nextNode)
					{
						pos = pos->getParentNode();

						if((startNode == pos) || (0 == pos))
						{
							nextNode = 0;
							break;
						}
					}
				}
			}

			pos = nextNode;
		} // if (fDone == false)
    } // while(0 != pos)
} // end buildKeysTable method



KeyTable::~KeyTable()
{
}



const NodeRefListBase*
KeyTable::getNodeSetByKey(
					  const XalanDOMString&		name, 
					  const XalanDOMString&		ref) const
{
	// It makes things much easier if we always return
	// a list of nodes.  So this is just an empty one
	// to return when the ref is not found.
    const MutableNodeRefList*			nl = &s_dummyList;

	const KeysMapType::const_iterator	i = m_keys.find(name);

	if (i != m_keys.end())
	{
		const NodeListMapType&					theMap = (*i).second;

		const NodeListMapType::const_iterator	j = theMap.find(ref);

		if (j != theMap.end())
		{
			nl = &(*j).second;
		}
	}

	return nl;
}

