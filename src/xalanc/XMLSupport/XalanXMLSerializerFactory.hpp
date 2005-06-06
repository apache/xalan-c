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
#if !defined(XALANXMLSERIALIZERFACTORY_HEADER_GUARD_1357924680)
#define XALANXMLSERIALIZERFACTORY_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include "xalanc/XMLSupport/XMLSupportDefinitions.hpp"



#include <xercesc/framework/MemoryManager.hpp>




XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;
class Writer;
class XalanDOMString;



XALAN_USING_XERCES(MemoryManager)



/**
 * A factory class for creating XML serializers.
 */
class XALAN_XMLSUPPORT_EXPORT XalanXMLSerializerFactory
{
public:

    /**
     * Create a FormatterListener for generating XML markup.
     */
    static FormatterListener*
    create(
            MemoryManager&          theManager,
            Writer&                 theWriter,
            const XalanDOMString&   theVersion,
            bool                    doIndent,
            int                     theIndentAmount,
            const XalanDOMString&   theEncoding,
            const XalanDOMString&   theMediaType,
            const XalanDOMString&   theDoctypeSystem,
            const XalanDOMString&   theDoctypePublic,
            bool                    generateXMLDeclaration,
            const XalanDOMString&   theStandaloneString);
 
private:

    static void
    setEncoding(
            MemoryManager&          theManager,
            Writer&            theWriter,
            XalanDOMString&    theEncoding);

    // These are not implemented.
    XalanXMLSerializerFactory(const XalanXMLSerializerFactory&);

    XalanXMLSerializerFactory&
    operator=(const XalanXMLSerializerFactory&);

    bool
    operator==(const XalanXMLSerializerFactory&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANXMLSERIALIZERFACTORY_HEADER_GUARD_1357924680
