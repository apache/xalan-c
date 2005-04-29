/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#if !defined(XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680)
#define XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <algorithm>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XalanXPathException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Exception class thrown when an unknown function is encountered
 */
class XALAN_XPATH_EXPORT XPathExceptionFunctionNotAvailable : public XalanXPathException
{
public:

    typedef Function::LocatorType   LocatorType;

    XPathExceptionFunctionNotAvailable(const XalanDOMString&    theFunctionNumber,
                                        XalanDOMString&         theResult);

    XPathExceptionFunctionNotAvailable(
        const XalanDOMString&   theFunctionNumber,
        const LocatorType&      theLocator,
        XalanDOMString&         theResult);

    ~XPathExceptionFunctionNotAvailable();
};



/**
 * Exception class thrown when an installFunction() is called with a 
 * function name that is not supported.
 */
class XALAN_XPATH_EXPORT XPathExceptionFunctionNotSupported : public XalanXPathException
{
public:

    XPathExceptionFunctionNotSupported(const XalanDOMChar*  theFunctionName,
                                        XalanDOMString&     theResult);

    ~XPathExceptionFunctionNotSupported();
};



/**
 * Class defines a table of functions that can be called in XPath expresions.
 */
class XALAN_XPATH_EXPORT XPathFunctionTable
{
public:

    enum { InvalidFunctionNumberID = -1, TableSize = 36 };

    typedef size_t                      SizeType;
    typedef XalanDOMString::size_type   StringSizeType;
    typedef DeleteFunctor<Function>     DeleteFunctorType;

    /**
     * Constructor.
     *
     * @param fCreateTable If true, the internal table will be created.  Otherwise, CreateTable() must be called.
     */
    XPathFunctionTable(bool     fCreateTable = true);

    ~XPathFunctionTable();

    void
    setMemoryManager(MemoryManagerType& theManager)
    {
        m_memoryManager = &theManager;
    }
    /**
     * Set up the internal table.
     */
    void
    CreateTable();

    /**
     * Destroy the internal table.
     */
    void
    DestroyTable();

    /**
     * Retrieve the function object for a specified function name.
     * 
     * @param theFunctionName name of function
     * @return function named
     */
    const Function&
    operator[](const XalanDOMString&    theFunctionName) const
    {
        const int   theFunctionID =
            getFunctionIndex(theFunctionName);

        if (theFunctionID != InvalidFunctionNumberID)
        {
            return *m_functionTable[theFunctionID];
        }
        else
        {
            MemoryManagerType* theManager = const_cast<MemoryManagerType*>(m_memoryManager);

            XalanDOMString   theResult(*theManager);

            throw XPathExceptionFunctionNotAvailable(theFunctionName, theResult);
        }
    }

    /**
     * Retrieve the function object for a specified function ID number.
     * 
     * @param theFunctionID ID number of the function
     * @return function named
     */
    const Function&
    operator[](int  theFunctionID) const
    {
        assert(theFunctionID >= 0 && theFunctionID < TableSize);
        assert(m_functionTable[theFunctionID] != 0);

        return *m_functionTable[theFunctionID];
    }

    /**
     * Map a function ID to the corresponding name.
     * 
     * @param theFunctionID The ID number of the function
     * @return The name of the function, or an empty string if the function doesn't exist.
     */
    const XalanDOMString&
    idToName(int    theFunctionID,
                XalanDOMString& theResult) const
    {

        if (theFunctionID >= 0 && theFunctionID < TableSize)
        {
            theResult.assign(
                s_functionNames[theFunctionID].m_name,
                s_functionNames[theFunctionID].m_size);
        }

        return theResult;
    }

    /**
     * Map a function name to the corresponding ID number.
     * 
     * @param theName name of function
     * @return The ID number of function, or InvalidFunctionNumberID if the function doesn't exist.
     */
    int
    nameToID(const XalanDOMString&  theName) const
    {
        return getFunctionIndex(theName);
    }

    /**
     * Insert a named function into the function table.
     * 
     * @param theFunctionName name of function
     * @param theFunction     function object corresponding to name
     */
    void
    InstallFunction(
            const XalanDOMString&   theFunctionName,
            const Function&         theFunction)
    {
        InstallFunction(theFunctionName.c_str(), theFunction);
    }

    /**
     * Remove a named function from the function table.
     * 
     * @param theFunctionName name of function
     * @return true if the function was found and removed.
     */
    bool
    UninstallFunction(const XalanDOMString&     theFunctionName)
    {
        return UninstallFunction(theFunctionName.c_str());
    }

    /**
     * Insert a named function into the function table.
     * 
     * @param theFunctionName name of function
     * @param theFunction     function object corresponding to name
     */
    void
    InstallFunction(
            const XalanDOMChar*     theFunctionName,
            const Function&         theFunction);

    /**
     * Remove a named function from the function table.
     * 
     * @param theFunctionName name of function
     * @return true if the function was found and removed.
     */
    bool
    UninstallFunction(const XalanDOMChar*   theFunctionName);

    /**
     * Whether a named function is in the function table.
     * 
     * @param theFunctionName name of function
     * @return true if function is in table
     */
    bool
    isInstalledFunction(const XalanDOMString&   theFunctionName) const
    {
        return getFunctionIndex(theFunctionName) != InvalidFunctionNumberID ? true : false;
    }

#if defined(XALAN_NO_MEMBER_TEMPLATES)

