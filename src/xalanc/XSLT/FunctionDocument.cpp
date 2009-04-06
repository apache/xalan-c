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



#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/sax/SAXParseException.hpp"



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
typedef Function::GetCachedString   GetCachedString;



inline void
doWarn(
            XPathExecutionContext&  executionContext,
            const XalanDOMString&   uri,
            const XalanDOMString&   base,
            const XalanNode*        sourceNode,
            const Locator*          locator)
{
    const GetCachedString   theGuard(executionContext);

    XalanDOMString&     theMessage = theGuard.get();

    const XalanMessages::Codes  theMessageID = base.empty() ?
            XalanMessages::CantLoadReqDocument_1Param :
            XalanMessages::CantLoadReqDocument_2Param;

    XalanMessageLoader::getMessage(
        theMessage,
        theMessageID,
        uri,
        base);

    executionContext.problem(
        XPathExecutionContext::eXSLTProcessor,
        XPathExecutionContext::eWarning,
        theMessage,
        locator,
        sourceNode);
}



inline void
doWarnFragID(
            XPathExecutionContext&  executionContext,
            const XalanDOMString&   uri,
            const XalanDOMString&   fragID,
            const XalanNode*        sourceNode,
            const Locator*          locator)
{
    const GetCachedString   theGuard(executionContext);

    XalanDOMString&     theMessage = theGuard.get();

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::CannotFindFragment_2Param,
        uri,
        fragID);

    executionContext.problem(
        XPathExecutionContext::eXSLTProcessor,
        XPathExecutionContext::eWarning,
        theMessage,
        locator,
        sourceNode);
}



class LocalErrorHandler : public ErrorHandler
{
public:

    LocalErrorHandler(
            XPathExecutionContext&  theExecutionContext,
            const Locator*          theLocator,
            const XalanNode*        theSourceNode) :
        m_executionContext(theExecutionContext),
        m_locator(theLocator),
        m_sourceNode(theSourceNode),
        m_fatal(false)
    {
    }

    virtual void
    warning(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc)
    {
        const GetCachedString   theGuard(m_executionContext);

        XalanDOMString&     theMessage = theGuard.get();

        theMessage = exc.getMessage();

        m_executionContext.problem(
            XPathExecutionContext::eXMLParser,
            XPathExecutionContext::eWarning,
            theMessage,
            m_locator,
            m_sourceNode);
    }

    virtual void
    error(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc)
    {
        warning(exc);
    }

    virtual void
    fatalError(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exc)
    {
        m_fatal = true;

        warning(exc);

        throw exc;
    }

    virtual void
    resetErrors()
    {
    }

    bool
    getFatalError() const
    {
        return m_fatal;
    }

private:

    XPathExecutionContext&  m_executionContext;

    const Locator* const    m_locator;

    const XalanNode* const  m_sourceNode;

    bool                    m_fatal;
};



