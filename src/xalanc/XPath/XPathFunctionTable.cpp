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
// Base class header file
#include "XPathFunctionTable.hpp"



#include <cstring>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "FunctionConcat.hpp"
#include "FunctionContains.hpp"
#include "FunctionID.hpp"
#include "FunctionLang.hpp"
#include "FunctionString.hpp"
#include "FunctionNamespaceURI.hpp"
#include "FunctionNormalizeSpace.hpp"
#include "FunctionStartsWith.hpp"
#include "FunctionSubstring.hpp"
#include "FunctionSubstringAfter.hpp"
#include "FunctionSubstringBefore.hpp"
#include "FunctionTranslate.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class FunctionNotImplemented : public Function
{
public:

    FunctionNotImplemented(const XalanDOMChar*  theName) :
        m_name(theName)
    {
    }

    /**
     * Create a copy of the function object.
     *
     * @return pointer to the new object
     */
    virtual Function*
    clone(MemoryManager& theManager) const
    {
        typedef FunctionNotImplemented  ThisType;

        XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

        ThisType* const     theResult =
            new (theGuard.get()) ThisType(m_name);

        theGuard.release();

        return theResult;
    }

protected:

    /**
     * Get the error message to report when
     * the function is called with the wrong
     * number of arguments.
     *
     * @return function error message
     */
    virtual const XalanDOMString&
    getError(XalanDOMString& theResult) const
    {
        return XalanMessageLoader::getMessage(
                    theResult,
                    XalanMessages::FunctionIsNotImplemented_1Param,
                    m_name);
    }

private:

    // Not implemented...
    Function&
    operator=(const Function&);

    bool
    operator==(const Function&) const;

    const XalanDOMChar* const   m_name;
};



XPathFunctionTable::XPathFunctionTable( bool fCreateTable) :
    m_memoryManager(0),
    m_functionTable(),
    m_functionTableEnd(m_functionTable + (sizeof(m_functionTable) / sizeof(m_functionTable[0])) - 1)
{
    assert(int(s_functionNamesSize) == TableSize);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    std::memset(m_functionTable, 0, sizeof(m_functionTable));
#else
    memset(m_functionTable, 0, sizeof(m_functionTable));
#endif

    if (fCreateTable == true)
    {
        CreateTable();
    }
}



XPathFunctionTable::~XPathFunctionTable()
{
    DestroyTable();
}



void
XPathFunctionTable::InstallFunction(
            const XalanDOMChar*     theFunctionName,
            const Function&         theFunction)
{
    const int   theFunctionID =
            getFunctionIndex(theFunctionName);

    assert (m_memoryManager != 0);

    if (theFunctionID == InvalidFunctionNumberID)
    {
        XalanDOMString theResult(*m_memoryManager);

        throw XPathExceptionFunctionNotSupported(
                theFunctionName,
                theResult,
                0);
    }
    else
    {
        if (m_functionTable[theFunctionID] == 0)
        {
            m_functionTable[theFunctionID] = theFunction.clone(*m_memoryManager);
        }
        else
        {
            const Function* const   theOldFunction = m_functionTable[theFunctionID];

            m_functionTable[theFunctionID] = theFunction.clone(*m_memoryManager);

            const_cast<Function*>(theOldFunction)->~Function();

            m_memoryManager->deallocate((void*)theOldFunction);
        }
    }
}



bool
XPathFunctionTable::UninstallFunction(const XalanDOMChar*   theFunctionName)
{
    const int   theFunctionID =
            getFunctionIndex(theFunctionName);

    if (theFunctionID == InvalidFunctionNumberID)
    {
        return false;
    }
    else
    {
        Function* const theFunction =
            const_cast<Function*>(m_functionTable[theFunctionID]);

        if (theFunction != 0)
        {
            m_functionTable[theFunctionID] = 0;

            XalanDestroy(
                *m_memoryManager,
                *theFunction);
        }

        return true;
    }
}



#if 0
#include <fstream>

