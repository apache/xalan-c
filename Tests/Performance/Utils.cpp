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



// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <stdio.h>

#if !defined(XALAN_CLASSIC_IOSTREAMS)
#include <sstream>
#endif



#if defined(XALAN_WINDOWS)
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif



#include <xalanc/Include/XalanMemoryManagement.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "Utils.hpp"



XALAN_USING_XALAN(XalanMemMgrs)
XALAN_USING_XALAN(CharVectorType)
XALAN_USING_XALAN(TranscodeToLocalCodePage)
XALAN_USING_XALAN(c_str)



bool checkFileExists(const XalanDOMString& fileName)
{
    CharVectorType     theResult(XalanMemMgrs::getDefaultXercesMemMgr());
    TranscodeToLocalCodePage(fileName, theResult, true);

    FILE* fileHandle  = fopen(c_str(theResult), "r");

    if (fileHandle != 0)
    {
        fclose(fileHandle);
        return true;
    }
    else
    {
        return false;
    }
}



XalanDOMString getWorkingDirectory()
{
    char path[4096];
    getcwd(path, 4096);
    return XalanDOMString(path);
}



const XalanDOMChar* getPathSep()
{
    return XalanFileUtility::s_pathSep;
}



void fileToStream(
            const XalanDOMString&               fileName,
#if defined(XALAN_CLASSIC_IOSTREAMS)
            CharVectorType&                     resultStream)
#else
            XALAN_STD_QUALIFIER istringstream&  resultStream)
#endif
{
    CharVectorType resultFileName;
    fileName.transcode(resultFileName);

#if !defined(XALAN_NO_STD_NAMESPACE)
    using std::ifstream;
    using std::ostringstream;
#endif

    ifstream resultFile(c_str(resultFileName));

#if !defined(XALAN_CLASSIC_IOSTREAMS)
    ostringstream fileOutputStream;
#endif

    char ch; 

    while(resultFile.get(ch))
    {
#if defined(XALAN_CLASSIC_IOSTREAMS)
        resultStream.push_back(ch);
#else
        fileOutputStream.put(ch);
#endif
    }

#if defined(XALAN_CLASSIC_IOSTREAMS)
    resultStream.push_back('\0');
#else
    resultStream.str(fileOutputStream.str());
#endif
}



void copyFile(
        const XalanDOMString&   destFile,
        const XalanDOMString&   sourceFile)
{
    CharVectorType sourceFileName;
    sourceFile.transcode(sourceFileName);

#if !defined(XALAN_NO_STD_NAMESPACE)
    using std::ifstream;
    using std::ofstream;
#endif

    ifstream sourceFileStream(c_str(sourceFileName));

    CharVectorType destFileName;
    destFile.transcode(destFileName);
    ofstream destFileStream(c_str(destFileName));

    char ch;

    while (sourceFileStream.get(ch))
    {
        destFileStream.put(ch);
    }

    sourceFileStream.close();
    destFileStream.close();
}




