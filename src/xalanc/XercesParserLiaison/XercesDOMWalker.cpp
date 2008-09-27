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
#include "XercesDOMWalker.hpp"



#include <cassert>



// Xerces header files...
#include <xercesc/dom/DOMNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMWalker::XercesDOMWalker()
{
}



XercesDOMWalker::~XercesDOMWalker()
{
}



const DOMNodeType*
XercesDOMWalker::traverse(const DOMNodeType*    pos)
{
    assert(pos != 0);

    const DOMNodeType*  thePos = pos;

    bool    fStop = false;

    while(thePos != 0 && fStop == false)
    {
        fStop = startNode(thePos);

        const DOMNodeType*  nextNode = thePos->getFirstChild();

        while(nextNode == 0)
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

            if(nextNode == 0)
            {
                thePos = thePos->getParentNode();

                if(thePos == 0)
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



DOMNodeType*
XercesDOMWalker::traverse(DOMNodeType*  pos)
{
    assert(pos != 0);

    DOMNodeType*    thePos = pos;

    bool    fStop = false;

    while(thePos != 0 && fStop == false)
    {
        fStop = startNode(thePos);

        DOMNodeType*    nextNode = thePos->getFirstChild();

        while(nextNode == 0)
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

            if(nextNode == 0)
            {
                thePos = thePos->getParentNode();

                if(thePos == 0)
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



const DOMNodeType*
XercesDOMWalker::traverse(
            const DOMNodeType*  pos,
            const DOMNodeType*  parent)
{
    assert(pos != 0);
    assert(parent != 0);

    const DOMNodeType*  thePos = pos;

    bool    fStop = false;

    while(parent != thePos && fStop == false)
    {
        fStop = startNode(thePos);
      
        const DOMNodeType*  nextNode = thePos->getFirstChild();

        while(nextNode == 0)
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

            if(nextNode == 0)
            {
                thePos = thePos->getParentNode();

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



DOMNodeType*
XercesDOMWalker::traverse(
            DOMNodeType*    pos,
            DOMNodeType*    parent)
{
    assert(pos != 0);
    assert(parent != 0);

    DOMNodeType*    thePos = pos;

    bool    fStop = false;

    while(parent != thePos && fStop == false)
    {
        fStop = startNode(thePos);
      
        DOMNodeType*    nextNode = thePos->getFirstChild();

        while(nextNode == 0)
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

            if(nextNode == 0)
            {
                thePos = thePos->getParentNode();

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
XercesDOMWalker::traverseSubtree(const DOMNodeType*     pos)
{
    if (pos != 0)
    {
        startNode(pos);

        const DOMNodeType* const    theFirstChild = pos->getFirstChild();

        if (theFirstChild != 0)
        {
            traverse(theFirstChild, pos);
        }

        endNode(pos);
    }
}



void
XercesDOMWalker::traverseSubtree(DOMNodeType*   pos)
{
    if (pos != 0)
    {
        startNode(pos);

        DOMNodeType* const  theFirstChild = pos->getFirstChild();

        if (theFirstChild != 0)
        {
            traverse(theFirstChild, pos);
        }

        endNode(pos);
    }
}



bool
XercesDOMWalker::startNode(DOMNodeType*     node)
{
    return startNode(const_cast<const DOMNodeType*>(node));
}



bool
XercesDOMWalker::endNode(DOMNodeType*   node)
{
    return endNode(const_cast<const DOMNodeType*>(node));
}



XALAN_CPP_NAMESPACE_END
