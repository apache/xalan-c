/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
