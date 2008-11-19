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
#if !defined(STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>
#include <xalanc/PlatformSupport/XalanArrayAllocator.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringPool.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION) || defined(XALAN_BUILD_DLL)
#include <xalanc/XPath/XPathProcessor.hpp>
#endif



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>



#include <xalanc/PlatformSupport/XalanDOMStringCache.hpp>



#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XalanQNameByValue.hpp>
#include <xalanc/XPath/XalanQNameByValueAllocator.hpp>



// Base class header file...
#include <xalanc/XSLT/StylesheetConstructionContext.hpp>



#include <xalanc/XSLT/AVT.hpp>
#include <xalanc/XSLT/XalanAVTAllocator.hpp>
#include <xalanc/XSLT/AVTPartSimple.hpp>
#include <xalanc/XSLT/XalanAVTPartSimpleAllocator.hpp>
#include <xalanc/XSLT/AVTPartXPath.hpp>
#include <xalanc/XSLT/XalanAVTPartXPathAllocator.hpp>
#include <xalanc/XSLT/XalanElemApplyTemplatesAllocator.hpp>
#include <xalanc/XSLT/XalanElemAttributeAllocator.hpp>
#include <xalanc/XSLT/XalanElemAttributeSetAllocator.hpp>
#include <xalanc/XSLT/XalanElemCallTemplateAllocator.hpp>
#include <xalanc/XSLT/XalanElemElementAllocator.hpp>
#include <xalanc/XSLT/XalanElemLiteralResultAllocator.hpp>
#include <xalanc/XSLT/XalanElemTemplateAllocator.hpp>
#include <xalanc/XSLT/XalanElemTextLiteralAllocator.hpp>
#include <xalanc/XSLT/XalanElemValueOfAllocator.hpp>
#include <xalanc/XSLT/XalanElemVariableAllocator.hpp>
#include "xalanc/XSLT/XalanMatchPatternDataAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XPathEnvSupport;
class XPathFactory;
class XPathProcessor;
class XSLTEngineImpl;



/**
 *
 * An default implementation of an abtract class which provides support for
 * constructing the internal representation  of a stylesheet.
 *
 */
class XALAN_XSLT_EXPORT StylesheetConstructionContextDefault : public StylesheetConstructionContext
{
public:

    typedef XalanArrayAllocator<XalanDOMChar>           XalanDOMCharVectorAllocatorType;
    typedef XalanArrayAllocator<const void*>            PointerVectorAllocatorType;
    typedef XalanDOMCharVectorAllocatorType::size_type  VectorAllocatorSizeType;

    // Default size for vector allocation.
    enum {
            eDefaultXalanDOMCharVectorBlockSize = 1024,
            eDefaultAVTBlockSize = 128,
            eDefaultAVTPartSimpleBlockSize = 128,
            eDefaultAVTPartXPathBlockSize = 128,
            eDefaultXalanQNameByValueBlockSize = 32,
            eDefaultPointerVectorBlockSize = 512,
            eDefaultElemApplyTemplatesBlockSize = 10,
            eDefaultElemAttributeBlockSize = 10,
            eDefaultElemAttributeSetBlockSize = 10,
            eDefaultElemCallTemplateBlockSize = 10,
            eDefaultElemElementBlockSize = 10,
            eDefaultElemLiteralResultBlockSize = 20,
            eDefaultElemTemplateBlockSize = 10,
            eDefaultElemTextLiteralBlockSize = 20,
            eDefaultElemValueOfBlockSize = 10,
            eDefaultElemVariableBlockSize = 10,
            eDefaultMatchPatternDataBlockSize = 10 };

