/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
// Class header file.
#include "TreeWalker.hpp"



#include <cassert>



#include <xalanc/XalanDOM/XalanNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



TreeWalker::TreeWalker()
{
}



TreeWalker::~TreeWalker()
{
}



const XalanNode*
TreeWalker::traverse(const XalanNode*	pos)
{
	assert(pos != 0);

	const XalanNode*	thePos = pos;

	bool	fStop = false;

	while(0 != thePos && fStop == false)
	{
		fStop = startNode(thePos);

		const XalanNode*	nextNode = thePos->getFirstChild();

		while(0 == nextNode)
		{
			if (fStop == false)
			{
				fStop = endNode(thePos);
			}
			else
			{
				endNode(thePos);
			}

			nextNode = thePos->getNextSibling();

			if(0 == nextNode)
			{
				thePos = thePos->getParentNode();

				if(0 == thePos)
				{
					nextNode = thePos;

					break;
				}
			}
		}

		thePos = nextNode;
	}

	return thePos;
}



XalanNode*
TreeWalker::traverse(XalanNode*		pos)
{
	assert(pos != 0);

	XalanNode*	thePos = pos;

	bool	fStop = false;

	while(0 != thePos && fStop == false)
	{
		fStop = startNode(thePos);

		XalanNode*	nextNode = thePos->getFirstChild();

		while(0 == nextNode)
		{
			if (fStop == false)
			{
				fStop = endNode(thePos);
			}
			else
			{
				endNode(thePos);
			}

			nextNode = thePos->getNextSibling();

			if(0 == nextNode)
			{
				thePos = thePos->getParentNode();

				if(0 == thePos)
				{
					nextNode = thePos;

					break;
				}
			}
		}

		thePos = nextNode;
	}

	return thePos;
}



const XalanNode*
TreeWalker::traverse(
			const XalanNode*	pos,
			const XalanNode*	parent)
{
	assert(pos != 0);
	assert(parent != 0);

	const XalanNode*	thePos = pos;

	bool	fStop = false;

	while(parent != thePos && fStop == false)
	{	  
		fStop = startNode(thePos);
	  
		const XalanNode*	nextNode = thePos->getFirstChild();

		while(0 == nextNode)
		{
			if (fStop == false)
			{
				fStop = endNode(thePos);
			}
			else
			{
				endNode(thePos);
			}

			nextNode = thePos->getNextSibling();

			if(0 == nextNode)
			{
				thePos = thePos->getParentNode();
				assert(thePos != 0);

				if(parent == thePos)
				{
					nextNode = thePos;

					break;
				}
			}
		}

		thePos = nextNode;
	}

	return thePos;
}



XalanNode*
TreeWalker::traverse(
			XalanNode*	pos,
			XalanNode*	parent)
{
	assert(pos != 0);
	assert(parent != 0);

	XalanNode*	thePos = pos;

	bool	fStop = false;

	while(parent != thePos && fStop == false)
	{	  
		fStop = startNode(thePos);
	  
		XalanNode*	nextNode = thePos->getFirstChild();

		while(0 == nextNode)
		{
			if (fStop == false)
			{
				fStop = endNode(thePos);
			}
			else
			{
				endNode(thePos);
			}

			nextNode = thePos->getNextSibling();

			if(0 == nextNode)
			{
				thePos = thePos->getParentNode();
				assert(thePos != 0);

				if(parent == thePos)
				{
					nextNode = thePos;

					break;
				}
			}
		}

		thePos = nextNode;
	}

	return thePos;
}



void
TreeWalker::traverseSubtree(const XalanNode*	pos)
{
	if (pos != 0)
	{
		startNode(pos);

		const XalanNode* const	theFirstChild = pos->getFirstChild();

		if (theFirstChild != 0)
		{
			traverse(theFirstChild, pos);
		}

		endNode(pos);
	}
}



void
TreeWalker::traverseSubtree(XalanNode*	pos)
{
	if (pos != 0)
	{
		startNode(pos);

		XalanNode* const	theFirstChild = pos->getFirstChild();

		if (theFirstChild != 0)
		{
			traverse(theFirstChild, pos);
		}

		endNode(pos);
	}
}



XALAN_CPP_NAMESPACE_END
