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

#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>

#include <util/PlatformUtils.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/DOMSupport/DOMSupportDefault.hpp>

#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPathSupportDefault.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>

#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XSLT/XSLTEngineImpl.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>

#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>

#include <XercesPlatformSupport/TextFileOutputStream.hpp>
#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>

//This is here for the threads.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#define THREADFUNCTIONRETURN DWORD WINAPI

#if !defined(XALAN_NO_NAMESPACES)
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::ios_base;
    using std::ostrstream;
    using std::string;
#endif

// Used to hold compiled stylesheet, and source document.
StylesheetRoot* glbStylesheetRoot;
//XalanNode*        glbSourceDoc;

void outputMessage(DWORD id, char msg[])
{
        ostrstream threadMsg;
    
        threadMsg << "\n" << msg << " Thread: " << id << '\0';
        cout << threadMsg.str();
        threadMsg.freeze(false);
}


THREADFUNCTIONRETURN theThread(LPVOID   param)
{
// This routine uses compiled stylesheet (glbStylesheetRoot), which is set using the 
// theProcessor.setStylesheetRoot method. The transform is done using the theProcessor's
// process() method.

    const int   number = reinterpret_cast<int>(param);
    const DWORD     theThreadID = GetCurrentThreadId();

    // Create the support objects that are necessary for running the processor...
    DOMSupportDefault               theDOMSupport;
    XercesParserLiaison             theParserLiaison(theDOMSupport);
    XPathSupportDefault             theXPathSupport(theDOMSupport);
    XSLTProcessorEnvSupportDefault  theXSLTProcessorEnvSupport;
    XObjectFactoryDefault           theXObjectFactory(theXSLTProcessorEnvSupport,
                                                      theXPathSupport);
    XPathFactoryDefault             theXPathFactory;

    // Create a processor...and output start message.
    XSLTEngineImpl  theProcessor(
                    theParserLiaison,
                    theXPathSupport,
                    theXSLTProcessorEnvSupport,
                    theXObjectFactory,
                    theXPathFactory);
    outputMessage(theThreadID,"Starting ");

    // Connect the processor to the support object...
    theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

    // The execution context uses the same factory support objects as
    // the processor, since those objects have the same lifetime as
    // other objects created as a result of the execution.
    StylesheetExecutionContextDefault   ssExecutionContext(
                        theProcessor,
                        theXSLTProcessorEnvSupport,
                        theXPathSupport,
                        theXObjectFactory);

    // Our input files.  The assumption is that the executable will be run
    // from same directory as the input files.

    // Generate the input and output file names.
    ostrstream theFormatterIn, theFormatterOut;
    theFormatterIn << "Birds.xml" << '\0';
    theFormatterOut << "Birds" << number << ".out" << '\0';

    //Generate the XML input and output objects.
    XSLTInputSource     theInputSource(theFormatterIn.str());
    XSLTResultTarget    theResultTarget(theFormatterOut.str());

    // Unfreeze the ostrstreams, so the memory is returned...
    theFormatterIn.freeze(false);
    theFormatterOut.freeze(false);

    // Set the stylesheet to be the compiled stylesheet. Then do the transform. 
    // Report both the start of the transform and end of the thread.
    theProcessor.setStylesheetRoot(glbStylesheetRoot);
    outputMessage(theThreadID,"Transforming");
    theProcessor.process(theInputSource,theResultTarget,ssExecutionContext);
    outputMessage(theThreadID,"Finishing");
    return (0);
}
void doThreads(int x)
{
    DWORD dwStackSize = 4096;               // initial thread stack size
    LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)theThread;
    DWORD dwCreationFlags = 0;              // creation flags
    int nThreads = x;

    std::vector<HANDLE> hThreads;
    hThreads.reserve(nThreads);
    int i=0;
    
    try
    {
        cout << endl << "Clock before starting threads: " << clock() << endl;

        for (i=0; i< nThreads; i++)
            {
                HANDLE hThread;
                DWORD  threadID;

                hThread = CreateThread(
                    0, dwStackSize,
                    lpStartAddress,                 // pointer to thread function
                    reinterpret_cast<LPVOID>(i),    // argument for new thread
                    dwCreationFlags,                // creation flags
                    &threadID);
                assert(hThread != 0);
                hThreads.push_back(hThread);
            }
        WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);

        cout << endl << "Clock after threads: " << clock() << endl;

        for (i=0; i< nThreads; i++)
            CloseHandle(hThreads[i]);
    }
    catch(...)
    {
        cerr << "Exception caught!!!"
             << endl
            << endl;
    }
}


int
main(
            int     argc,
            char*   /* argv */[])
{

    if (argc != 1)
    {
        cerr << "Usage: ThreadTest"
             << endl
             << endl;
    }
    else
    {
        try
        {
            // Call the static initializers...
            XMLPlatformUtils::Initialize();
            XSLTEngineImpl::Initialize();

            // Create the necessary stuff of compile the stylesheet.
            DOMSupportDefault               ssDOMSupport;
            XercesParserLiaison             ssParserLiaison(ssDOMSupport);
            XPathSupportDefault             ssXPathSupport(ssDOMSupport);
            XSLTProcessorEnvSupportDefault  ssXSLTProcessorEnvSupport;
            XObjectFactoryDefault           ssXObjectFactory(ssXSLTProcessorEnvSupport,
                                                            ssXPathSupport);
            XPathFactoryDefault             ssXPathFactory;

            // Create a processor to compile the stylesheet...
            XSLTEngineImpl  ssProcessor(
                    ssParserLiaison,
                    ssXPathSupport,
                    ssXSLTProcessorEnvSupport,
                    ssXObjectFactory,
                    ssXPathFactory);

            // Create separate factory support objects so the stylesheet's
            // factory-created XObject and XPath instances are independent 
            // from processor's.
            XObjectFactoryDefault   ssStylesheetXObjectFactory(
                                    ssXSLTProcessorEnvSupport,
                                    ssXPathSupport);
            XPathFactoryDefault     ssStylesheetXPathFactory;

            // Create a stylesheet construction context, using the
            // stylesheet's factory support objects.
            StylesheetConstructionContextDefault    ssConstructionContext(
                                                    ssProcessor,
                                                    ssXSLTProcessorEnvSupport,
                                                    ssStylesheetXObjectFactory,
                                                    ssStylesheetXPathFactory);

            const XalanDOMString  theXSLFileName("Birds.xsl");
            const XalanDOMString  theXMLFileName("Birds.xml");

            // Our stylesheet input source...
            XSLTInputSource     ssStylesheetSourceXSL(c_wstr(theXSLFileName));
            XSLTInputSource     ssStylesheetSourceXML(c_wstr(theXMLFileName));

            // Ask the processor to create a StylesheetRoot for the specified
            // input XSL.  This is the compiled stylesheet.  We don't have to
            // delete it, since it is owned by the StylesheetConstructionContext
            // instance.
            glbStylesheetRoot = ssProcessor.processStylesheet(ssStylesheetSourceXSL,
                                                       ssConstructionContext);
            assert(glbStylesheetRoot != 0);
            
            // Create a compiled source tree as well, to use with perThreadCC
            // glbSourceDoc = ssProcessor.getSourceTreeFromInput(ssStylesheetSourceXML);
            // assert(glbSourceDoc != 0);

            doThreads(10);
        }
        catch(...)
        {
            cerr << "Exception caught!!!"
                 << endl
                 << endl;
        }
    }

    return 0;
}
