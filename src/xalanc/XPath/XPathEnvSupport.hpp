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
#if !defined(XPATHENVSUPPORT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include "xalanc/PlatformSupport/ProblemListenerBase.hpp"



#include <xalanc/XPath/Function.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



class NodeRefListBase;
class PrefixResolver;
class XObject;
class XPathExecutionContext;
class XObjectFactory;
class XalanDocument;
class XalanDOMString;
class XalanElement;
class XalanNode;
class XalanQName;



class XALAN_XPATH_EXPORT XPathEnvSupport : public ProblemListenerBase
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator  LocatorType;

    typedef Function::XObjectArgVectorType  XObjectArgVectorType;

    XPathEnvSupport();

    virtual
    ~XPathEnvSupport();


    // These methods are inherited from ProblemListener ...
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


    // These interfaces are new to XPathEnvSupport
    /**
     * Provides support for XML parsing service.
     *
     * @param theManager The MemoryManager instance to use.
     * @param urlString location of the XML
     * @param base base location for URI
     * @param theErrorHandler An optional ErrorHandler instance for error reporting.
     * @return parsed document
     */
    virtual XalanDocument*
    parseXML(
            MemoryManager&          theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base,
            ErrorHandler*           theErrorHandler = 0) = 0;

    /**
     * Get the source document for the given URI.
     *
     * @param theURI document URI
     * @return source document
     */
    virtual XalanDocument*
    getSourceDocument(const XalanDOMString&     theURI) const = 0;

    /**
     * Associate a document with a given URI.
     *
     * @param theURI      document URI
     * @param theDocument source document
     */
    virtual void
    setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument) = 0;

    /**
     * Given a DOM Document, tell what URI was used to parse it. Needed for
     * relative resolution.
     *
     * @param owner source document
     * @return document URI
     */
    virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const = 0;

    /**
     * Determine if an external element is available.
     *
     * @param theNamespace  namespace for the element
     * @param elementName name of extension element
     * @return whether the element is available or not
     */
    virtual bool
    elementAvailable(
            const XalanDOMString&   theNamespace, 
            const XalanDOMString&   elementName) const = 0;

    /**
     * Determine if a function is available.  For standard
     * function availability, theNamespace should be an
     * empty string.
     *
     * @param theNamespace  namespace for the function
     * @param functionName name of the function
     * @return whether the function is available or not
     */
    virtual bool
    functionAvailable(
            const XalanDOMString&   theNamespace, 
            const XalanDOMString&   functionName) const = 0;

    /**
     * Handle an extension function.
     * 
     * @param executionContext  current execution context
     * @param theNamespace  namespace of function    
     * @param functionName extension function name
     * @param argVec        vector of arguments to function
     * @param locator A Locator instance for error reporting.
     * @return pointer to XObject result
     */
    virtual XObjectPtr
    extFunction(
            XPathExecutionContext&          executionContext,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName, 
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const Locator*                  locator) const = 0;

    /**
     * Reset the instance.
     */
    virtual void
    reset() = 0;

private:

    // These are not implemented...
    XPathEnvSupport(const XPathEnvSupport&);

    XPathEnvSupport&
    operator=(const XPathEnvSupport&);

    bool
    operator==(const XPathEnvSupport&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHENVSUPPORT_HEADER_GUARD_1357924680
