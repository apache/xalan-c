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
#if !defined(XALANCRPROCESSOR_HEADER_GUARD_1357924680)
#define XALANCRPROCESSOR_HEADER_GUARD_1357924680

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <strstream.h>
#else
#include <sstream>
#endif



#include <xercesc/dom/DOMDocument.hpp>



#include <xalanc/Include/XalanMap.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>




#include <xalanc/XMLSupport/FormatterToNull.hpp>



#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_USING_XERCES(DOMDocument);
XALAN_CPP_NAMESPACE_USE



/**
 * Implementation of the test interface for XalanC 1.x
 *
 */
class XalanCProcessor
{
public:

    XalanDOMString m_name;

    typedef const XalanCompiledStylesheet*  CompiledStylesheetType;
    typedef const XalanParsedSource*            ParsedInputSourceType;

    struct ResultTarget {
        virtual
    ~ ResultTarget(){}

        XSLTResultTarget resultTarget;

        virtual ResultTarget* clone(const XalanDOMString&) = 0;
        virtual void reset() = 0;
    };

    typedef ResultTarget*           ResultTargetType;

    typedef XSLTInputSource::StreamType StreamType;

     XalanCProcessor();
    ~XalanCProcessor();



    /**
     * Initializes the processor interface.
     * Assumes static processor initialization functions are already completed.
     */
    void
    init();



    /**
     * Terminates the processor interface.
     */
    void
    terminate();



    /**
     *  Returns the name of the processor implementation.
     *  @returns name of the processor
     */
    XalanDOMString& getName() { return m_name;}



    /** 
     *  Compiles the specified stylesheet.
     *  Xalan interface does not support any options.
     *  @param fileName stylesheet to compile
     *  @param compileOptions (none for this interface)
     *  @returns compiled stylesheet
     */
    CompiledStylesheetType 
    compileStylesheet(
        const XalanDOMString& fileName,
        const XalanNode* compileOptions = 0);



    /** 
     *  Compiles the specified stylesheet.
     *  Xalan interface does not support any options.
     *  @param stream stylesheet to compile
     *  @param compileOptions (none for this interface)
     *  @returns compiled stylesheet
     */
    CompiledStylesheetType 
    compileStylesheet(
        StreamType& stream,
        const XalanNode* compileOptions = 0);


    /**
     *  Release stylesheet object
     *  @param stylesheet object to release
     */
    void
    releaseStylesheet(CompiledStylesheetType stylesheet);
    


    /**
     * Parses an input document.
     * Parse options, a text node containing:<ul>
     * <li><b>xst</b>: parse to XalanSourceTree (default)
     * <li><b>xerces-dom</b>: parse to XercesDOM</ul>
     * @param fileName input document to parse.
     * @param parseOptions  options to the parser.
     * @returns parsed document
     */
    ParsedInputSourceType
    parseInputSource(
        const XalanDOMString& fileName,
        const XalanNode* parseOptions = 0);



    /**
     * Parses an input document.
     * Parse options, a text node containing:<ul>
     * <li><b>xst</b>: parse to XalanSourceTree (default)
     * <li><b>xerces-dom</b>: parse to XercesDOM</ul>
     * @param stream input document to parse.
     * @param parseOptions  options to the parser.
     * @returns parsed document
     */
    ParsedInputSourceType 
    parseInputSource(
        StreamType& stream,
        const XalanNode* parseOptions = 0);



    /**
     * Releases and parsed input source
     * @param inputSource source to release
     */
    void
    releaseInputSource(ParsedInputSourceType inputSource);



