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
#include "FunctionDocument.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionDocument::FunctionDocument()
{
}



FunctionDocument::~FunctionDocument()
{
}


typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;



inline void
doWarn(
            XPathExecutionContext&              executionContext,
            const XalanDOMString&               uri,
            const XalanDOMString&               base,
            const XalanNode*                    sourceNode,
            const LocatorType*                  locator)
{
    XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);

    XalanDOMString& theMessage = theGuard.get();

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::CantLoadReqDocument_1Param,
        uri);

    if (length(base) > 0)
    {
        XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);
        XalanDOMString& theTmpString = theGuard.get();

        TranscodeFromLocalCodePage(" (Base URI: ", theTmpString);

        theMessage+= (theTmpString);
        theMessage += base;

        TranscodeFromLocalCodePage(")", theTmpString);
        theMessage += theTmpString;
    }

    executionContext.warn(theMessage, sourceNode, locator);
}



inline XalanDocument*
parseDoc(
            XPathExecutionContext&  executionContext,
            const XalanDOMString&   uri,
            const XalanDOMString&   base,
            const XalanNode*        sourceNode,
            const LocatorType*      locator)
{
    try
    {
        return executionContext.parseXML(executionContext.getMemoryManager(), uri, base);
    }
    catch(...)
    {
        doWarn(executionContext, uri, base, sourceNode, locator);
    }

    return 0;
}



static void
getDoc(
            XPathExecutionContext&              executionContext,
            const XalanDOMString&               uri,
            const XalanDOMString&               base,
            BorrowReturnMutableNodeRefList&     mnl,
            const XalanNode*                    sourceNode,
            const LocatorType*                  locator)
{
    XalanDocument*  newDoc = executionContext.getSourceDocument(uri);

    if(newDoc == 0)
    {
        if(length(uri) != 0)
        {
            newDoc = parseDoc(executionContext, uri, base, sourceNode, locator);
        }
        else
        {
            assert(executionContext.getPrefixResolver() != 0);

            newDoc =
                parseDoc(
                    executionContext,
                    executionContext.getPrefixResolver()->getURI(),
                    base,
                    sourceNode,
                    locator);
        }
    }

    if(newDoc != 0)
    {
        mnl->addNodeInDocOrder(newDoc, executionContext);
    }
}



inline void
getDoc(
            XPathExecutionContext&              executionContext,
            const XalanDOMString&               uri,
            BorrowReturnMutableNodeRefList&     mnl,
            const XalanNode*                    sourceNode,
            const LocatorType*                  locator)
{
    getDoc(
        executionContext,
        uri,
        XalanDOMString(executionContext.getMemoryManager()),
        mnl,
        sourceNode,
        locator);
}



inline void
getDoc(
            XPathExecutionContext&              executionContext,
            const XalanDOMString&               uri,
            const XalanNode*                    resolver,
            BorrowReturnMutableNodeRefList&     mnl,
            const LocatorType*                  locator)
{
    assert(resolver != 0);

    const XalanDocument* const  ownerDocument = XalanNode::DOCUMENT_NODE == resolver->getNodeType() ?
            static_cast<const XalanDocument*>(resolver) :
            resolver->getOwnerDocument();

    getDoc(
        executionContext,
        uri,
        executionContext.findURIFromDoc(ownerDocument),
        mnl,
        resolver,
        locator);
}



XObjectPtr
FunctionDocument::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,            
            const XObjectPtr        arg1,
            const LocatorType*      locator) const
{
    assert(arg1.null() == false);

    if (arg1->getType() == XObject::eTypeNodeSet)
    {
        return doExecute(executionContext, context, arg1, 0, 1, locator);
    }
    else
    {
        XalanDOMString              base(executionContext.getMemoryManager());

        assert(executionContext.getPrefixResolver() != 0);

        base = executionContext.getPrefixResolver()->getURI();

        return doExecute(executionContext, context, arg1, &base, 1, locator);
    }
}



XObjectPtr
FunctionDocument::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,            
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const LocatorType*      locator) const
{
    assert(arg1.null() == false && arg2.null() == false);

    XPathExecutionContext::GetAndReleaseCachedString theGuard(executionContext);

    XalanDOMString& base = theGuard.get();

    bool    fNoRelativeURI = false;

    if (context == 0)
    {
        XPathExecutionContext::GetAndReleaseCachedString theGuard1(executionContext);

        executionContext.error(
            XalanMessageLoader::getMessage(
                theGuard1.get(),
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "document"),
            context,
            locator);

        return XObjectPtr();
    }
    else
    {
        if(XObject::eTypeNodeSet != arg2->getType())
        {
            XPathExecutionContext::GetAndReleaseCachedString theGuard1(executionContext);

            executionContext.error(
                XalanMessageLoader::getMessage(
                    theGuard1.get(),
                    XalanMessages::SecondArgumentToFunctionMustBeNode_set_1Param,
                    "document"),
                context,
                locator);
        }
        else
        {
            const NodeRefListBase&  nodeset = arg2->nodeset();

            if (nodeset.getLength() == 0)
            {
                // The errata require that we refuse to resolve a relative URI if
                // an empty node-set is provided as the second argument.
                fNoRelativeURI = true;
            }
            else
            {
                XalanNode* const    baseNode =  nodeset.item(0);
                assert(baseNode != 0);

                XalanDocument* const    baseDoc = XalanNode::DOCUMENT_NODE == baseNode->getNodeType() ?
                    static_cast<XalanDocument*>(baseNode) :
                    baseNode->getOwnerDocument();

                base = executionContext.findURIFromDoc(baseDoc);
            }
        }
    }

    return doExecute(executionContext, context, arg1, &base, 2, locator, fNoRelativeURI);
}