    typedef XalanVector<XalanDOMString>         InstalledFunctionNameVectorType;

    /**
     * Add a list of the names of installed functions to a vector of names.
     * 
     * @param theVector vector of function name strings added to
     */
    void
    getInstalledFunctionNames(InstalledFunctionNameVectorType&  theVector) const
    {
        XalanDOMString  theString;

        for (int i = 0; i < TableSize; ++i)
        {
            if (m_functionTable[i] != 0)
            {
                theString.assign(
                    s_functionNames[i].m_name,
                    s_functionNames[i].m_size);

                theVector.push_back(theString);
            }
        }
    }
#else

    /**
     * Add a list of the names of installed functions to a vector of names.
     * 
     * @param theIterator function table iterator to append names to
     */
    template<class OutputIteratorType>
    void
    getInstalledFunctionNames(OutputIteratorType    theIterator) const
    {
        XalanDOMString  theString(XalanMemMgrs::getDefaultXercesMemMgr());

        for (int i = 0; i < TableSize; ++i)
        {
            if (m_functionTable[i] != 0)
            {
                theString.assign(
                    s_functionNames[i].m_name,
                    s_functionNames[i].m_size);

                *theIterator = theString;

                ++theIterator;
            }
        }
    }
#endif

    struct FunctionNameTableEntry
    {
        const XalanDOMChar*     m_name;

        StringSizeType          m_size;
    };

    // These are static strings for the functions supported.
    // Note that the XSLT functions are also here, since it's
    // just easier to do it this way.

    // The string "id"
    static const XalanDOMChar   s_id[];

    // The string "key"
    static const XalanDOMChar   s_key[];

    // The string "not"
    static const XalanDOMChar   s_not[];

    // The string "sum"
    static const XalanDOMChar   s_sum[];

    // The string "lang"
    static const XalanDOMChar   s_lang[];

    // The string "last"
    static const XalanDOMChar   s_last[];

    // The string "name"
    static const XalanDOMChar   s_name[];

    // The string "true"
    static const XalanDOMChar   s_true[];

    // The string "count"
    static const XalanDOMChar   s_count[];

    // The string "false"
    static const XalanDOMChar   s_false[];

    // The string "floor"
    static const XalanDOMChar   s_floor[];

    // The string "round"
    static const XalanDOMChar   s_round[];

    // The string "concat"
    static const XalanDOMChar   s_concat[];

    // The string "number"
    static const XalanDOMChar   s_number[];

    // The string "string"
    static const XalanDOMChar   s_string[];

    // The string "boolean"
    static const XalanDOMChar   s_boolean[];

    // The string "ceiling"
    static const XalanDOMChar   s_ceiling[];

    // The string "current"
    static const XalanDOMChar   s_current[];

    // The string "contains"
    static const XalanDOMChar   s_contains[];

    // The string "document"
    static const XalanDOMChar   s_document[];

    // The string "position"
    static const XalanDOMChar   s_position[];

    // The string "substring"
    static const XalanDOMChar   s_substring[];

    // The string "translate"
    static const XalanDOMChar   s_translate[];

    // The string "local-name"
    static const XalanDOMChar   s_localName[];

    // The string "generate-id"
    static const XalanDOMChar   s_generateId[];

    // The string "starts-with"
    static const XalanDOMChar   s_startsWith[];

    // The string "format-number"
    static const XalanDOMChar   s_formatNumber[];

    // The string "namespace-uri"
    static const XalanDOMChar   s_namespaceUri[];

    // The string "string-length"
    static const XalanDOMChar   s_stringLength[];

    // The string "normalize-space"
    static const XalanDOMChar   s_normalizeSpace[];

    // The string "substring-after"
    static const XalanDOMChar   s_substringAfter[];

    // The string "system-property"
    static const XalanDOMChar   s_systemProperty[];

    // The string "substring-before"
    static const XalanDOMChar   s_substringBefore[];

    // The string "element-available"
    static const XalanDOMChar   s_elementAvailable[];

    // The string "function-available"
    static const XalanDOMChar   s_functionAvailable[];

    // The string "unparsed-entity-uri"
    static const XalanDOMChar   s_unparsedEntityUri[];

    // A table of function names.
    static const FunctionNameTableEntry         s_functionNames[];

    // The size of the table.
    static const SizeType                       s_functionNamesSize;

private:

    static int
    getFunctionIndex(const XalanDOMString&  theName)
    {
        return getFunctionIndex(
                theName.c_str(),
                theName.length());
    }

    static int
    getFunctionIndex(const XalanDOMChar*    theName)
    {
        return getFunctionIndex(
                theName,
                XalanDOMString::length(theName));
    }

    static int
    getFunctionIndex(
            const XalanDOMChar*     theName,
            StringSizeType          theNameLength);

    MemoryManagerType*          m_memoryManager;

    const Function*             m_functionTable[TableSize];

    const Function** const      m_functionTableEnd;

    // The last one in the table of function names.
    static const FunctionNameTableEntry* const  s_lastFunctionName;
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680
