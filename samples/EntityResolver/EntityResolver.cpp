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

#include "xalanc/Include/PlatformDefinitions.hpp"



#include <cassert>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/sax/EntityResolver.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLEntityResolver.hpp"
#include "xercesc/util/XMLResourceIdentifier.hpp"



#include "xalanc/XalanTransformer/XalanTransformer.hpp"



XALAN_USING_XERCES(EntityResolver)
XALAN_USING_XERCES(InputSource)
XALAN_USING_XERCES(MemBufInputSource)
XALAN_USING_XALAN(MemoryManager)
XALAN_USING_XERCES(XMLEntityResolver)
XALAN_USING_XERCES(XMLResourceIdentifier)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanTransformer)



// This class implements both the EntityResolver and XMLEntityResolver abstract
// base classes.  It maps three system IDs to three pre-defined entities.  One
// entity is present in the input XML file, one in the input XSL file, and another
// as an argument to the XSLT document() function.
class MyEntityResolver : public EntityResolver, public XMLEntityResolver
{
public:

    MyEntityResolver(
        const XalanDOMString&   theDocumentEntityFile,
        const XalanDOMString&   theStylesheetEntityFile,
        const XalanDOMString&   theDocumentFunctionEntityFile) :
        EntityResolver(),
        XMLEntityResolver(),
        m_documentEntityFile(theDocumentEntityFile),
        m_stylesheetEntityFile(theStylesheetEntityFile),
        m_documentFunctionEntityFile(theDocumentFunctionEntityFile)
    {
    }

    virtual
    ~MyEntityResolver()
    {
    }

    virtual InputSource*
    resolveEntity(
        const XMLCh* const  /* publicId */,
        const XMLCh* const  systemId)
    {
        if (m_documentEntityFile == systemId)
        {
            return new MemBufInputSource(
                            reinterpret_cast<const XMLByte*>(s_documentEntity),
                            s_documentEntityLength,
                            m_documentEntityFile.c_str());
        }
        else if (m_stylesheetEntityFile == systemId)
        {
            return new MemBufInputSource(
                            reinterpret_cast<const XMLByte*>(s_stylesheetEntity),
                            s_stylesheetEntityLength,
                            m_stylesheetEntityFile.c_str());
        }
        else if (m_documentFunctionEntityFile == systemId)
        {
            return new MemBufInputSource(
                            reinterpret_cast<const XMLByte*>(s_documentFunctionEntity),
                            s_documentFunctionEntityLength,
                            m_documentFunctionEntityFile.c_str());
        }
        else
        {
            return 0;
        }
    }

    virtual InputSource*
    resolveEntity(XMLResourceIdentifier*    resourceIdentifier)
    {
        assert(resourceIdentifier != 0);

        if (resourceIdentifier->getResourceIdentifierType() == XMLResourceIdentifier::ExternalEntity)
        {
            const XMLCh* const  theSystemID = resourceIdentifier->getSystemId();

            if (m_documentEntityFile == theSystemID)
            {
                return new MemBufInputSource(
                                reinterpret_cast<const XMLByte*>(s_documentEntity),
                                s_documentEntityLength,
                                m_documentEntityFile.c_str());
            }
            else if (m_stylesheetEntityFile == theSystemID)
            {
                return new MemBufInputSource(
                                reinterpret_cast<const XMLByte*>(s_stylesheetEntity),
                                s_stylesheetEntityLength,
                                m_stylesheetEntityFile.c_str());
            }
            else if (m_documentFunctionEntityFile == theSystemID)
            {
                return new MemBufInputSource(
                                reinterpret_cast<const XMLByte*>(s_documentFunctionEntity),
                                s_documentFunctionEntityLength,
                                m_documentFunctionEntityFile.c_str());
            }
        }

        return 0;
    }

private:

    // Not implemented...
    MyEntityResolver&
    operator=(const MyEntityResolver&);

    bool
    operator==(const MyEntityResolver&) const;


    const XalanDOMString&   m_documentEntityFile;

    const XalanDOMString&   m_stylesheetEntityFile;

    const XalanDOMString&   m_documentFunctionEntityFile;

    static const char* const    s_documentEntity;

    static const XalanDOMString::size_type  s_documentEntityLength;

    static const char* const    s_stylesheetEntity;

    static const XalanDOMString::size_type  s_stylesheetEntityLength;

    static const char* const    s_documentFunctionEntity;

    static const XalanDOMString::size_type  s_documentFunctionEntityLength;
};


const char* const   MyEntityResolver::s_documentEntity =
#if defined(XALAN_NON_ASCII_PLATFORM)
    "<?xml version='1.0' encoding='EBCDIC-CP-US' ?>Hello world!";
#else
    "<?xml version='1.0' encoding='UTF-8' ?>Hello world!";
#endif

const XalanDOMString::size_type  MyEntityResolver::s_documentEntityLength =
        XalanDOMString::length(s_documentEntity);

const char* const   MyEntityResolver::s_stylesheetEntity =
#if defined(XALAN_NON_ASCII_PLATFORM)
    "<?xml version='1.0' encoding='EBCDIC-CP-US' ?><xsl:apply-templates xmlns:xsl='http://www.w3.org/1999/XSL/Transform'/>";
#else
    "<?xml version='1.0' encoding='UTF-8' ?><xsl:apply-templates xmlns:xsl='http://www.w3.org/1999/XSL/Transform'/>";
#endif

