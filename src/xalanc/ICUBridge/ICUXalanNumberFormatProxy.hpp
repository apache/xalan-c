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
#if !defined(ICUXALANNUMBERFORMATPROXY_HEADER_GUARD_1357924680)
#define ICUXALANNUMBERFORMATPROXY_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/PlatformSupport/XalanNumberFormat.hpp>



#include <unicode/decimfmt.h>



XALAN_CPP_NAMESPACE_BEGIN



// Class that implements the XSLT function format-number using the ICU.
//
class XALAN_ICUBRIDGE_EXPORT ICUXalanNumberFormatProxy : public XalanNumberFormat
{
public:

    explicit
    ICUXalanNumberFormatProxy(MemoryManager& theManager);

    virtual
    ~ICUXalanNumberFormatProxy();

    virtual XalanDOMString&
    format(
            double              theValue,
            XalanDOMString&     theResult);


    virtual XalanDOMString&
    format(
            int                 theValue,
            XalanDOMString&     theResult);


    virtual XalanDOMString&
    format(
            unsigned int        theValue,
            XalanDOMString&     theResult);


    virtual XalanDOMString&
    format(
            long                theValue,
            XalanDOMString&     theResult);


    virtual XalanDOMString&
    format(
            unsigned long       theValue,
            XalanDOMString&     theResult);

    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @param theResult the string result
     */
    virtual XalanDOMString&
    format(
            XALAN_INT64         theValue,
            XalanDOMString&     theResult);

    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @param theResult the string result
     */
    virtual XalanDOMString&
    format(
            XALAN_UINT64        theValue,
            XalanDOMString&     theResult);

    virtual bool
    isGroupingUsed() const;

    virtual void
    setGroupingUsed(bool bUsed);

    virtual void
    setGroupingSize(unsigned long   size);

    virtual void
    setGroupingSeparator(const XalanDOMString&  s);

private:

#if defined(XALAN_HAS_CPP_NAMESPACE)
    typedef U_ICU_NAMESPACE::DecimalFormat      DecimalFormatType;
#else
    typedef DecimalFormat*                      DecimalFormatType;
#endif

    DecimalFormatType*      m_decimalFormat;

};



XALAN_CPP_NAMESPACE_END



#endif  // ICUXALANNUMBERFORMATPROXY_HEADER_GUARD_1357924680