inline XalanDocument*
parseDoc(
            XPathExecutionContext&  executionContext,
            const XalanDOMString&   uri,
            const XalanDOMString&   base,
            const XalanNode*        sourceNode,
            const Locator*          locator)
{
    LocalErrorHandler   theErrorHandler(executionContext, locator, sourceNode);

    try
    {
        return executionContext.parseXML(
                    executionContext.getMemoryManager(),
                    uri,
                    base,
                    &theErrorHandler);
    }
    catch(...)
    {
        if (theErrorHandler.getFatalError())
        {
            doWarn(executionContext, uri, base, sourceNode, locator);
        }
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
            const Locator*                      locator)
{
    const XalanDOMString::size_type fragIndex =
        indexOf(uri, XalanUnicode::charNumberSign);

    const GetCachedString   theGuard1(executionContext);
    const GetCachedString   theGuard2(executionContext);

    XalanDOMString&     newURI = theGuard1.get();
    XalanDOMString&     fragID = theGuard2.get();

    if (fragIndex == uri.length())
    {
        newURI = uri;
    }
    else
    {
        substring(uri, newURI, 0, fragIndex);
        substring(uri, fragID, fragIndex + 1);
    }

    XalanDocument*  newDoc = executionContext.getSourceDocument(newURI);

    if (newDoc == 0)
    {
        if (newURI.length() != 0)
        {
            newDoc = parseDoc(executionContext, newURI, base, sourceNode, locator);
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

    if (newDoc != 0)
    {
        if (fragID.length() == 0)
        {
            mnl->addNodeInDocOrder(newDoc, executionContext);
        }
        else
        {
            XalanNode* const    theNode = newDoc->getElementById(fragID);

            if (theNode != 0)
            {
                mnl->addNodeInDocOrder(theNode, executionContext);
            }
            else
            {
                doWarnFragID(executionContext, fragID, newURI, sourceNode, locator);
            }
        }
    }
}



inline void
getDoc(
            XPathExecutionContext&              executionContext,
            const XalanDOMString&               uri,
            BorrowReturnMutableNodeRefList&     mnl,
            const XalanNode*                    sourceNode,
            const Locator*                      locator)
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
            const Locator*                      locator)
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
            const Locator*          locator) const
{
    assert(arg1.null() == false);

    if (arg1->getType() == XObject::eTypeNodeSet)
    {
        return doExecute(executionContext, context, arg1, 0, 1, locator);
    }
    else
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString&     base = theGuard.get();

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
            const Locator*          locator) const
{
    assert(arg1.null() == false && arg2.null() == false);

    const GetCachedString   theGuard(executionContext);

    XalanDOMString& base = theGuard.get();

    bool    fNoRelativeURI = false;

    if (context == 0)
    {
        const GetCachedString   theGuard1(executionContext);

        executionContext.problem(
            XPathExecutionContext::eXSLTProcessor,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard1.get(),
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "document"),
            locator,
            context);

        return XObjectPtr();
    }
    else
    {
        if (XObject::eTypeNodeSet != arg2->getType())
        {
            const GetCachedString   theGuard1(executionContext);

            executionContext.problem(
                XPathExecutionContext::eXSLTProcessor,
                XPathExecutionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard1.get(),
                    XalanMessages::SecondArgumentToFunctionMustBeNode_set_1Param,
                    "document"),
                locator,
                context);
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
            const Locator*          locator,
            bool                    fNoRelativeURI) const
{
    // This list will hold the nodes...
    BorrowReturnMutableNodeRefList  mnl(executionContext);

    const XObject::eObjectType          theType = arg->getType();

    const NodeRefListBase::size_type    nRefs = XObject::eTypeNodeSet == theType ?
                                                arg->nodeset().getLength()
                                                : 1;

    for (NodeRefListBase::size_type i = 0; i < nRefs; i++)
    {
        assert(XObject::eTypeNodeSet != theType ||
                            arg->nodeset().item(i) != 0);

        const XalanNode*    resolver = 0;

        const GetCachedString   theGuard(executionContext);

        XalanDOMString&     ref = theGuard.get();

        if (theType != XObject::eTypeNodeSet)
        {
            ref = arg->str(executionContext);
        }
        else
        {
            resolver = arg->nodeset().item(i);
            assert(resolver != 0);

            DOMServices::getNodeData(*resolver, executionContext, ref);
        }

        // This is the case where the function was called with
        // an empty string, which refers to the stylesheet itself.
        if (nRefs == 1 && ref.empty() == true && argCount == 1)
        {
            if (base != 0)
            {
                base->clear();
            }

            ref = executionContext.getPrefixResolver()->getURI();
        }

        if (!ref.empty())
        {
            // From http://www.ics.uci.edu/pub/ietf/uri/rfc1630.txt
            // A partial form can be distinguished from an absolute form in that the
            // latter must have a colon and that colon must occur before any slash
            // characters. Systems not requiring partial forms should not use any
            // unencoded slashes in their naming schemes.  If they do, absolute URIs
            // will still work, but confusion may result.
            const XalanDOMString::size_type     theLength = ref.length();

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

            if (indexOfColon < theLength &&
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
                    const GetCachedString   theGuard(executionContext);

                    executionContext.problem(
                        XPathExecutionContext::eXSLTProcessor,
                        XPathExecutionContext::eWarning,
                        XalanMessageLoader::getMessage(
                            theGuard.get(),
                            XalanMessages::CannotResolveURIInDocumentFunction),
                        locator,
                        context);
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
FunctionDocument::clone(MemoryManager&  theManager) const
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
