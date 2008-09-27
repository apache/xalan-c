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
#include "XercesDOMSupport.hpp"



#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMDocumentType.hpp"
#include "xercesc/dom/DOMEntity.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp"



#include "xalanc/DOMSupport/DOMServices.hpp"



#include "xalanc/XercesParserLiaison/XercesDocumentWrapper.hpp"
#include "xalanc/XercesParserLiaison/XercesParserLiaison.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XercesDOMSupport::XercesDOMSupport(XercesParserLiaison&     theLiaison) :
    DOMSupport(),
    m_liaison(theLiaison)
{
}



XercesDOMSupport::~XercesDOMSupport()
{
}



void
XercesDOMSupport::reset()
{
}



const XalanDOMString&
XercesDOMSupport::getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const
{
    const XercesDocumentWrapper* const  theWrapper =
        m_liaison.mapDocumentToWrapper(&theDocument);

    if (theWrapper != 0)
    {
        XALAN_USING_XERCES(DOMDocument)
        XALAN_USING_XERCES(DOMDocumentType)
        XALAN_USING_XERCES(DOMEntity)
        XALAN_USING_XERCES(DOMNamedNodeMap)
        XALAN_USING_XERCES(DOMNode)

        const DOMDocument* const    theDOMDocument =
            theWrapper->getXercesDocument();
        assert(theDOMDocument != 0);

        const DOMDocumentType* const    theDoctype =
            theDOMDocument->getDoctype();

        if  (theDoctype != 0)
        {
            const DOMNamedNodeMap* const    theEntities =
                theDoctype->getEntities();

            if (theEntities != 0)
            {
                const DOMNode* const    theNode =
                    theEntities->getNamedItem(theName.c_str());

                if (theNode != 0 && theNode->getNodeType() == DOMNode::ENTITY_NODE)
                {
                    const DOMEntity* const    theEntity =
                        static_cast<const DOMEntity*>(theNode);

                    if(length(theEntity->getNotationName()) != 0) // then it's unparsed
                    {
                        // The draft says: "The XSLT processor may use the public
                        // identifier to generate a URI for the entity instead of the URI
                        // specified in the system identifier. If the XSLT processor does
                        // not use the public identifier to generate the URI, it must use
                        // the system identifier; if the system identifier is a relative
                        // URI, it must be resolved into an absolute URI using the URI of
                        // the resource containing the entity declaration as the base
                        // URI [RFC2396]."
                        // So I'm falling a bit short here.
                        const XMLCh* theURI = theEntity->getSystemId();

                        if(theURI == 0)
                        {
                            theURI = theEntity->getPublicId();
                        }

                        if(theURI != 0)
                        {
                            return theWrapper->getPooledString(theURI);
                        }
                    }
                }
            }
        }
    }

    return s_emptyString;
}



bool
XercesDOMSupport::isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const
{
    return DOMServices::isNodeAfter(node1, node2);
}



XALAN_CPP_NAMESPACE_END
