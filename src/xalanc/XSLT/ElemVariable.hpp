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
#if !defined(XALAN_ELEMVARIABLE_HEADER_GUARD)
#define XALAN_ELEMVARIABLE_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"

// Base class header file.
#include "ElemTemplateElement.hpp"



#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XSLT/Constants.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPath;



class ElemVariable : public ElemTemplateElement
{
public:

    typedef ElemTemplateElement     ParentType;

    /**
     * Construct an object corresponding to an "xsl:variable" element
     * 
     * @param constructionContext context for construction of object
     * @param stylesheetTree      stylesheet containing element
     * @param atts                list of attributes for element
     * @param lineNumber                line number in document
     * @param columnNumber          column number in document
     */
    ElemVariable(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber);

    virtual
    ~ElemVariable();

    /**
     * Determines if this is a top level variable.
     * 
     * @return true if it is a top level variable
     */
    bool
    isTopLevel() const
    {
        return m_isTopLevel;
    }

    // These methods are inherited from ElemTemplateElement ...
    
    virtual const XalanQName&
    getNameAttribute() const;

    virtual void
    addToStylesheet(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     theStylesheet);

    virtual const XalanDOMString&
    getElementName() const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    const ElemTemplateElement*
    startElement(StylesheetExecutionContext&    executionContext) const;

    void
    endElement(StylesheetExecutionContext&      executionContext) const;
#else
    virtual void
    execute(StylesheetExecutionContext&     executionContext) const;
#endif

    const XObjectPtr
    getValue(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      sourceNode) const;

    virtual void
    setParentNodeElem(ElemTemplateElement*      theParent);

    virtual const XPath*
    getXPath(XalanSize_t    index) const;

protected:

    /**
     * Construct an object corresponding to an "xsl:variable" element
     * 
     * @param constructionContext context for construction of object
     * @param stylesheetTree      stylesheet containing element
     * @param atts                list of attributes for element
     * @param lineNumber                line number in document
     * @param columnNumber          column number in document
     */
    ElemVariable(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber,
            int                             xslToken);

    /**
     * Do common initialization.
     * 
     * @param constructionContext context for construction of object
     * @param stylesheetTree      stylesheet containing element
     * @param atts                list of attributes for element
     */
    void
    init(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts);


    const XalanQName*   m_qname;

private:

    // not implemented
    ElemVariable(const ElemVariable &);

    ElemVariable&
    operator=(const ElemVariable &);

    const XPath*    m_selectPattern;

    bool            m_isTopLevel;

    XObjectPtr      m_value;

    XalanNode*      m_varContext;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_ELEMVARIABLE_HEADER_GUARD