    /*
     * Construct an instance.  If the stylesheet(s) constructed is/are meant to be reused (a.k.a. "compiled"),
     * the XObjectFactory and XPathFactory instance must exist for the lifetime of the construction context
     * and, therefore, for the lifetime of the stylesheet(s).  Otherwise, XObject and XPath instance will be
     * destroyed when the corresponding factories are destryed, leaving pointers to destroyed objects in the.
     * stylesheet(s).
     *
     * @param processor a reference to an XSLTEngineImpl instance.  Used for error reporting.
     * @param xpathFactory a reference to an XPathFactory instance.  See comments above for important details.
     * @param theXalanDOMCharVectorAllocatorBlockSize The block size to use for allocating vectors of XalanDOMChars
     * @param theAVTAllocatorBlockSize The block size to use for allocating AVT instances.
     * @param theAVTPartSimpleAllocatorBlockSize The block size to use for allocating AVTPartSimple instances.
     * @param theAVTPartXPathAllocatorBlockSize The block size to use for allocating AVTPartXPath instances.
     * @param theXalanQNameByValueAllocatorBlockSize The block size to use for allocating XalanQNameByValue instances.
     * @param thePointerVectorAllocatorBlockSize The block size to use for allocating vectors of pointers.
     */
    StylesheetConstructionContextDefault(
            MemoryManager&                      theManager,
            XSLTEngineImpl&                         processor,
            XPathFactory&                           xpathFactory,
            VectorAllocatorSizeType                 theXalanDOMCharVectorAllocatorBlockSize = eDefaultXalanDOMCharVectorBlockSize,
            XalanAVTAllocator::size_type            theAVTAllocatorBlockSize = eDefaultAVTBlockSize,
            XalanAVTPartSimpleAllocator::size_type  theAVTPartSimpleAllocatorBlockSize = eDefaultAVTPartSimpleBlockSize,
            XalanAVTPartXPathAllocator::size_type   theAVTPartXPathAllocatorBlockSize = eDefaultAVTPartXPathBlockSize,
            XalanQNameByValueAllocator::size_type   theXalanQNameByValueAllocatorBlockSize = eDefaultXalanQNameByValueBlockSize,
            VectorAllocatorSizeType                 thePointerVectorAllocatorBlockSize = eDefaultPointerVectorBlockSize);

    virtual
    ~StylesheetConstructionContextDefault();


    // These interfaces are inherited from StylesheetConstructionContext...

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

    virtual void
    reset();

    virtual const XalanDOMString&
    getPooledString(const XalanDOMString&   theString);

    virtual const XalanDOMString&
    getPooledString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength = XalanDOMString::npos);

    virtual XalanDOMString&
    getCachedString();

    virtual bool
    releaseCachedString(XalanDOMString&     theString);

    virtual StylesheetRoot*
    create(const XalanDOMString&    theBaseIdentifier);

    virtual StylesheetRoot*
    create(const XSLTInputSource&   theInputSource);

    virtual Stylesheet*
    create(
            StylesheetRoot&         theStylesheetRoot,
            const XalanDOMString&   theBaseIdentifier);

    virtual void
    destroy(StylesheetRoot*     theStylesheetRoot);

    virtual URLAutoPtrType
    getURLFromString(const XalanDOMString&  urlString);

    virtual XalanDOMString&
    getURLStringFromString(
            const XalanDOMString&   urlString,
            XalanDOMString&         theResult);

    virtual URLAutoPtrType
    getURLFromString(
            const XalanDOMString&   urlString,
            const XalanDOMString&   base);

    virtual XalanDOMString&
    getURLStringFromString(
            const XalanDOMString&   urlString,
            const XalanDOMString&   base,
            XalanDOMString& theResult);

    virtual const XalanDOMString&
    getXSLTNamespaceURI() const;

    virtual XPath*
    createMatchPattern(
            const Locator*          locator,
            const XalanDOMString&   str,
            const PrefixResolver&   resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true);

    virtual XPath*
    createMatchPattern(
            const Locator*          locator,
            const XalanDOMChar*     str,
            const PrefixResolver&   resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true);

    virtual XPath*
    createXPath(
            const Locator*          locator,
            const XalanDOMString&   str,
            const PrefixResolver&   resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true);

    virtual XPath*
    createXPath(
            const Locator*          locator,
            const XalanDOMChar*     str,
            const PrefixResolver&   resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true);

    virtual XPath*
    createXPath(
            const Locator*              locator,
            const XalanDOMChar*         str,
            XalanDOMString::size_type   len,
            const PrefixResolver&       resolver,
            bool                        allowVariableReferences = true,
            bool                        allowKeyFunction = true);

    virtual const Locator*
    getLocatorFromStack() const;

    virtual void
    pushLocatorOnStack(const Locator*   locator);

    virtual void
    popLocatorStack();

    virtual const XalanDOMString&
    getXalanXSLNameSpaceURL() const;

    virtual XalanDocument*
    parseXML(
            const XalanDOMString&   urlString,
            DocumentHandler*        docHandler, 
            XalanDocument*          docToRegister,
            ErrorHandler*           theErrorHandler = 0);

    virtual bool
    isXMLSpaceAttribute(
            const XalanDOMChar*     theAttributeName,
            const Stylesheet&       theStylesheet,
            const Locator*          theLocator = 0);

    virtual bool
    isXSLUseAttributeSetsAttribute(
            const XalanDOMChar*     theAttributeName,
            const Stylesheet&       theStylesheet,
            const Locator*          theLocator = 0);

    virtual bool
    isValidQName(
            const XalanDOMChar*     theName,
            const Stylesheet&       theStylesheet,
            const Locator*          theLocator = 0);

    virtual eElementToken
    getElementToken(const XalanDOMString&   name) const;

    virtual double
    getXSLTVersionSupported() const;

    virtual XalanDOMChar*
    allocateXalanDOMCharVector(XalanDOMString::size_type    theLength);

    virtual XalanDOMChar*
    allocateXalanDOMCharVector(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength = XalanDOMString::npos,
            bool                        fTerminate = true);

    virtual const AVT*
    createAVT(
            const Locator*          locator,
            const XalanDOMChar*     name,
            const XalanDOMChar*     stringedValue,
            const PrefixResolver&   resolver);

    virtual const AVTPart*
    createAVTPart(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength = XalanDOMString::npos);

    virtual const AVTPart*
    createAVTPart(
            const Locator*              locator,
            const XalanDOMChar*         str,
            XalanDOMString::size_type   len,
            const PrefixResolver&       resolver);

    virtual const AVT**
    allocateAVTPointerVector(size_type  theLength);

    virtual const AVTPart**
    allocateAVTPartPointerVector(size_type  theLength);

    virtual const XalanQName*
    createXalanQName(
            const XalanDOMString&       qname,
            const NamespacesStackType&  namespaces,
            const Locator*              locator = 0,
            bool                        fUseDefault = false);

    virtual const XalanQName*
    createXalanQName(
            const XalanDOMChar*         qname,
            const NamespacesStackType&  namespaces,
            const Locator*              locator = 0,
            bool                        fUseDefault = false);

    virtual const XalanQName**
    tokenizeQNames(
            size_type&                  count,
            const XalanDOMChar*         qnameTokens,
            const NamespacesStackType&  namespaces,
            const Locator*              locator = 0,
            bool                        fUseDefault = false);

    virtual ElemTemplateElement*
    createElement(
            int                         token,
            Stylesheet&                 stylesheetTree,
            const AttributeListType&    atts,
            const Locator*              locator = 0);

    virtual ElemTemplateElement*
    createElement(
            int                         token,
            Stylesheet&                 stylesheetTree,
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            const Locator*              locator = 0);

    virtual ElemTemplateElement*
    createElement(
            Stylesheet&                 stylesheetTree,
            const XalanDOMChar*         chars,
            XalanDOMString::size_type   length,
            bool                        preserveSpace,
            bool                        disableOutputEscaping,
            const Locator*              locator = 0);

    virtual ElemTemplateElement*
    createElement(
            Stylesheet&                 stylesheetTree,
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            ExtensionNSHandler&         handler,
            const Locator*              locator = 0);

    virtual const XalanMatchPatternData*
    createXalanMatchPatternData(
            const ElemTemplate&     theTemplate,
            size_type               thePosition,
            const XalanDOMString&   theTargetString,
            const XPath&            theMatchPattern,
            const XalanDOMString&   thePatternString,
            XPath::eMatchScore      thePriority);

    static eElementToken
    getElementNameToken(const XalanDOMString&   name);

    typedef XalanVector<StylesheetRoot*>            StylesheetVectorType;
    typedef XalanVector<ElemTemplateElement*>   ElemTemplateElementVectorType;

