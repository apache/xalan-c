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

#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#include <cstdio>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::FILE;
using std::fclose;
using std::fflush;
using std::fopen;
using std::fwrite;
#endif



// This is a simple class that illustrates how XalanTransformer's "callback" API
// is used.  This example just abstracts writing data to a FILE*, but other
// actions are possible.
class CallbackHandler
{
public:

    CallbackHandler(FILE*   theFile) :
        m_file(theFile)
    {
        assert(m_file != 0);
    }

    CallbackSizeType
    write(
            const char*         theData,
            CallbackSizeType    theLength)
    {
        return fwrite(theData, sizeof(char), theLength, m_file);
    }

    void
    flush()
    {
        fflush(m_file);
    }

private:

    FILE* const     m_file;
};



// These functions need to have C linkage, so surround them with an extern C block...
extern "C"
{

// This is the write callback function, which casts the handle
// to the appropriate type, then calls the write() member function
// on the CallbackHandler class.
CallbackSizeType
writeCallback(
            const char*         theData,
            CallbackSizeType    theLength,
            void*               theHandle)
{
    return reinterpret_cast<CallbackHandler*>(theHandle)->write(theData, theLength);
}

// This is the flush callback function, which casts the handle
// to the appropriate type, then calls the flush() member function
// on the CallbackHandler class.
void
flushCallback(void* theHandle)
{
    reinterpret_cast<CallbackHandler*>(theHandle)->flush();
}

}



int
doTransform(
            const char*     theXMLFile,
            const char*     theXSLFile,
            FILE*           theOutputFile)
{
    XALAN_USING_STD(cerr)
    XALAN_USING_STD(endl)

    XALAN_USING_XALAN(XalanTransformer)

    // Create a XalanTransformer...
    XalanTransformer    theXalanTransformer;

    // Create an instance of the class we wrote to handle
    // the callbacks...
    CallbackHandler     theHandler(theOutputFile);

    // Do the transform...
    const int   theResult = theXalanTransformer.transform(
                    theXMLFile,
                    theXSLFile,
                    &theHandler,
                    writeCallback,
                    flushCallback);
        
    if(theResult != 0)
    {
        cerr << "XalanError: " << theXalanTransformer.getLastError() << endl;
    }

    return theResult;
}



int
main(
            int     argc,
            char*   argv[])
{
    XALAN_USING_STD(cerr)
    XALAN_USING_STD(cout)
    XALAN_USING_STD(endl)

    if (argc < 3 || argc > 4)
    {
        cerr << "Usage: XalanTransformerCallback XMLFileName XSLFileName [OutFileName]" << endl;

        return -1;
    }

    int     theResult = -1;

    try
    {
        XALAN_USING_XERCES(XMLPlatformUtils)

        XALAN_USING_XALAN(XalanTransformer)

        // Call the static initializer for Xerces.
        XMLPlatformUtils::Initialize();

        // Initialize Xalan.
        XalanTransformer::initialize();

        if (argc == 3)
        {
            // No output file, so use stdout...
            theResult = doTransform(argv[1], argv[2], stdout);
        }
        else
        {
            // Output file specified, so try to open it...
            FILE* const theOutputFile = fopen(argv[3], "w");

            if (theOutputFile == 0)
            {
                cerr << "Error: " << "Unable to open output file " << argv[3] << endl;
            }
            else
            {
                theResult = doTransform(argv[1], argv[2], theOutputFile);

                fclose(theOutputFile);
            }
        }

        // Terminate Xalan...
        XalanTransformer::terminate();

        // Terminate Xerces...
        XMLPlatformUtils::Terminate();

        // Clean up the ICU, if it's integrated...
        XalanTransformer::ICUCleanUp();
    }
    catch(...)
    {
        cerr << "An unknown error occurred!" << endl;
    }

    return theResult;
}
