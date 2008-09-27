/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
TreeWalker::traverse(const XalanNode*   pos)
{
    assert(pos != 0);

    const XalanNode*    thePos = pos;

    bool    fStop = false;

    while(0 != thePos && fStop == false)
    {
        fStop = startNode(thePos);

        const XalanNode*    nextNode = thePos->getFirstChild();

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
TreeWalker::traverse(XalanNode*     pos)
{
    assert(pos != 0);

    XalanNode*  thePos = pos;

    bool    fStop = false;

    while(0 != thePos && fStop == false)
    {
        fStop = startNode(thePos);

        XalanNode*  nextNode = thePos->getFirstChild();

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
            const XalanNode*    pos,
            const XalanNode*    parent)
{
    assert(pos != 0);
    assert(parent != 0);

    const XalanNode*    thePos = pos;

    bool    fStop = false;

    while(parent != thePos && fStop == false)
    {     
        fStop = startNode(thePos);
      
        const XalanNode*    nextNode = thePos->getFirstChild();

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
            XalanNode*  pos,
            XalanNode*  parent)
{
    assert(pos != 0);
    assert(parent != 0);

    XalanNode*  thePos = pos;

    bool    fStop = false;

    while(parent != thePos && fStop == false)
    {     
        fStop = startNode(thePos);
      
        XalanNode*  nextNode = thePos->getFirstChild();

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
TreeWalker::traverseSubtree(const XalanNode*    pos)
{
    if (pos != 0)
    {
        startNode(pos);

        const XalanNode* const  theFirstChild = pos->getFirstChild();

        if (theFirstChild != 0)
        {
            traverse(theFirstChild, pos);
        }

        endNode(pos);
    }
}



void
TreeWalker::traverseSubtree(XalanNode*  pos)
{
    if (pos != 0)
    {
        startNode(pos);

        XalanNode* const    theFirstChild = pos->getFirstChild();

        if (theFirstChild != 0)
        {
            traverse(theFirstChild, pos);
        }

        endNode(pos);
    }
}



XALAN_CPP_NAMESPACE_END