void
dumpTable(
            const XPathFunctionTable::FunctionNameIndexMapType&     theTable,
            std::ostream&                                           theSourceStream,
            std::ostream&                                           theHeaderStream)
{
    XPathFunctionTable::FunctionNameIndexMapType::const_iterator    i = theTable.begin();

    while(i != theTable.end())
    {
        theSourceStream << "const XalanDOMChar\tXPathFunctionTable::s_";

        const XalanDOMString&   theString = (*i).first;

        theHeaderStream << "\t// The string \"" << theString << "\"\n\tstatic const XalanDOMChar\ts_";

        bool    nextCap = false;

        XalanDOMString::const_iterator  j = theString.begin();

        while(*j)
        {
            if (*j == '-')
            {
                nextCap = true;
            }
            else
            {
                assert(*j >= 'a' && *j <= 'z');

                if (nextCap)
                {
                    theSourceStream << char(*j -'a' + 'A');
                    theHeaderStream << char(*j -'a' + 'A');

                    nextCap = false;
                }
                else
                {
                    theSourceStream << char(*j);
                    theHeaderStream << char(*j);
                }
            }

            ++j;
        }

        j = theString.begin();

        theSourceStream << "[] =\n{\n";
        theHeaderStream << "[];\n\n";

        while(*j)
        {
            if (*j == '-')
            {
                theSourceStream << "\tXalanUnicode::charHyphenMinus,\n";
            }
            else
            {
                assert(*j >= 'a' && *j <= 'z');

                theSourceStream << "\tXalanUnicode::charLetter_";

                theSourceStream << char(*j) << ",\n";
            }

            ++j;
        }

        theSourceStream << "\t0\n};\n\n";

        ++i;
    }
}
#endif



void
XPathFunctionTable::CreateTable()
{
    try
    {
        InstallFunction(
                s_id,
                FunctionID());

        InstallFunction(
                s_key,
                FunctionNotImplemented(s_key));

        InstallFunction(
                s_not,
                FunctionNotImplemented(s_not));

        InstallFunction(
                s_sum,
                FunctionNotImplemented(s_sum));

        InstallFunction(
                s_lang,
                FunctionLang());

        InstallFunction(
                s_last,
                FunctionNotImplemented(s_last));

        InstallFunction(
                s_name,
                FunctionNotImplemented(s_name));

        InstallFunction(
                s_true,
                FunctionNotImplemented(s_true));

        InstallFunction(
                s_count,
                FunctionNotImplemented(s_count));

        InstallFunction(
                s_false,
                FunctionNotImplemented(s_false));

        InstallFunction(
                s_floor,
                FunctionNotImplemented(s_floor));

        InstallFunction(
                s_round,
                FunctionNotImplemented(s_round));

        InstallFunction(
                s_concat,
                FunctionConcat());

        InstallFunction(
                s_number,
                FunctionNotImplemented(s_number));

        InstallFunction(
                s_string,
                FunctionString());

        InstallFunction(
                s_boolean,
                FunctionNotImplemented(s_boolean));

        InstallFunction(
                s_ceiling,
                FunctionNotImplemented(s_ceiling));

        InstallFunction(
                s_current,
                FunctionNotImplemented(s_current));

        InstallFunction(
                s_contains,
                FunctionContains());

        InstallFunction(
                s_document,
                FunctionNotImplemented(s_document));

        InstallFunction(
                s_position,
                FunctionNotImplemented(s_position));

        InstallFunction(
                s_substring,
                FunctionSubstring());

        InstallFunction(
                s_translate,
                FunctionTranslate());

        InstallFunction(
                s_localName,
                FunctionNotImplemented(s_localName));

        InstallFunction(
                s_generateId,
                FunctionNotImplemented(s_generateId));

        InstallFunction(
                s_startsWith,
                FunctionStartsWith());

        InstallFunction(
                s_formatNumber,
                FunctionNotImplemented(s_formatNumber));

        InstallFunction(
                s_namespaceUri,
                FunctionNamespaceURI());

        InstallFunction(
                s_stringLength,
                FunctionNotImplemented(s_stringLength));

        InstallFunction(
                s_normalizeSpace,
                FunctionNormalizeSpace());

        InstallFunction(
                s_substringAfter,
                FunctionSubstringAfter());

        InstallFunction(
                s_systemProperty,
                FunctionNotImplemented(s_systemProperty));

        InstallFunction(
                s_substringBefore,
                FunctionSubstringBefore());

        InstallFunction(
                s_elementAvailable,
                FunctionNotImplemented(s_elementAvailable));

        InstallFunction(
                s_functionAvailable,
                FunctionNotImplemented(s_functionAvailable));

        InstallFunction(
                s_unparsedEntityUri,
                FunctionNotImplemented(s_unparsedEntityUri));

#if 0
        std::ofstream   theSourceStream("\\foo.cpp");
        std::ofstream   theHeaderStream("\\foo.hpp");

        dumpTable(m_FunctionNameIndex, theSourceStream, theHeaderStream);
#endif
    }
    catch(...)
    {
        DestroyTable();

        throw;
    }
}



