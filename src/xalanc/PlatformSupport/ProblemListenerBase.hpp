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
#if !defined(XALAN_PROBLEMLISTENERBASE_HEADER_GUARD)
#define XALAN_PROBLEMLISTENERBASE_HEADER_GUARD



// Base include file.  Must be first.
#include "xalanc/PlatformSupport/PlatformSupportDefinitions.hpp"



#include "LocalMsgIndex.hpp"



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(Locator)


class XalanDOMString;
class XalanNode;
class PrintWriter;



/**
 * This is the abstract class that is used when reporting a problem
 * some kind, that requires a message, an error or a warning.
 */
class XALAN_PLATFORMSUPPORT_EXPORT ProblemListenerBase
{
public:

    // Sources of problem.  The upper case versions
    // of these enums are deprecated.
    enum eSource { eXMLPARSER      = 0,
                   eXMLParser      = 0,
                   eXSLPROCESSOR   = 1,
                   eXSLTProcessor  = 1,
                   eXPATH          = 2,
                   eXPath          = 2,
                   eSourceCount };

    // A typedef for compatibility.
    typedef eSource    eProblemSource;

    // Severity of problem.  The upper case versions
    // of these enums are deprecated.
    enum eClassification {  eMESSAGE    = 0,
                            eMessage    = 0,
                            eWARNING    = 1,
                            eWarning    = 1,
                            eERROR      = 2,
                            eError      = 2,
                            eClassificationCount };

    ProblemListenerBase();

    virtual
    ~ProblemListenerBase();

    /**
     * Function that is called when a problem event occurs.
     *
     * @param source Either eXMLParser, eXSLProcessor, or eXPATH.
     * @param classification Either eMessage, eWarning, or eError.
     * @param locator The current Locator instance for the stylesheet.  Maybe be a null pointer.
     * @param sourceNode The current source node, if any.
     * @param msg The error message.
     */
    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode) = 0;

    /**
     * Function that is called when a problem event occurs.  This version
     * assumes location information is already formatted into the message.
     * 
     * @param   source         either eXMLPARSER, eXSLPROCESSOR, or eXPATH
     * @param   classification either eMESSAGE, eERROR or eWARNING
     * @param   msg            string message explaining the problem.
     */
    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode) = 0;

    /**
     * Function to format a problem call to a PrintWriter instance.
     * This is a bare-bones implementation that uses localized strings.
     *
     * @param pw The PrintWriter instance to use.
     * @param source Either eXMLParser, eXSLProcessor, or eXPATH.
     * @param classification Either eMessage, eWarning, or eError.
     * @param locator The current Locator instance for the stylesheet.  Maybe be a null pointer.
     * @param sourceNode The current source node, if any.
     * @param msg The error message.
     */
    static void
    defaultFormat(
            PrintWriter&            pw,
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode);

    /**
     * Function to format a problem call to a PrintWriter instance.
     * This is a bare-bones implementation that uses localized strings.
     *
     * @param pw The PrintWriter instance to use.
     * @param   source         either eXMLPARSER, eXSLPROCESSOR, or eXPATH
     * @param   classification either eMESSAGE, eERROR or eWARNING
     * @param   msg            string message explaining the problem.
     */
    static void
    defaultFormat(
            PrintWriter&            pw,
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode);

protected:

    static const XalanMessages::Codes s_messageCodes[eSourceCount][eClassificationCount];
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_PROBLEMLISTENER_HEADER_GUARD
