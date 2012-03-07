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

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif

// for 'C' strcmp function used in parsing main() parameters
#include <string.h>

#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

// The next 2 include files are for parsing XML documents as nodeset parameters.

#include <xalanc/XalanTransformer/XalanParsedSource.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>

// DATA STORAGE FOR TOP-LEVEL PARAMETERS
struct {
    char *  parmType;   // -s key string, -n key number, -d key document
    char *  parmName;   // the key or parameter name
    char *  parmValue;  // the represented string value
} Parameter[15];

// The Parsed Document Nodesets
XALAN_USING_XALAN(XalanParsedSource)
const XalanParsedSource * NodesetParam[15] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


// THE USAGE HELP FUNCTION

int Usage()
{
    XALAN_USING_STD(cerr)
    XALAN_USING_STD(endl)
    cerr << "Usage: UseStylesheetParam xmlFile xslFile outFile [parameters]" <<endl
         << "   Parameters are:" <<endl
         << "   -s name \"'String Value'\"" <<endl
         << "   -n name NumberValue" <<endl
         << "   -d name \"Document Nodeset File\"" <<endl;
    return 1;
}

// THE MAIN SAMPLE PROGRAM

int
main(
          int       argc,
          char*     argv[])
{
    XALAN_USING_STD(cerr)
    XALAN_USING_STD(endl)

    int theResult = 0;
    int i, j;
    int paramCount = 0;

    if (argc < 4)
    {
        return Usage();
    }

    // Collect the top-level parameter arguments

    for (i=4, j=0; i < argc;)
    {
        if (!strcmp(argv[i],"-s") || !strcmp(argv[i],"-n") || !strcmp(argv[i],"-d"))
        {
            Parameter[j].parmType  = argv[i++];
            Parameter[j].parmName  = argv[i++];
            Parameter[j].parmValue = argv[i++];
            paramCount = ++j;
        }
        else  return Usage();
    }

    {
        XALAN_USING_XERCES(XMLPlatformUtils)
        XALAN_USING_XERCES(XMLException)

        XALAN_USING_XALAN(XalanTransformer)
        XALAN_USING_XALAN(XSLTInputSource)

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

        if ( theResult == 0)
        {

            // Initialize Xalan.
            XalanTransformer::initialize();

            {
                XALAN_USING_XALAN(XalanDOMString)

                // Create a XalanTransformer.
                XalanTransformer    theXalanTransformer;

                // Prepare and set the top-level parameters

                for (j = 0; j < paramCount; j++)
                {
                    // The String Top-Level Parameters
                    if (!strcmp(Parameter[j].parmType, "-s"))
                    {
                        theXalanTransformer.setStylesheetParam(
                            Parameter[j].parmName,
                            Parameter[j].parmValue);
                    }

                    // The DOUBLE Number Top-Level Parameters
                    if (!strcmp(Parameter[j].parmType, "-n"))
                    {
                        theXalanTransformer.setStylesheetParam(
                            Parameter[j].parmName,
                            strtod(Parameter[j].parmValue, NULL));
                    }

                    // The Parsed XML Document (nodeset) Top-Level Parameters
                    if (!strcmp(Parameter[j].parmType, "-d"))
                    {
//                      NodesetParam[j] = NULL;
                        if (theXalanTransformer.parseSource(
                                    XSLTInputSource(Parameter[j].parmValue,
                                        theXalanTransformer.getMemoryManager()),
                                    NodesetParam[j]) == 0)
                        {
                            theXalanTransformer.setStylesheetParam(
                                Parameter[j].parmName,
                                NodesetParam[j]->getDocument());
                        }
                        else
                        {
                            cerr << "WARNING: XML parsing error in "
                                 << Parameter[j].parmName << " file: " 
                                 << Parameter[j].parmValue <<endl
                                 << theXalanTransformer.getLastError() <<endl <<endl;
                        }
                    }
                }

                // Our input files...The assumption is that the executable will be run
                // from same directory as the input files.
                // argv[1]="foo.xml"
                // argv[2]="foo.xslt"
                // argv[3]="foo.out"
                // -d parmA "parmA.xml"
                // -d parmB "parmB.xml"
                // -s stringA "'This is a test string'"
                // -n numberA 432.12345

                theResult = theXalanTransformer.transform(argv[1], argv[2], argv[3]);
                if(theResult != 0)
                {
                    cerr << "UseStylesheetParam Error: \n" << theXalanTransformer.getLastError()
                         << endl
                         << endl;
                }
  
            // Clear the top-level parameters - Note: They are sticky across transformations.

                theXalanTransformer.clearStylesheetParams();

            // Destroy the parsed XML documents used as nodeset parameters.

                for (j=0; j < paramCount; j++)
                {
                    if (!strcmp(Parameter[j].parmType, "-d") && NodesetParam[j])
                        theXalanTransformer.destroyParsedSource(NodesetParam[j]);
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
