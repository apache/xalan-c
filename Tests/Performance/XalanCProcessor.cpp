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



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xalanc/XMLSupport/FormatterToNull.hpp>
#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>



#include "XalanCProcessor.hpp"



XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XalanMemMgrs)


typedef XalanCProcessor::CompiledStylesheetType CompiledStylesheetType;
typedef XalanCProcessor::ParsedInputSourceType        ParsedInputSourceType;
typedef XalanCProcessor::ResultTargetType       ResultTargetType;

typedef XalanCProcessor::ParseOptionType        ParseOptionType;
typedef XalanCProcessor::ResultTarget           ResultTarget;





XalanCProcessor::XalanCProcessor() :
    m_name("Xalan"),
    m_resultOptionsMap(XalanMemMgrs::getDefaultXercesMemMgr()),
    m_transformer(0),
    m_nullResult(),
    m_xalanSourceTreeResult(),
    m_xercesDOMResult(),
    m_fileResult(),
    m_streamResult()

{
}

XalanCProcessor::~XalanCProcessor()
{
}



void
XalanCProcessor::init()
{
    m_transformer = new XalanTransformer();

    m_resultOptionsMap[XalanDOMString("")] = &m_fileResult;
    m_resultOptionsMap[XalanDOMString("file")] = &m_fileResult;
    m_resultOptionsMap[XalanDOMString("xst")] = &m_xalanSourceTreeResult;
    m_resultOptionsMap[XalanDOMString("xerces-dom")] = &m_xercesDOMResult;
    m_resultOptionsMap[XalanDOMString("null")] = &m_nullResult;
    m_resultOptionsMap[XalanDOMString("stream")] = &m_streamResult;
}



void
XalanCProcessor::terminate()
{
     delete m_transformer;
}



ParseOptionType  
XalanCProcessor::getParseOption(const XalanNode* option)
{
    if (option != 0 
        && option->getNodeType() == XalanNode::TEXT_NODE)
    {
        if (option->getNodeValue() == XalanDOMString("") 
            || option->getNodeValue() == XalanDOMString("xst"))
        {
            return eXalanSourceTree;
        }
        else if (option->getNodeValue() == XalanDOMString("xerces-dom"))
        {
            return eXercesDOM;
        }
        else 
        {
            return eInvalid;
        }
    }
    else
    {
        return eInvalid;
    }
}

 

ResultTarget*
XalanCProcessor::getResultOption(
    const XalanDOMString& fileName,
    const XalanNode* option)
{
    if (option != 0 
        && option->getNodeType() == XalanNode::TEXT_NODE
        && m_resultOptionsMap.find(option->getNodeValue()) != m_resultOptionsMap.end())
    {
        return m_resultOptionsMap[option->getNodeValue()]->clone(fileName);
    }
    else
    {
        return m_resultOptionsMap[XalanDOMString("")]->clone(fileName);
    }
}



CompiledStylesheetType 
XalanCProcessor::compileStylesheet(
    const XalanDOMString& fileName,
    const XalanNode* /* compileOptions */)
{     
    CompiledStylesheetType theCompiledStylesheet= 0;
    
    m_transformer->compileStylesheet(fileName, theCompiledStylesheet);
    
    return theCompiledStylesheet;
}



CompiledStylesheetType 
XalanCProcessor::compileStylesheet(
    StreamType& stream,
    const XalanNode* /* compileOptions */)
{
    CompiledStylesheetType theCompiledStylesheet = 0;
    m_transformer->compileStylesheet(stream, theCompiledStylesheet);
    return theCompiledStylesheet;
}



void
XalanCProcessor::releaseStylesheet(CompiledStylesheetType stylesheet)
{
    m_transformer->destroyStylesheet(stylesheet);
}

    

ParsedInputSourceType
XalanCProcessor::parseInputSource(
    const XalanDOMString& fileName,
    const XalanNode* parseOptions )
{
    ParsedInputSourceType theParsedSource = 0;
    
    XSLTInputSource inpFileName(fileName, m_transformer->getMemoryManager());

    switch(getParseOption(parseOptions))
    {
        case eXercesDOM:
            {
                m_transformer->parseSource(inpFileName, theParsedSource, true);
                
                break;
            }
        case eXalanSourceTree:
        default:
            {
                m_transformer->parseSource(inpFileName, theParsedSource);
                
                break;
            }
    }
        
    return theParsedSource;
}



ParsedInputSourceType 
XalanCProcessor::parseInputSource(
    StreamType& stream,
    const XalanNode* parseOptions)
{
    ParsedInputSourceType theParsedSource = 0;
    switch(getParseOption(parseOptions))
    {
        case eXercesDOM:
            m_transformer->parseSource(stream, theParsedSource, true);
            break;
        case eXalanSourceTree:
        default:
            m_transformer->parseSource(stream, theParsedSource);
            break;
    }
        
    return theParsedSource;

}



void
XalanCProcessor::releaseInputSource(ParsedInputSourceType inputSource)
{
    m_transformer->destroyParsedSource(inputSource);
}



ResultTargetType
XalanCProcessor::createResultTarget(
    const XalanDOMString& resultFileName,
    const XalanNode* resultOptions)
{
    return getResultOption(resultFileName, resultOptions);
}



void
XalanCProcessor::releaseResultTarget(ResultTargetType resultTarget)
{
    delete resultTarget;
}




void 
XalanCProcessor::transform(
    const CompiledStylesheetType&   compiledStylesheet,
    const ParsedInputSourceType&    parsedInputSourceType,
    const ResultTargetType&         resultTargetType,
    const XalanNode* /* transformOptions */)
{
    m_transformer->transform(*parsedInputSourceType, compiledStylesheet, resultTargetType->resultTarget);
}



