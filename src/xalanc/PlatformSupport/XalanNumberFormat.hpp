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
#if !defined(XALANNUMBERFORMAT_HEADER_GUARD_1357924680)
#define XALANNUMBERFORMAT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanNumberFormat
{
public:

    explicit
    XalanNumberFormat(MemoryManager&      theManager);

    static XalanNumberFormat*
    create(MemoryManager&      theManager)
    {
        XalanNumberFormat*  theInstance;

        return XalanConstruct(
                    theManager,
                    theInstance,
                    theManager);
    }

    virtual
    ~XalanNumberFormat();

    MemoryManager&
    getMemoryManager()
    {
        return m_groupingSeparator.getMemoryManager();
    }

    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @return string representation of number
     */
    virtual XalanDOMString&
    format(double   theValue,
            XalanDOMString&     theResult);



    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @return string representation of number
     */
    virtual XalanDOMString&
    format(int  theValue,
            XalanDOMString&     theResult);



    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @param theResult the string result
     */
    virtual XalanDOMString&
    format(
            unsigned int        theValue,
            XalanDOMString&     theResult);

    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @return string representation of number
     */
    virtual XalanDOMString&
    format(long     theValue,
            XalanDOMString&     theResult);


    /**
     * Format a number into a string.
     *
     * @param theValue number to format
     * @param theResult the string result
     */
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

    /**
     * Whether groupings are used for numbers, for example, "234,678"
     *
     * @return true if grouping used
     */
    virtual bool
    isGroupingUsed() const;

    /**
     * Change whether groupings are used for numbers, for example, "234,678"
     *
     * @param bUsed true to use grouping
     */
    virtual void
    setGroupingUsed(bool bUsed);

    /**
     * Change the size of groupings, for example, "234,678" uses a size of "3"
     *
     * @param size the grouping size
     */
    virtual void
    setGroupingSize(unsigned long   size);

    /**
     * Change the separator string used for groupings, for example, "234,678"
     * uses the separator ","
     *
     * @param s grouping separator string
     */
    virtual void
    setGroupingSeparator(const XalanDOMString&  s);

protected:

    void
    applyGrouping(
            const XalanDOMString&       value,
            XalanDOMString&             result);

private:

    // Not implemented...
    XalanNumberFormat(const XalanNumberFormat&);

    XalanNumberFormat&
    operator=(const XalanNumberFormat&);

    bool
    operator==(const XalanNumberFormat&);

    // Data members...
    bool                        m_isGroupingUsed;

    XalanDOMString              m_groupingSeparator;

    unsigned long               m_groupingSize;

    static const XalanDOMChar   s_defaultGroupingSeparator[];
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANNUMBERFORMAT_HEADER_GUARD_1357924680