private:

    const AVT**
    doAllocateAVTPointerVector(size_type    theSize)
    {
        assert(sizeof(AVT**) == sizeof(PointerVectorAllocatorType::value_type));

        return reinterpret_cast<const AVT**>(m_pointerVectorAllocator.allocate(theSize));
    }

    const AVTPart**
    doAllocateAVTPartPointerVector(size_type    theSize)
    {
        assert(sizeof(AVTPart**) == sizeof(PointerVectorAllocatorType::value_type));

        return reinterpret_cast<const AVTPart**>(m_pointerVectorAllocator.allocate(theSize));
    }

    const XalanQName**
    doAllocateXalanQNamePointerVector(size_type theSize)
    {
        assert(sizeof(XalanQName**) == sizeof(PointerVectorAllocatorType::value_type));

        return reinterpret_cast<const XalanQName**>(m_pointerVectorAllocator.allocate(theSize));
    }


    XSLTEngineImpl&                         m_processor;

    XPathFactory&                           m_xpathFactory;

    typedef XalanMemMgrAutoPtr<XPathProcessor>  XPathProcessAutoPtr;

    XPathProcessAutoPtr                     m_xpathProcessor;

    StylesheetVectorType                    m_stylesheets;

    XalanDOMStringPool                      m_stringPool;

    XalanDOMCharVectorAllocatorType         m_xalanDOMCharVectorAllocator;

    mutable XalanDOMString                  m_tempBuffer;

    XalanQNameByValue                       m_scratchQName;

    XalanDOMStringCache                     m_stringCache;

    XalanAVTAllocator                       m_avtAllocator;

    XalanAVTPartSimpleAllocator             m_avtPartSimpleAllocator;

    XalanAVTPartXPathAllocator              m_avtPartXPathAllocator;

    XalanQNameByValueAllocator              m_xalanQNameByValueAllocator;

    const XalanQNameByReference             m_useAttributeSetsQName;

    PointerVectorAllocatorType              m_pointerVectorAllocator;

    ElemTemplateElementVectorType           m_allocatedElements;

    XalanElemApplyTemplatesAllocator        m_elemApplyTemplatesAllocator;

    XalanElemAttributeAllocator             m_elemAttributeAllocator;

    XalanElemAttributeSetAllocator          m_elemAttributeSetAllocator;

    XalanElemCallTemplateAllocator          m_elemCallTemplateAllocator;

    XalanElemElementAllocator               m_elemElementAllocator;

    XalanElemLiteralResultAllocator         m_elemLiteralResultAllocator;

    XalanElemTemplateAllocator              m_elemTemplateAllocator;

    XalanElemTextLiteralAllocator           m_elemTextLiteralAllocator;

    XalanElemValueOfAllocator               m_elemValueOfAllocator;

    XalanElemVariableAllocator              m_elemVariableAllocator;

    XalanMatchPatternDataAllocator          m_matchPatternDataAllocator;

    const XalanQNameByReference             m_spaceAttrQName;

    // Static strings for stylesheet compilation...

    // The string "if"
    static const XalanDOMChar   s_if[];

    // The string "key"
    static const XalanDOMChar   s_key[];

    // The string "copy"
    static const XalanDOMChar   s_copy[];

    // The string "sort"
    static const XalanDOMChar   s_sort[];

    // The string "text"
    static const XalanDOMChar   s_text[];

    // The string "when"
    static const XalanDOMChar   s_when[];

    // The string "empty"
    static const XalanDOMChar   s_empty[];

    // The string "param"
    static const XalanDOMChar   s_param[];

    // The string "choose"
    static const XalanDOMChar   s_choose[];

    // The string "import"
    static const XalanDOMChar   s_import[];

    // The string "number"
    static const XalanDOMChar   s_number[];

    // The string "output"
    static const XalanDOMChar   s_output[];

    // The string "comment"
    static const XalanDOMChar   s_comment[];

    // The string "copy-of"
    static const XalanDOMChar   s_copyOf[];

    // The string "element"
    static const XalanDOMChar   s_element[];

    // The string "include"
    static const XalanDOMChar   s_include[];

    // The string "message"
    static const XalanDOMChar   s_message[];

    // The string "fallback"
    static const XalanDOMChar   s_fallback[];

    // The string "for-each"
    static const XalanDOMChar   s_forEach[];

    // The string "template"
    static const XalanDOMChar   s_template[];

    // The string "value-of"
    static const XalanDOMChar   s_valueOf[];

    // The string "variable"
    static const XalanDOMChar   s_variable[];

    // The string "attribute"
    static const XalanDOMChar   s_attribute[];

    // The string "otherwise"
    static const XalanDOMChar   s_otherwise[];

    // The string "transform"
    static const XalanDOMChar   s_transform[];

    // The string "stylesheet"
    static const XalanDOMChar   s_stylesheet[];

    // The string "with-param"
    static const XalanDOMChar   s_withParam[];

    // The string "strip-space"
    static const XalanDOMChar   s_stripSpace[];

    // The string "apply-imports"
    static const XalanDOMChar   s_applyImports[];

    // The string "attribute-set"
    static const XalanDOMChar   s_attributeSet[];

    // The string "call-template"
    static const XalanDOMChar   s_callTemplate[];

    // The string "decimal-format"
    static const XalanDOMChar   s_decimalFormat[];

    // The string "preserve-space"
    static const XalanDOMChar   s_preserveSpace[];

    // The string "apply-templates"
    static const XalanDOMChar   s_applyTemplates[];

    // The string "namespace-alias"
    static const XalanDOMChar   s_namespaceAlias[];

    // The string "processing-instruction"
    static const XalanDOMChar   s_processingInstruction[];

    // A struct for an array that maps stylesheet element names
    // to int tokens.
    struct ElementTokenTableEntry
    {
        const XalanDOMChar*     m_name;

        eElementToken           m_token;
    };

    static const ElementTokenTableEntry     s_elementTokenTable[];

    static const XalanSize_t                s_elementTokenTableSize;

    static const ElementTokenTableEntry&    s_elementTokenTableLast;

    static const ElementTokenTableEntry&    s_elementTokenTableDummy;
};



XALAN_CPP_NAMESPACE_END



#endif  // STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
