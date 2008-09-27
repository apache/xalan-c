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


#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)


// Class header file.
#include "XercesTreeWalker.hpp"



// Xerces header files...
#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#endif



XALAN_CPP_NAMESPACE_BEGIN



XercesTreeWalker::XercesTreeWalker()
{
}



XercesTreeWalker::~XercesTreeWalker()
{
}



void
XercesTreeWalker::traverse(const DOM_NodeType&  pos)
{
    DOM_NodeType    thePos(pos);

    while(thePos != 0)
    {
        startNode(thePos);

        DOM_NodeType    nextNode = thePos.getFirstChild();

        while(nextNode == 0)
        {
            endNode(thePos);

            nextNode = thePos.getNextSibling();

            if(nextNode == 0)
            {
                thePos = thePos.getParentNode();

                if(thePos == 0)
                {
                    nextNode = thePos;

                    break;
                }
            }
        }

        thePos = nextNode;
    }
}



void
XercesTreeWalker::traverse(
            const DOM_NodeType&     pos,
            const DOM_NodeType&     parent)
{
    DOM_NodeType    thePos(pos);

    while(parent != thePos)
    {     
        startNode(thePos);
      
        DOM_NodeType    nextNode = thePos.getFirstChild();

        while(nextNode == 0)
        {
            endNode(thePos);

            nextNode = thePos.getNextSibling();

            if(nextNode == 0)
            {
                thePos = thePos.getParentNode();

                if(parent == thePos)
                {
                    nextNode = thePos;

                    break;
                }
            }
        }

        thePos = nextNode;
    }
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


