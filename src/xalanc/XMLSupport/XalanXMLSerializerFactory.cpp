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
#include "XalanXMLSerializerFactory.hpp"



#include "xalanc/PlatformSupport/XalanTranscodingServices.hpp"



#include "xalanc/XMLSupport/FormatterToXML.hpp"
#include "xalanc/XMLSupport/FormatterToXMLUnicode.hpp"
#include "xalanc/XMLSupport/XalanUTF8Writer.hpp"
#include "xalanc/XMLSupport/XalanUTF16Writer.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FormatterListener*
XalanXMLSerializerFactory::create(
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
            const XalanDOMString&   theStandaloneString)
{
    bool    isVersion1_1 = false;

    if (theVersion == XalanXMLSerializerBase::s_1_1String)
    {
        isVersion1_1 = true;
    }

    FormatterListener*  theFormatter = 0;

    if (doIndent == false &&
        (theEncoding.empty() == true ||
         XalanTranscodingServices::encodingIsUTF8(theEncoding)))
    {
        if (isVersion1_1 == true)
        {
            typedef FormatterToXMLUnicode<
                        XalanUTF8Writer,
                        XalanXMLSerializerBase::UTF8,
                        XalanXMLSerializerBase::AttributeFunctor1_1,
                        XalanXMLSerializerBase::ContentFunctor1_1,
                        XalanXMLSerializerBase::RangeFunctor,
                        XalanXMLSerializerBase::XML_VERSION_1_1>  Type;

            theFormatter =
                Type::create(
                    theManager,
                    theWriter,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString);
        }
        else
        {
            typedef FormatterToXMLUnicode<
                        XalanUTF8Writer,
                        XalanXMLSerializerBase::UTF8,
                        XalanXMLSerializerBase::AttributeFunctor1_0,
                        XalanXMLSerializerBase::ContentFunctor1_0,
                        XalanXMLSerializerBase::RangeFunctor,
                        XalanXMLSerializerBase::XML_VERSION_1_0>  Type;

            theFormatter =
                Type::create(
                    theManager,
                    theWriter,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString);
        }
    }
    else if (doIndent == false &&
             XalanTranscodingServices::encodingIsUTF16(theEncoding))
    {
        if (isVersion1_1 == true)
        {
            typedef FormatterToXMLUnicode<
                        XalanUTF16Writer,
                        XalanXMLSerializerBase::UTF16,
                        XalanXMLSerializerBase::AttributeFunctor1_1,
                        XalanXMLSerializerBase::ContentFunctor1_1,
                        XalanXMLSerializerBase::RangeFunctor,
                        XalanXMLSerializerBase::XML_VERSION_1_1>  Type;

            theFormatter =
                Type::create(
                    theManager,
                    theWriter,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString);
        }
        else
        {
            typedef FormatterToXMLUnicode<
                        XalanUTF16Writer,
                        XalanXMLSerializerBase::UTF16,
                        XalanXMLSerializerBase::AttributeFunctor1_0,
                        XalanXMLSerializerBase::ContentFunctor1_0,
                        XalanXMLSerializerBase::RangeFunctor,
                        XalanXMLSerializerBase::XML_VERSION_1_0>  Type;

		    theFormatter =
                Type::create(
                    theManager,
                    theWriter,
                    theDoctypeSystem,
                    theDoctypePublic,
                    generateXMLDeclaration,
                    theStandaloneString);
        }
    }
    else
    {
        theFormatter =
            FormatterToXML::create(
                theManager,
                theWriter,
                theVersion,
                doIndent,
                theIndentAmount,
                theEncoding,
                theMediaType,
                theDoctypeSystem,
                theDoctypePublic,
                generateXMLDeclaration,
                theStandaloneString);
    }

    return theFormatter;
}



XALAN_CPP_NAMESPACE_END
