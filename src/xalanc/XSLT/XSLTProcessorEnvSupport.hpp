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
#if !defined(XSLTPROCESSORENVSUPPORT_HEADER_GUARD_1357924680)
#define XSLTPROCESSORENVSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/XPathEnvSupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ElemTemplateElement;
class XSLTProcessor;



class XALAN_XSLT_EXPORT XSLTProcessorEnvSupport : public XPathEnvSupport
{
public:

    XSLTProcessorEnvSupport();

    virtual
    ~XSLTProcessorEnvSupport();


    // These interfaces are inherited from XPathEnvSupport...

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode) = 0;

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode) = 0;

    virtual XalanDocument*
    parseXML(
            MemoryManager&          theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base,
            ErrorHandler*           theErrorHandler = 0) = 0;

    virtual XalanDocument*
    getSourceDocument(const XalanDOMString&     theURI) const = 0;

    virtual void
    setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument) = 0;

    virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const = 0;

    virtual bool
    elementAvailable(
            const XalanDOMString&   theNamespace, 
            const XalanDOMString&   elementName) const = 0;

    virtual bool
    functionAvailable(
            const XalanDOMString&   theNamespace, 
            const XalanDOMString&   functionName) const = 0;

    virtual XObjectPtr
    extFunction(
            XPathExecutionContext&          executionContext,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const Locator*                  locator) const = 0;

    virtual void
    reset() = 0;

private:

    // These are not implemented...
    XSLTProcessorEnvSupport(const XSLTProcessorEnvSupport&);

    XSLTProcessorEnvSupport&
    operator=(const XSLTProcessorEnvSupport&);

    bool
    operator==(const XSLTProcessorEnvSupport&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // XSLTPROCESSORENVSUPPORT_HEADER_GUARD_1357924680