XObjectPtr
FunctionDocument::doExecute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr&       arg,
            XalanDOMString*         base,
            int                     argCount,
            const LocatorType*      locator,
            bool                    fNoRelativeURI) const
{
    // This list will hold the nodes...
    BorrowReturnMutableNodeRefList  mnl(executionContext);

    const XObject::eObjectType          theType = arg->getType();

    const NodeRefListBase::size_type    nRefs = XObject::eTypeNodeSet == theType ?
        arg->nodeset().getLength()
                                                : 1;

    for(NodeRefListBase::size_type i = 0; i < nRefs; i++)
    {
        assert(XObject::eTypeNodeSet != theType ||
                            arg->nodeset().item(i) != 0);

        const XalanNode*    resolver = 0;

        XPathExecutionContext::GetAndReleaseCachedString theGuard(executionContext);

        XalanDOMString&     ref = theGuard.get();

        if (theType != XObject::eTypeNodeSet)
        {
            ref = arg->str();
        }
        else
        {
            resolver = arg->nodeset().item(i);
            assert(resolver != 0);

            DOMServices::getNodeData(*resolver,ref);
        }

        // This is the case where the function was called with
        // an empty string, which refers to the stylesheet itself.
        if (nRefs == 1 && isEmpty(ref) == true && argCount == 1)
        {
            if (base != 0)
            {
                clear(*base);
            }

            ref = executionContext.getPrefixResolver()->getURI();
        }

        if(!isEmpty(ref))
        {
            // From http://www.ics.uci.edu/pub/ietf/uri/rfc1630.txt
            // A partial form can be distinguished from an absolute form in that the
            // latter must have a colon and that colon must occur before any slash
            // characters. Systems not requiring partial forms should not use any
            // unencoded slashes in their naming schemes.  If they do, absolute URIs
            // will still work, but confusion may result.
            const XalanDOMString::size_type     theLength = length(ref);

            const XalanDOMString::size_type     indexOfColon = indexOf(ref, XalanUnicode::charColon);
            XalanDOMString::size_type           indexOfSlash = indexOf(ref, XalanUnicode::charSolidus);

#if defined(XALAN_WINDOWS)              
			// $$$TODO
			// Note this is hackish code for Windows, and we should try to remove it.
            const XalanDOMString::size_type     indexOfBackSlash = indexOf(ref, XalanUnicode::charReverseSolidus);

            if(indexOfBackSlash > indexOfSlash && indexOfBackSlash < theLength)
            {
                indexOfSlash = indexOfBackSlash;
            }
#endif              

            if(indexOfColon < theLength &&
               indexOfSlash < theLength &&
               indexOfColon < indexOfSlash)
            {
                // The ref is absolute...
                getDoc(executionContext, ref, mnl, context, locator);
            }
            else
            {
                // The ref is relative.  If there was a base URI
                // provided, use that...
                if (fNoRelativeURI == true)
                {
                    XPathExecutionContext::GetAndReleaseCachedString theGuard(executionContext);

                    executionContext.warn(
                        XalanMessageLoader::getMessage(
                            theGuard.get(),
                            XalanMessages::CannotResolveURIInDocumentFunction),
                        context,
                        locator);
                }
                else if (base != 0)
                {
                    getDoc(executionContext, ref, *base, mnl, context, locator);
                }
                else
                {
                    // If there's no resolver, then try using the
                    // relative ref...
                    if (resolver == 0)
                    {
                        getDoc(executionContext, ref, mnl, context, locator);
                    }
                    else
                    {
                        getDoc(executionContext, ref, resolver, mnl, locator);
                    }
                }
            }
        }
    }

    assert(mnl->checkForDuplicates(executionContext.getMemoryManager()) == false);

    mnl->setDocumentOrder();

    return executionContext.getXObjectFactory().createNodeSet(mnl);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionDocument*
#endif
FunctionDocument::clone(MemoryManagerType&  theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionDocument::getError(XalanDOMString&  theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionAcceptsOneTwoArguments_1Param,
                "document()");
}



XALAN_CPP_NAMESPACE_END
