/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(XERCESDOMWALKER_HEADER_GUARD_1357924680)
#define XERCESDOMWALKER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOMWalker
{
public:

	/**
	 * Constructor.
	 */
	XercesDOMWalker();

	virtual
	~XercesDOMWalker();

	/**
	 * Perform a document-order traversal.
	 *
	 * Derived classes and stop the traversal by returning
	 * true from startNode() or endNode().  If that happens,
	 * the function will return the next node in document
	 * order.  If desired, the caller can start traversing
	 * the tree again from that point.  Note that terminal
	 * nodes will always have startNode() and endNode()
	 * called before the traversal terminates.
	 *
	 * @param pos The node in the tree with which to start the walk
	 *
	 * @return 0 if the traversal completes, or the next node if the traversal doesn't complete.
	 */
	const DOMNodeType*
	traverse(const DOMNodeType*		pos);

	/**
	 * Perform a document-order traversal.
	 *
	 * Derived classes and stop the traversal by returning
	 * true from startNode() or endNode().  If that happens,
	 * the function will return the next node in document
	 * order.  If desired, the caller can start traversing
	 * the tree again from that point.  Note that terminal
	 * nodes will always have startNode() and endNode()
	 * called before the traversal terminates.
	 *
	 * @param pos The node in the tree with which to start the walk
	 *
	 * @return 0 if the traversal completes, or the next node if the traversal doesn't complete.
	 */
	DOMNodeType*
	traverse(DOMNodeType*	pos);

	/**
	 * Perform a document-order traversal stopping at the
	 * provided parent node.
	 *
	 * Derived classes and stop the traversal by returning
	 * true from startNode() or endNode().  If that happens,
	 * the function will return the next node in document
	 * order.  If desired, the caller can start traversing
	 * the tree again from that point.  Note that terminal
	 * nodes will always have startNode() and endNode()
	 * called before the traversal terminates.
	 *
	 * @param pos The node in the tree with which to start the walk
	 * @param parent The parent of pos.  Note that for multiple calls that continue the traversal, this node must remain the same.
	 *
	 * @return parent if the traversal completes, or the next node if the traversal doesn't complete.
	 */
	const DOMNodeType*
	traverse(
			const DOMNodeType*	pos,
			const DOMNodeType*	parent);

	/**
	 * Perform a document-order traversal stopping at the
	 * provided parent node.
	 *
	 * Derived classes and stop the traversal by returning
	 * true from startNode() or endNode().  If that happens,
	 * the function will return the next node in document
	 * order.  If desired, the caller can start traversing
	 * the tree again from that point.  Note that terminal
	 * nodes will always have startNode() and endNode()
	 * called before the traversal terminates.
	 *
	 * @param pos The node in the tree with which to start the walk
	 * @param parent The parent of pos.  Note that for multiple calls that continue the traversal, this node must remain the same.
	 *
	 * @return parent if the traversal completes, or the next node if the traversal doesn't complete.
	 */
	DOMNodeType*
	traverse(
			DOMNodeType*	pos,
			DOMNodeType*	parent);

	/**
	 * Perform a pre-order traversal.
	 * 
	 * @param pos starting node
	 */
	void
	traverseSubtree(const DOMNodeType*	pos);

	/**
	 * Perform a document-order traversal.
	 * 
	 * @param pos starting node
	 */
	void
	traverseSubtree(DOMNodeType*	pos);

protected:

	/**
	 * Called when first walking a node
	 * 
	 * @param node The node
	 *
	 * @return return false if traversal should continue, or true if it should not.
	 */
	virtual bool
	startNode(const DOMNodeType*	node) = 0;

	/**
	 * Called when first walking a node
	 * 
	 * @param node The node
	 *
	 * @return return false if traversal should continue, or true if it should not.
	 */
	virtual bool
	startNode(DOMNodeType*	node);

	/**
	 * Called when leaving a node
	 * 
	 * @param node The node
	 *
	 * @return return false if traversal should continue, or true if it should not.
	 */
	virtual bool
	endNode(const DOMNodeType*	node) = 0;

	/**
	 * Called when leaving a node
	 * 
	 * @param node The node
	 *
	 * @return return false if traversal should continue, or true if it should not.
	 */
	virtual bool
	endNode(DOMNodeType*	node);
};



XALAN_CPP_NAMESPACE_END



#endif	// XERCESDOMWALKER_HEADER_GUARD_1357924680
