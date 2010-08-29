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
#if !defined(XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>
#include <xalanc/XPath/XPathEnvSupport.hpp>



#include<xalanc/PlatformSupport/DOMStringHelper.hpp>



#include<xalanc/Include/XalanMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



typedef XalanMap<XalanDOMString, const Function*>   FunctionTableTypeDefinition;
XALAN_USES_MEMORY_MANAGER(FunctionTableTypeDefinition)



class XALAN_XPATH_EXPORT XPathEnvSupportDefault : public XPathEnvSupport
{
public:

    typedef XalanMap<XalanDOMString, XalanDocument*>    SourceDocsTableType;
    typedef FunctionTableTypeDefinition                 FunctionTableType;
    typedef XalanMap<XalanDOMString, FunctionTableType> NamespaceFunctionTablesType;

    /**
     * Perform initialization of statics -- must be called before any
     * processing occurs.  See class XPathInit.
     */
    static void
    initialize(MemoryManager&   theManager);

    /**
     * Perform termination of statics.  See class XPathInit.
     */
    static void
    terminate();


    XPathEnvSupportDefault(MemoryManager&   theManager XALAN_DEFAULT_MEMMGR);

    virtual
    ~XPathEnvSupportDefault();

    MemoryManager& 
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    virtual void
    setPrintWriter(PrintWriter*     pw);

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode);

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode);

    // Interfaces to install and uninstall external functions globally.
    // These calls are not thread-safe, and should happen during
    // processing.

    /**
     * Install an external function in the global space.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     * @param function The function to install.
     */
    static void
    installExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function);

    /**
     * Uninstall an external function from the global space.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     */
    static void
    uninstallExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName);

    // Interfaces to install and uninstall external functions in this instance.

    /**
     * Install an external function in the local space.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     * @param function The function to install.
     */
    virtual void
    installExternalFunctionLocal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function);

    /**
     * Uninstall an external function from the local space.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     */
    virtual void
    uninstallExternalFunctionLocal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName);


    // These interfaces are inherited from XPathEnvSupport...

    virtual XalanDocument*
    parseXML(
            MemoryManager&      theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base,
            ErrorHandler*           theErrorHandler = 0);

    virtual XalanDocument*
    getSourceDocument(const XalanDOMString& theURI) const;

    virtual void
    setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument);

    virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const;

    virtual bool
    elementAvailable(
            const XalanDOMString&   theNamespace, 
            const XalanDOMString&   elementName) const;

    virtual bool
    functionAvailable(
            const XalanDOMString&   theNamespace, 
            const XalanDOMString&   functionName) const;

    virtual XObjectPtr
    extFunction(
            XPathExecutionContext&          executionContext,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName, 
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const Locator*                  locator) const;

    virtual void
    reset();

    // Delete functor for table cleanup...
    struct NamespaceFunctionTableDeleteFunctor
    {
        typedef FunctionTableType               FunctionTableInnerType;
        typedef NamespaceFunctionTablesType     NamespaceFunctionTablesInnerType;

        NamespaceFunctionTableDeleteFunctor(MemoryManager&  theManager);
        /**
         * Delete the value object in a map value pair.  The value of the pair must
         * be of pointer type.
         *
         * @param thePair key-value pair
         */
        void
        operator()(const NamespaceFunctionTablesInnerType::value_type&  thePair) const;

    private:

        MemoryManager&  m_memoryManager;
    };

protected:

    /**
     * Find an external function.
     *
     * @param theNamespace The namespace for the function.
     * @param functionName The name of the function.
     * @return a pointer to the function if found, or 0 if not found.
     */
    const Function*
    findFunction(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName) const;

private:

    // These are not implemented...
    XPathEnvSupportDefault(const XPathEnvSupportDefault&);

    XPathEnvSupportDefault&
    operator=(const XPathEnvSupportDefault&);

    bool
    operator==(const XPathEnvSupportDefault&) const;

    /**
     * Update the supplied function table.  If the parameter
     * function is 0, and a function with the supplied
     * namespace and name exists in the table, it will be
     * removed.  If function is not 0, and a function with
     * the supplied namespace and name exists in the table,
     * it will be replaced with the new function.  Otherwise,
     * the function will be added.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     * @param function The function to install.
     */
    static void
    updateFunctionTable(
            NamespaceFunctionTablesType&    theTable,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            const Function*                 function);

    /**
     * Find an external function in the supplied table.
     *
     * @param theTable The table to search.
     * @param theNamespace The namespace for the function.
     * @param functionName The name of the function.
     * @return a pointer to the function if found, or 0 if not found.
     */
    const Function*
    findFunction(
            const NamespaceFunctionTablesType&  theTable,
            const XalanDOMString&               theNamespace,
            const XalanDOMString&               functionName) const;

    // Data members...

    SourceDocsTableType                     m_sourceDocs;

    NamespaceFunctionTablesType             m_externalFunctions;

    MemoryManager&                          m_memoryManager;

    PrintWriter*                            m_pw;

    static NamespaceFunctionTablesType      s_externalFunctions;

    static const XalanDOMString             s_emptyString;
};

XALAN_CPP_NAMESPACE_END



#endif  // XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