    /**
     * Creates a result target.
     * Result target options, a text node containing:<ul>
     * <li><b>file</b>: result to a file (default)</li>
     * <li><b>xst</b>: result to a XalanSourceTree</li>
     * <li><b>xerces-dom</b>: result to XercesDOM</li>
     * <li><b>null</b>:  generate no result</li>
     * <li><b>stream</b>: result to a stream (string)</li></ul>
     * @param resultFileName file to write result to, if any
     * @param resultOptions  options to create result
     * @return result target
     */
    ResultTargetType
    createResultTarget(
        const XalanDOMString& resultFileName,
        const XalanNode* resultOptions = 0);


    
    /**
     * Releases result target
     * @param resultTarget target to release
     */
    void
    releaseResultTarget(ResultTargetType resultTarget);



    /**
     * Executes a transform
     * @param compileStyle stylesheet
     * @param parsedInputSourceType parsed document
     * @param resultTargetType result target
     * @param transformOptions (none for this interface)
     */
    void transform(
        const CompiledStylesheetType&   compiledStylesheet,
        const ParsedInputSourceType&    parsedInputSourceType,
        const ResultTargetType&         resultTargetType,
        const XalanNode* transformOptions = 0);

    typedef enum {eInvalid, eXalanSourceTree, eXercesDOM} ParseOptionType;

protected:

    ParseOptionType  getParseOption(const XalanNode* option);

    typedef XalanMap<XalanDOMString, ResultTarget*>  ResultsMapType;

    ResultTarget* getResultOption(
        const XalanDOMString& fileName,
        const XalanNode* option);    

    ResultsMapType      m_resultOptionsMap;

    XalanTransformer    *m_transformer;

    
    struct NullResult : public ResultTarget
    {
        NullResult()
        {
            resultTarget.setFormatterListener(&formatterToNull);
        }
        
        ResultTarget * 
        clone(const XalanDOMString&) { return new NullResult;}
        
        void reset() {}

        FormatterToNull formatterToNull;
    };

    NullResult m_nullResult;

    struct XalanSourceTreeResult : public ResultTarget
    {
        XalanSourceTreeResult() :
            document(XalanMemMgrs::getDefaultXercesMemMgr())
        {
            formatterToSourceTree.setDocument(&document);
            resultTarget.setFormatterListener(&formatterToSourceTree);
        }

        ResultTarget * 
        clone(const XalanDOMString&) { return new XalanSourceTreeResult;}

        void reset() { formatterToSourceTree.resetDocument();}


        FormatterToSourceTree   formatterToSourceTree;
        XalanSourceTreeDocument document;
    };

    XalanSourceTreeResult m_xalanSourceTreeResult;

    struct XercesDOMResult : public ResultTarget
    {
        XercesDOMResult() :
                parserLiaison(),
                document(parserLiaison.createDOMFactory()),
                formatterToXercesDOM(document, 0)
        {
        }

        ResultTarget *
        clone(const XalanDOMString&) { return new XercesDOMResult;}

        void reset() { formatterToXercesDOM.resetDocument();}

        XercesParserLiaison     parserLiaison;
        DOMDocument*            document;
        FormatterToXercesDOM    formatterToXercesDOM;

    };

    XercesDOMResult m_xercesDOMResult;

    struct FileResult : public ResultTarget
    {
        FileResult(const XalanDOMString & fileName = XalanDOMString(""))
        {
            resultTarget.setFileName(fileName);
        }

        ResultTarget *
        clone(const XalanDOMString& fileName) { return new FileResult(fileName);}

        void reset() {}

    };

    FileResult  m_fileResult;

    struct StreamResult : public XalanCProcessor::ResultTarget
    {
        StreamResult()
        {
            resultTarget.setByteStream(&stringstream);
        }

        ResultTarget *
        clone(const XalanDOMString&) { return new StreamResult;}

        void reset() { stringstream.flush();}

#if defined(XALAN_CLASSIC_IOSTREAMS)
        ostrstream                          stringstream;
#else
        XALAN_STD_QUALIFIER ostringstream   stringstream;
#endif
    };

    StreamResult    m_streamResult;

};

#endif // XALANCRPROCESSOR_HEADER_GUARD_1357924680


