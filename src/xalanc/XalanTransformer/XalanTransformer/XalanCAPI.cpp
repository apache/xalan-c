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
#include "XalanTransformerDefinitions.hpp"

#include <xercesc/util/PlatformUtils.hpp>

#include <cassert>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <strstream.h>
#else
#include <strstream>
#endif

#include "XalanCAPI.h"
#include "XalanTransformer.hpp"
#include "XalanParsedSource.hpp"

#include "xalanc/Include/XalanMemoryManagement.hpp"

XALAN_USING_STD(istrstream)

XALAN_USING_XALAN(XalanAllocationGuard)
XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XERCES(MemoryManager)

XALAN_USING_XALAN(XalanMemMgrAutoPtr)
XALAN_USING_XALAN(XalanMemMgrs)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)

static bool fInitialized = false;


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanInitialize()
{
    try
    {
        // Call the static initializer for Xerces.
        XMLPlatformUtils::Initialize();

        // Initialize Xalan.
        XalanTransformer::initialize(XalanMemMgrs::getDefaultXercesMemMgr());

        fInitialized = true;
    }
    catch(...)
    {
    }

    return fInitialized == true ? 0 : -1;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanTerminate(int  fCleanUpICU)
{
    // Terminate Xalan.
    XalanTransformer::terminate();

    // Call the static terminator for Xerces.
    XMLPlatformUtils::Terminate();

    // Call the cleanup function for the ICU,
    // if requested.
    if (fCleanUpICU)
    {
        XalanTransformer::ICUCleanUp();
    }
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanHandle)
CreateXalanTransformer()
{   
    MemoryManager&  theManager =
        XalanMemMgrs::getDefaultXercesMemMgr();

    // Create a XalanTransformer object.
    typedef XalanTransformer ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(theManager);

    theGuard.release();

    return theResult;
}


inline XalanTransformer*
getTransformer(XalanHandle  theHandle)
{
    assert(theHandle != 0);

    return static_cast<XalanTransformer*>(theHandle);
}


inline const XalanCompiledStylesheet*
getStylesheet(XalanCSSHandle    theHandle)
{
    assert(theHandle != 0);

    return reinterpret_cast<const XalanCompiledStylesheet*>(theHandle);
}


inline const XalanParsedSource*
getParsedSource(XalanPSHandle   theHandle)
{
    assert(theHandle != 0);

    return reinterpret_cast<const XalanParsedSource*>(theHandle);
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
DeleteXalanTransformer(XalanHandle theXalanHandle)
{
    // Delete a XalanTransformer object.
    
    XalanTransformer* transformer = getTransformer(theXalanHandle);

    transformer->~XalanTransformer();

    XalanMemMgrs::getDefaultXercesMemMgr().deallocate(transformer);


}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToFile(
            const char*     theXMLFileName, 
            const char*     theXSLFileName,
            const char*     theOutFileName,
            XalanHandle     theXalanHandle)
{
    if(theXSLFileName == 0)
    {
        return getTransformer(theXalanHandle)->transform(
            XSLTInputSource(theXMLFileName, XalanMemMgrs::getDefaultXercesMemMgr()),
            XSLTResultTarget(theOutFileName, XalanMemMgrs::getDefaultXercesMemMgr()));
    }
    else
    {
        return getTransformer(theXalanHandle)->transform(
            XSLTInputSource(theXMLFileName, XalanMemMgrs::getDefaultXercesMemMgr()),
            XSLTInputSource(theXSLFileName, XalanMemMgrs::getDefaultXercesMemMgr()),
            XSLTResultTarget(theOutFileName, XalanMemMgrs::getDefaultXercesMemMgr()));
    }
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToFilePrebuilt(
            XalanPSHandle   theParsedSource, 
            XalanCSSHandle  theCSSHandle,
            const char*     theOutFileName,
            XalanHandle     theXalanHandle)
{
    // Do the transformation...
    return getTransformer(theXalanHandle)->transform(
                *getParsedSource(theParsedSource),
                getStylesheet(theCSSHandle),
                XSLTResultTarget(theOutFileName, XalanMemMgrs::getDefaultXercesMemMgr()));
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToData(
            const char*     theXMLFileName, 
            const char*     theXSLFileName,
            char**          theOutput,
            XalanHandle     theXalanHandle)
{
    XALAN_USING_STD(ostrstream)

    int status = 0;     

    ostrstream  theOutputStream;    

    XalanTransformer* const     theTransformer =
        getTransformer(theXalanHandle);

    MemoryManager&  theMemoryManager =
        theTransformer->getMemoryManager();

    if(theXSLFileName == 0)
    {
        status = 
            theTransformer->transform(
                XSLTInputSource(theXMLFileName, theMemoryManager),
                XSLTResultTarget(theOutputStream, theMemoryManager));
    }
    else
    {
        status =
            theTransformer->transform(
                XSLTInputSource(theXMLFileName, theMemoryManager),
                XSLTInputSource(theXSLFileName, theMemoryManager),
                XSLTResultTarget(theOutputStream, theMemoryManager));
    }

    if (status == 0)
    {
        // Null-terminate the data.
        theOutputStream << '\0';

        *theOutput = theOutputStream.str();
    }

    return status;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanTransformToDataPrebuilt(
            XalanPSHandle   theParsedSource, 
            XalanCSSHandle  theCSSHandle,
            char**          theOutput,
            XalanHandle     theXalanHandle)
{
    XALAN_USING_STD(ostrstream)

    ostrstream  theOutputStream;    

    // Do the transformation...
    const int   status =
        getTransformer(theXalanHandle)->transform(
            *getParsedSource(theParsedSource),
            getStylesheet(theCSSHandle),
            XSLTResultTarget(theOutputStream, XalanMemMgrs::getDefaultXercesMemMgr()));

    if (status == 0)
    {
        // Null-terminate the data.
        theOutputStream << '\0';

        *theOutput = theOutputStream.str();
    }

    return status;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanFreeData(char* theStream)
{
    // Delete the data.
    delete[] theStream;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
XalanTransformToHandler(
            const char*             theXMLFileName,
            const char*             theXSLFileName,
            XalanHandle             theXalanHandle,
            void*                   theOutputHandle,
            XalanOutputHandlerType  theOutputHandler,
            XalanFlushHandlerType   theFlushHandler)
{
    XalanTransformer* const     theTransformer =
        getTransformer(theXalanHandle);

    MemoryManager&  theMemoryManager =
        theTransformer->getMemoryManager();

    // Do the transformation...
    return theTransformer->transform(
                XSLTInputSource(theXMLFileName, theMemoryManager),
                XSLTInputSource(theXSLFileName, theMemoryManager),
                theOutputHandle,
                theOutputHandler,
                theFlushHandler);
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
XalanTransformToHandlerPrebuilt(
            XalanPSHandle           theParsedSource,
            XalanCSSHandle          theCSSHandle,
            XalanHandle             theXalanHandle,
            void*                   theOutputHandle,
            XalanOutputHandlerType  theOutputHandler,
            XalanFlushHandlerType   theFlushHandler)
{
    // Do the transformation...
    return getTransformer(theXalanHandle)->transform(
            *getParsedSource(theParsedSource),
            getStylesheet(theCSSHandle),
            theOutputHandle,
            theOutputHandler,
            theFlushHandler);
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanCompileStylesheet(
            const char*         theXSLFileName,
            XalanHandle         theXalanHandle,
            XalanCSSHandle*     theCSSHandle)
{
    XalanTransformer* const     theTransformer =
        getTransformer(theXalanHandle);

    MemoryManager&  theMemoryManager =
        theTransformer->getMemoryManager();

    const XalanCompiledStylesheet*  theCompiledStylesheet = 0;

    const int   theResult =
        theTransformer->compileStylesheet(
            XSLTInputSource(theXSLFileName, theMemoryManager),
            theCompiledStylesheet);

    if (theResult == 0)
    {
        *theCSSHandle = theCompiledStylesheet;
    }

    return theResult;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanCompileStylesheetFromStream(
            const char*         theXSLStream,
            unsigned long       theXSLStreamLength,
            XalanHandle         theXalanHandle,
            XalanCSSHandle*     theCSSHandle)
{
    XalanTransformer* const     theTransformer =
        getTransformer(theXalanHandle);

    MemoryManager&  theMemoryManager =
        theTransformer->getMemoryManager();

    const XalanCompiledStylesheet*  theCompiledStylesheet = 0;

    istrstream  theInputStream(theXSLStream, theXSLStreamLength);

    const int   theResult =
        theTransformer->compileStylesheet(
            XSLTInputSource(theInputStream, theMemoryManager),
            theCompiledStylesheet);

    if (theResult == 0)
    {
        *theCSSHandle = theCompiledStylesheet;
    }

    return theResult;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanDestroyCompiledStylesheet(
            XalanCSSHandle  theCSSHandle,
            XalanHandle     theXalanHandle)
{
    return getTransformer(theXalanHandle)->destroyStylesheet(getStylesheet(theCSSHandle));
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanParseSource(
            const char*     theXMLFileName,
            XalanHandle     theXalanHandle,
            XalanPSHandle*  thePSHandle)
{
    XalanTransformer* const     theTransformer =
        getTransformer(theXalanHandle);

    MemoryManager&  theMemoryManager =
        theTransformer->getMemoryManager();

    const XalanParsedSource*    theParsedSource = 0;

    const int   theResult =
        theTransformer->parseSource(
            XSLTInputSource(theXMLFileName, theMemoryManager),
            theParsedSource);

    if (theResult == 0)
    {
        *thePSHandle = theParsedSource;
    }

    return theResult;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanParseSourceFromStream(
            const char*     theXMLStream,
            unsigned long   theXMLStreamLength,
            XalanHandle     theXalanHandle,
            XalanPSHandle*  thePSHandle)
{
    XalanTransformer* const     theTransformer =
        getTransformer(theXalanHandle);

    MemoryManager&  theMemoryManager =
        theTransformer->getMemoryManager();

    const XalanParsedSource*    theParsedSource = 0;

    istrstream  theInputStream(theXMLStream, theXMLStreamLength);

    const int   theResult =
        theTransformer->parseSource(
            XSLTInputSource(theInputStream, theMemoryManager),
            theParsedSource);

    if (theResult == 0)
    {
        *thePSHandle = theParsedSource;
    }

    return theResult;
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
XalanDestroyParsedSource(
            XalanPSHandle   thePSHandle,
            XalanHandle     theXalanHandle)
{
    return getTransformer(theXalanHandle)->destroyParsedSource(getParsedSource(thePSHandle));
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParam(
            const char*     key,
            const char*     expression,
            XalanHandle     theXalanHandle)
{
    getTransformer(theXalanHandle)->setStylesheetParam(
        key,
        expression);
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParamUTF(
                const XalanUTF16Char*   key,
                const XalanUTF16Char*   expression,
                XalanHandle             theXalanHandle)
{
    getTransformer(theXalanHandle)->setStylesheetParam(
        XalanDOMString(key, XalanMemMgrs::getDefaultXercesMemMgr()),
        XalanDOMString(expression, XalanMemMgrs::getDefaultXercesMemMgr()));
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParamNodeset(
                const char*             key,
                XalanPSHandle           theNodeset,
                XalanHandle             theXalanHandle)
{
    getTransformer(theXalanHandle)->setStylesheetParam(
        key,
        getParsedSource(theNodeset)->getDocument());
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParamUTFNodeset(
                const XalanUTF16Char*   key,
                XalanPSHandle           theNodeset,
                XalanHandle             theXalanHandle)
{
    getTransformer(theXalanHandle)->setStylesheetParam(
        XalanDOMString(key, XalanMemMgrs::getDefaultXercesMemMgr()),
        getParsedSource(theNodeset)->getDocument());
}
        

XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParamNumber(
                const char*             key,
                double                  theNumber,
                XalanHandle             theXalanHandle)
{
    getTransformer(theXalanHandle)->setStylesheetParam(
        key,
        theNumber);
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanSetStylesheetParamUTFNumber(
                const XalanUTF16Char*   key,
                double                  theNumber,
                XalanHandle             theXalanHandle)
{
    getTransformer(theXalanHandle)->setStylesheetParam(
        XalanDOMString(key, XalanMemMgrs::getDefaultXercesMemMgr()),
        theNumber);
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
XalanClearStylesheetParams(
                XalanHandle             theXalanHandle)
{
    getTransformer(theXalanHandle)->clearStylesheetParams();
}


XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanCCharPtr)
XalanGetLastError(XalanHandle theXalanHandle)
{
    // Get the last error.
    return getTransformer(theXalanHandle)->getLastError();
}