void
XPathFunctionTable::DestroyTable()
{
    try
    {
        XALAN_USING_STD(for_each)

        for_each(
            m_functionTable,
            m_functionTable + TableSize,
            DeleteFunctorType(*m_memoryManager));

#if defined(XALAN_STRICT_ANSI_HEADERS)
        std::memset(m_functionTable, 0, sizeof(m_functionTable));
#else
        memset(m_functionTable, 0, sizeof(m_functionTable));
#endif
    }
    catch(...)
    {
    }
}



int
XPathFunctionTable::getFunctionIndex(
            const XalanDOMChar*     theName,
            StringSizeType          theNameLength)
{
    assert(theName != 0);

    // Do a binary search...
    const FunctionNameTableEntry*   theFirst = s_functionNames;
    const FunctionNameTableEntry*   theLast = s_lastFunctionName;

    while(theFirst <= theLast)
    {
        const FunctionNameTableEntry* const     theCurrent =
            theFirst + (theLast - theFirst) / 2;
        assert(theCurrent->m_size == length(theCurrent->m_name));

        const int   theResult = compare(
                theName,
                theNameLength,
                theCurrent->m_name,
                theCurrent->m_size);

        if (theResult < 0)
        {
            theLast = theCurrent - 1;
        }
        else if (theResult > 0)
        {
            theFirst = theCurrent + 1;
        }
        else
        {
            assert(int(theCurrent - s_functionNames) == theCurrent - s_functionNames);

            return int(theCurrent - s_functionNames);
        }
    }

    return InvalidFunctionNumberID;
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(
            const XalanDOMString&   theFunctionNumber,
            XalanDOMString&         theResult,
            const Locator*          theLocator) :
    XalanXPathException(
        XalanMessageLoader::getMessage(
            theResult,
            XalanMessages::FunctionNumberIsNotAvailable_1Param,
            theFunctionNumber),
        theResult.getMemoryManager(),
        theLocator)
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(const XPathExceptionFunctionNotAvailable& other) :
    XalanXPathException(other)
{
}



XPathExceptionFunctionNotAvailable::~XPathExceptionFunctionNotAvailable()
{
}




XPathExceptionFunctionNotSupported::XPathExceptionFunctionNotSupported(
            const XalanDOMChar*     theFunctionName,
            XalanDOMString&         theResult,
            const Locator*          theLocator) :
    XalanXPathException(
        XalanMessageLoader::getMessage(
            theResult,
            XalanMessages::FunctionIsNotSupported_1Param,
            theFunctionName),
        theResult.getMemoryManager(),
        theLocator)
{
}



XPathExceptionFunctionNotSupported::XPathExceptionFunctionNotSupported(const XPathExceptionFunctionNotSupported& other) :
    XalanXPathException(other)
{
}



XPathExceptionFunctionNotSupported::~XPathExceptionFunctionNotSupported()
{
}




const XalanDOMChar  XPathFunctionTable::s_id[] =
{
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_d,
    0
};

const XalanDOMChar  XPathFunctionTable::s_key[] =
{
    XalanUnicode::charLetter_k,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_y,
    0
};

const XalanDOMChar  XPathFunctionTable::s_not[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPathFunctionTable::s_sum[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_m,
    0
};

const XalanDOMChar  XPathFunctionTable::s_lang[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};

const XalanDOMChar  XPathFunctionTable::s_last[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPathFunctionTable::s_name[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_true[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_count[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPathFunctionTable::s_false[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_floor[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    0
};

const XalanDOMChar  XPathFunctionTable::s_round[] =
{
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_d,
    0
};

const XalanDOMChar  XPathFunctionTable::s_concat[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPathFunctionTable::s_number[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};

const XalanDOMChar  XPathFunctionTable::s_string[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};

const XalanDOMChar  XPathFunctionTable::s_boolean[] =
{
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};

const XalanDOMChar  XPathFunctionTable::s_ceiling[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};

const XalanDOMChar  XPathFunctionTable::s_current[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPathFunctionTable::s_contains[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_s,
    0
};

const XalanDOMChar  XPathFunctionTable::s_document[] =
{
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPathFunctionTable::s_position[] =
{
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};

const XalanDOMChar  XPathFunctionTable::s_substring[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};

const XalanDOMChar  XPathFunctionTable::s_translate[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_localName[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_generateId[] =
{
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_d,
    0
};

const XalanDOMChar  XPathFunctionTable::s_startsWith[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_s,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_w,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_h,
    0
};

const XalanDOMChar  XPathFunctionTable::s_formatNumber[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};

const XalanDOMChar  XPathFunctionTable::s_namespaceUri[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    0
};

const XalanDOMChar  XPathFunctionTable::s_stringLength[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_h,
    0
};

const XalanDOMChar  XPathFunctionTable::s_normalizeSpace[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_z,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_substringAfter[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};

const XalanDOMChar  XPathFunctionTable::s_systemProperty[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_y,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_m,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_y,
    0
};

const XalanDOMChar  XPathFunctionTable::s_substringBefore[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_elementAvailable[] =
{
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_functionAvailable[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    0
};

const XalanDOMChar  XPathFunctionTable::s_unparsedEntityUri[] =
{
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_d,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_y,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    0
};


typedef XPathFunctionTable::SizeType                SizeType;
typedef XPathFunctionTable::FunctionNameTableEntry  FunctionNameTableEntry;

#define XFTBL_SIZE(str) ((sizeof(str) / sizeof(str[0]) - 1))


const FunctionNameTableEntry    XPathFunctionTable::s_functionNames[] =
{
    {
        s_id,
        XFTBL_SIZE(s_id)
    },
    {
        s_key,
        XFTBL_SIZE(s_key)
    },
    {
        s_not,
        XFTBL_SIZE(s_not)
    },
    {
        s_sum,
        XFTBL_SIZE(s_sum)
    },
    {
        s_lang,
        XFTBL_SIZE(s_lang)
    },
    {
        s_last,
        XFTBL_SIZE(s_last)
    },
    {
        s_name,
        XFTBL_SIZE(s_name)
    },
    {
        s_true,
        XFTBL_SIZE(s_true)
    },
    {
        s_count,
        XFTBL_SIZE(s_count)
    },
    {
        s_false,
        XFTBL_SIZE(s_false)
    },
    {
        s_floor,
        XFTBL_SIZE(s_floor)
    },
    {
        s_round,
        XFTBL_SIZE(s_round)
    },
    {
        s_concat,
        XFTBL_SIZE(s_concat)
    },
    {
        s_number,
        XFTBL_SIZE(s_number)
    },
    {
        s_string,
        XFTBL_SIZE(s_string)
    },
    {
        s_boolean,
        XFTBL_SIZE(s_boolean)
    },
    {
        s_ceiling,
        XFTBL_SIZE(s_ceiling)
    },
    {
        s_current,
        XFTBL_SIZE(s_current)
    },
    {
        s_contains,
        XFTBL_SIZE(s_contains)
    },
    {
        s_document,
        XFTBL_SIZE(s_document)
    },
    {
        s_position,
        XFTBL_SIZE(s_position)
    },
    {
        s_substring,
        XFTBL_SIZE(s_substring)
    },
    {
        s_translate,
        XFTBL_SIZE(s_translate)
    },
    {
        s_localName,
        XFTBL_SIZE(s_localName)
    },
    {
        s_generateId,
        XFTBL_SIZE(s_generateId)
    },
    {
        s_startsWith,
        XFTBL_SIZE(s_startsWith)
    },
    {
        s_formatNumber,
        XFTBL_SIZE(s_formatNumber)
    },
    {
        s_namespaceUri,
        XFTBL_SIZE(s_namespaceUri)
    },
    {
        s_stringLength,
        XFTBL_SIZE(s_stringLength)
    },
    {
        s_normalizeSpace,
        XFTBL_SIZE(s_normalizeSpace)
    },
    {
        s_substringAfter,
        XFTBL_SIZE(s_substringAfter)
    },
    {
        s_systemProperty,
        XFTBL_SIZE(s_systemProperty)
    },
    {
        s_substringBefore,
        XFTBL_SIZE(s_substringBefore)
    },
    {
        s_elementAvailable,
        XFTBL_SIZE(s_elementAvailable)
    },
    {
        s_functionAvailable,
        XFTBL_SIZE(s_functionAvailable)
    },
    {
        s_unparsedEntityUri,
        XFTBL_SIZE(s_unparsedEntityUri)
    }
};


const FunctionNameTableEntry* const     XPathFunctionTable::s_lastFunctionName =
    &s_functionNames[sizeof(s_functionNames) / sizeof(s_functionNames[0]) - 1];


const SizeType      XPathFunctionTable::s_functionNamesSize =
    sizeof(s_functionNames) / sizeof(s_functionNames[0]);



XALAN_CPP_NAMESPACE_END
