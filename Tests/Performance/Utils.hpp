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
#if !defined(UTILS_HEADER_GUARD_1357924680)
#define UTILS_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <fstream>


#include <xalanc/Harness/XalanFileUtility.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



using xalanc::XalanDOMChar;
using xalanc::XalanDOMString;
using xalanc::XalanFileUtility;



/**
 *  Checks if a file exists
 */
bool checkFileExists(const XalanDOMString& fileName);



/**
 * Gets the current working directory
 */
XalanDOMString getWorkingDirectory();



/**
 * Gets the system path seperator
 */
const XalanDOMChar* getPathSep();



/**
 * Reads file into a stream
 */
void
fileToStream(
            const XalanDOMString&                           fileName,
            std::istringstream&              resultStream);


/**
 * Copies a file
 */
void
copyFile(
            const XalanDOMString&   destFile,
            const XalanDOMString&   sourceFile);

        

#endif //UTILS_HEADER_GUARD_1357924680


