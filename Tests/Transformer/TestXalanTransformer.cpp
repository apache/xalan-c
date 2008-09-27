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

#include <xalanc/Include/PlatformDefinitions.hpp>



#define TEST_XALAN_CPP


// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif



#include <xercesc/util/PlatformUtils.hpp>


#if defined(TEST_XALAN_CPP)

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif


XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)
XALAN_USING_STD(ostrstream)

#else

#include <xalanc/XalanTransformer/XalanCAPI.h>

#include <cstdio>

#endif


static CallbackSizeType xalan_output_handler(const char *data, CallbackSizeType length, void *handle)
{
    FILE *fp = (FILE*)handle;

    return fwrite( data, sizeof( char ), length, fp );
}



static void xalan_flush_handler(void *handle)
{
    FILE *fp = (FILE*)handle;

    fflush(fp);
}



int
runTests()
{
    const char* const       theXMLFileName = "d:\\xslt\\xsl-test\\perf\\basic\\basic-all_well.xml";
    const char* const       theXSLFileName = "d:\\xslt\\xsl-test\\perf\\basic\\basic-all_well.xsl";
    const char* const       theOutFileName = "d:\\Transformer-Results\\basic-all_well.out";

    const char* const       theXMLFileName2 = "d:\\xslt\\xsl-test\\perf\\basic\\miscfam.xml";
    const char* const       theXSLFileName2 = "d:\\xslt\\xsl-test\\perf\\basic\\miscfam.xsl";

    const char* const       theXMLFileName3 = "d:\\xslt\\xsl-test\\conf\\embed\\embed01.xml";   
    const char* const       theOutFileName3 = "d:\\Transformer-Results\\embed01.out";

    const char* const       theXMLFileName4 = "d:\\xml-xalan\\c\\samples\\UseStylesheetParam\\foo.xml"; 
    const char* const       theXSLFileName4 = "d:\\xml-xalan\\c\\samples\\UseStylesheetParam\\foo.xsl"; 

#if defined(TEST_XALAN_CPP)

    XALAN_USING_XALAN(XalanTransformer)
    XALAN_USING_XALAN(XalanCompiledStylesheet)

    XalanTransformer    xalan;

    const XalanCompiledStylesheet*  css = 0;
    
    if (xalan.compileStylesheet(theXSLFileName, css) != 0)
    {
        cout << xalan.getLastError();

        return 0;   
    }

    for(int i=0; i<1; ++i)
    {

        if(xalan.transform(theXMLFileName, css, "d:\\transformer-results\\css.out"))
        {
            cout << xalan.getLastError();

            return 0;   
        }
        
        if(xalan.transform(theXMLFileName3, theOutFileName3))
        {
            cout << xalan.getLastError();

            return 0;   
        }

        if(xalan.transform(theXMLFileName, theXSLFileName, theOutFileName))
        {
            cout << xalan.getLastError();

            return 0;   
        }

        ostrstream  theOutput;

        if(xalan.transform(theXMLFileName2, theXSLFileName2, &theOutput))
        {
            cout << xalan.getLastError();

            return 0;   
        }

        theOutput << '\0';

        cout << theOutput.str();

        theOutput.freeze(false);

        ostrstream  theOutput3;

        if(xalan.transform(theXMLFileName3, theOutput3))
        {
            cout << xalan.getLastError();

            return 0;   
        }
/*      
        if(xalan.transform(theXMLFileName, css, cout))
        {
            cout << xalan.getLastError();

            return 0;   
        }
*/      
        xalan.setStylesheetParam("param1",
                                 "'What is Up'");

        if(xalan.transform(theXMLFileName4, theXSLFileName4, &cout))
        {
            cout << xalan.getLastError();

            return 0;   
        }

        if(xalan.transform(theXMLFileName4, theXSLFileName4, &cout))
        {
            cout << xalan.getLastError();

            return 0;   
        }
    }

#else
    // $$$ ToDo: This C code is broken, because it returns without
    // shutting things down!!!!
    XalanInitialize();

    XalanHandle xalan = CreateXalanTransformer();
    XalanCSSHandle theXalanCSS2;
    XalanCSSHandle theXalanCSS4;
    XalanPSHandle  theXalanPS2;

    if (XalanCompileStylesheet(theXSLFileName2, xalan, &theXalanCSS2) != 0)
    {
        puts("Error");
        puts(XalanGetLastError(xalan));

        return 0;   
    }

    if (XalanCompileStylesheet(theXSLFileName4, xalan, &theXalanCSS4) != 0)
    {
        puts("Error");
        puts(XalanGetLastError(xalan));

        return 0;   
    }

    if (XalanParseSource(theXMLFileName2, xalan, &theXalanPS2) != 0)
    {
        puts("Error");
        puts(XalanGetLastError(xalan));

        return 0;   
    }

    for(int i = 0; i < 2; ++i)
    {
        if(XalanTransformToFile(theXMLFileName, theXSLFileName, theOutFileName, xalan))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;   
        }

        char*   theOutput;

        if(XalanTransformToData(theXMLFileName2, theXSLFileName2, &theOutput, xalan))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;   
        }

        puts(theOutput);

        XalanFreeData(theOutput);

        if(XalanTransformToFile(theXMLFileName3, NULL, theOutFileName3, xalan))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;   
        }
        
        if(XalanTransformToData(theXMLFileName3, NULL, &theOutput, xalan))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;   
        }

        puts(theOutput);

        XalanFreeData(theOutput);

        if(XalanTransformToDataPrebuilt(theXalanPS2, theXalanCSS2, &theOutput, xalan))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;   
        }

        puts(theOutput);

        XalanFreeData(theOutput);

        FILE* fp =0;
        fp = fopen("c:\\temp\\test.out", "w");

        if(XalanTransformToHandler(theXMLFileName2, theXSLFileName2, xalan, fp, xalan_output_handler, xalan_flush_handler))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;   
        }

        fclose(fp);

        XalanSetStylesheetParam("param1", "'hi'", xalan);

        //if(xalan.transform(theXMLFileName4, theXSLFileName4, &cout))
        if(XalanTransformToDataPrebuilt(theXMLFileName4, theXalanCSS4, &theOutput, xalan))
        {
            puts("Error");
            puts(XalanGetLastError(xalan));
            return 0;
        }

        puts(theOutput);

        XalanFreeData(theOutput);
    }

    DeleteXalanTransformer(xalan);

    // Terminate Xerces and Xalan, and clean up the ICU...
    XalanTerminate(1);
#endif

    return 0;
}



int
main(
            int     /* argc */,
            char*   /* argv */ [])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    int theResult = 0;

#if defined(TEST_XALAN_CPP)
    try
    {
        XALAN_USING_XERCES(XMLPlatformUtils)

        XALAN_USING_XALAN(XalanTransformer)

        // Call the static initializers for xerces and xalan, and create a transformer
        //
        XMLPlatformUtils::Initialize();

        XalanTransformer::initialize();

        theResult = runTests();

        // Terminate everything...
        XalanTransformer::terminate();

        XMLPlatformUtils::Terminate();

        XalanTransformer::ICUCleanUp();
    }
    catch(...)
    {
        cerr << "Initialization failed!" << endl << endl;

        theResult = -1;
    }
#endif

    return theResult;
}