const XalanDOMString::size_type  MyEntityResolver::s_stylesheetEntityLength =
        XalanDOMString::length(s_stylesheetEntity);

const char* const   MyEntityResolver::s_documentFunctionEntity =
#if defined(XALAN_NON_ASCII_PLATFORM)
    "<?xml version='1.0' encoding='EBCDIC-CP-US' ?><hello>Hello world from the document() function!</hello>";
#else
    "<?xml version='1.0' encoding='UTF-8' ?><hello>Hello world from the document() function!</hello>";
#endif

const XalanDOMString::size_type  MyEntityResolver::s_documentFunctionEntityLength =
        XalanDOMString::length(s_documentFunctionEntity);



static int
transform(
            XalanTransformer&       theTransformer,
            const XalanDOMString&   theXMLFile,
            const XalanDOMString&   theXSLFile,
            const char*             theOutputFile,
            bool                    fUseDOM)
{
    XALAN_USING_XALAN(XalanCompiledStylesheet)
    XALAN_USING_XALAN(XalanParsedSource)

    typedef XalanTransformer::EnsureDestroyCompiledStylesheet   EnsureDestroyCompiledStylesheet;
    typedef XalanTransformer::EnsureDestroyParsedSource         EnsureDestroyParsedSource;

    const XalanCompiledStylesheet*  theCompiledStylesheet;

    int     theResult =
        theTransformer.compileStylesheet(theXSLFile, theCompiledStylesheet);

    if (theResult != 0)
    {
        return theResult;
    }

    const EnsureDestroyCompiledStylesheet   theStylesheetGuard(theTransformer, theCompiledStylesheet);

    const XalanParsedSource*    theParsedSource;

    theResult =
        theTransformer.parseSource(theXMLFile, theParsedSource, fUseDOM);

    if (theResult != 0)
    {
        return theResult;
    }

    const EnsureDestroyParsedSource     theSourceGuard(theTransformer, theParsedSource);

    return theTransformer.transform(*theParsedSource, theCompiledStylesheet, theOutputFile);
}



static int
transformWithSourceTree(
            XalanTransformer&       theTransformer,
            MyEntityResolver&       theEntityResolver,
            const XalanDOMString&   theXMLFile,
            const XalanDOMString&   theXSLFile)
{
    theTransformer.setEntityResolver(&theEntityResolver);

    // Do the transform.
    int     theResult = transform(theTransformer, theXMLFile, theXSLFile, "foo1.out", false);
    
    if (theResult == 0)
    {
        theTransformer.setXMLEntityResolver(&theEntityResolver);

        theResult = transform(theTransformer, theXMLFile, theXSLFile, "foo2.out", false);
    }

    return theResult;
}



static int
transformWithXercesDOM(
            XalanTransformer&       theTransformer,
            MyEntityResolver&       theEntityResolver,
            const XalanDOMString&   theXMLFile,
            const XalanDOMString&   theXSLFile)
{
    theTransformer.setEntityResolver(&theEntityResolver);

    // Do the transform.
    int     theResult = transform(theTransformer, theXMLFile, theXSLFile, "foo3.out", true);
    
    if (theResult == 0)
    {
        theTransformer.setXMLEntityResolver(&theEntityResolver);

        theResult = transform(theTransformer, theXMLFile, theXSLFile, "foo4.out", true);
    }

    return theResult;
}



int
main(
            int     argc,
            char*   /* argv */[])
{
    XALAN_USING_STD(cerr)
    XALAN_USING_STD(endl)

    int theResult = 0;

    if (argc != 1)
    {
        cerr << "Usage: EntityResolver"
             << endl
             << endl;
    }
    else
    {
        XALAN_USING_XERCES(XMLPlatformUtils)
        XALAN_USING_XERCES(XMLException)

        // Call the static initializer for Xerces.
        try
        {
             XMLPlatformUtils::Initialize();
        }
        catch (const XMLException& toCatch)
        {
             cerr << "Error during Xerces initialization.  Error code was "
                  << toCatch.getCode()
                  << "."
                  << endl;

             theResult = -1;
        }

        if (theResult == 0)
        {
            // Initialize Xalan.
            XalanTransformer::initialize();

            {
                // Create a XalanTransformer.
                XalanTransformer    theTransformer;

                const XalanDOMString    theXMLFile("foo.xml");
                const XalanDOMString    theXSLFile("foo.xsl");

                const XalanDOMString    theDocumentEntityFile("hello.xml");
                const XalanDOMString    theStylesheetEntityFile("apply-templates.xml");
                const XalanDOMString    theDocumentFunctionEntityFile("document.xml");

                MyEntityResolver    theEntityResolver(
                                        theDocumentEntityFile,
                                        theStylesheetEntityFile,
                                        theDocumentFunctionEntityFile);

                theResult =
                    transformWithSourceTree(
                        theTransformer,
                        theEntityResolver,
                        theXMLFile,
                        theXSLFile);

                if (theResult == 0)
                {
                    theResult =
                        transformWithXercesDOM(
                            theTransformer,
                            theEntityResolver,
                            theXMLFile,
                            theXSLFile);
                }

                if(theResult != 0)
                {
                    cerr << "EntityResolver Error: \n" << theTransformer.getLastError()
                         << endl
                         << endl;
                }
            }

            // Terminate Xalan...
            XalanTransformer::terminate();
        }

        // Terminate Xerces...
        XMLPlatformUtils::Terminate();

        // Clean up the ICU, if it's integrated...
        XalanTransformer::ICUCleanUp();
    }

    return theResult;
}
