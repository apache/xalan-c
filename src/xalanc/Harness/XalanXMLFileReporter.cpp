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

#include "XalanXMLFileReporter.hpp" 



#include <cstdlib>
#include <ctime>



#include "xalanc/PlatformSupport/DOMStringHelper.hpp"
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include "xalanc/PlatformSupport/XalanTranscodingServices.hpp"
#include "xalanc/PlatformSupport/XalanUnicode.hpp"
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::fclose;
using std::fflush;
using std::fopen;
using std::fprintf;
using std::fputs;
using std::sprintf;
#endif



XalanOutputTranscoder*
makeNewUTF8Transcoder(MemoryManager&    theMemoryManager)
{
    XalanTranscodingServices::eCode     theCode;

    XalanOutputTranscoder* const    theTranscoder =
        XalanTranscodingServices::makeNewTranscoder(
            theMemoryManager,
            XalanTranscodingServices::s_utf8String,
            theCode,
            1024);

    if (theCode != XalanTranscodingServices::OK)
    {
        XalanDOMString  theBuffer(theMemoryManager);

        throw XalanTranscodingServices::MakeTranscoderException(
                theCode,
                XalanTranscodingServices::s_utf8String,
                theBuffer);
    }

    return theTranscoder;
}



XalanXMLFileReporter::XalanXMLFileReporter( MemoryManager& theManager, const XalanDOMString&  fileName) :
    OPT_FILENAME(theManager),
    ELEM_RESULTSFILE(theManager),
    ELEM_TESTFILE(theManager),
    ELEM_FILERESULT(theManager),
    ELEM_TESTCASE(theManager),
    ELEM_CASERESULT(theManager),
    ELEM_CHECKRESULT(theManager),
    ELEM_STATISTIC(theManager),
    ELEM_LONGVAL(theManager),
    ELEM_DOUBLEVAL(theManager),
    ELEM_MESSAGE(theManager),
    ELEM_ARBITRARY(theManager),
    ELEM_HASHTABLE(theManager),
    ELEM_HASHITEM(theManager),
    ATTR_LEVEL(theManager),
    ATTR_DESC(theManager),
    ATTR_TIME(theManager),
    ATTR_RESULT(theManager),
    ATTR_KEY(theManager),
    ATTR_FILENAME(theManager),
    LESS_THAN(theManager),
    GREATER_THAN(theManager),
    EQUALS_QUOTE(theManager),
    SPACE(theManager),
    QUOTE(theManager),
    QUOTE_SPACE(theManager),
    QUOTE_GREATER_THAN(theManager),
    QUOTE_SOLIDUS_GREATER_THAN(theManager),
    PASS(theManager),
    AMBG(theManager),
    ERRR(theManager),
    FAIL(theManager),
    LESS_THAN_SOLIDUS(theManager),
    XML_HEADER(theManager),
    REASON_EQUALS_QUOTE(theManager),
    TESTCASEINIT_HDR(theManager),
    TESTCASECLOSE_HDR(theManager),
    MESSAGE_HDR(theManager),
    STATISTIC_HDR(theManager),
    ARBITRARY_HDR(theManager),
    HASHTABLE_HDR(theManager),
    HASHITEM_HDR(theManager),
    CHECKPASS_HDR(theManager),
    CHECKAMBG_HDR(theManager),
    CHECKERRR_HDR(theManager),
    CHECKFAIL_HDR(theManager),
    CHECKFAIL_FTR(theManager),
    m_anyOutput(false),
    m_fileName(fileName, theManager),
    m_fileHandle(0),
    m_ready(false),
    m_error(false),
    m_flushOnCaseClose(true),
    m_transcoder(makeNewUTF8Transcoder(theManager)),
    m_buffer(theManager)
{
    assert(m_transcoder.get() != 0);

    if (m_fileName.empty() == false)
    {
        m_ready = initialize(theManager);
    }
}



XalanXMLFileReporter::~XalanXMLFileReporter()
{
}



bool
XalanXMLFileReporter::initialize(MemoryManager&     /* theManager */)
{       
    if (m_fileName.empty() == true)
    {
        // We don't have a valid file, so bail
        m_error = true;
        m_ready = false;
        fprintf(stderr, "XalanXMLFileReporter.initialize() ERROR: No file name specified");
    }
    else
    {
        // Transcode down the file name...
        TranscodeToLocalCodePage(m_fileName, m_buffer, true);

        const char* const       theTranscodedFileNamePointer = &*m_buffer.begin();

        // Create a file and ensure it has a place to live
        m_fileHandle = fopen(theTranscodedFileNamePointer, "w");
        if (m_fileHandle == 0)
        {
            // Couldn't create or find the directory for the file to live in, so bail
            m_error = true;
            m_ready = false;
            fprintf(stderr, "XalanXMLFileReporter.initialize() ERROR: unble to open file, %s", theTranscodedFileNamePointer);
            return(false);
        }
        else
        {
            m_ready = true;

            initStrings();

            startResultsFile();

            // fprintf(stderr, "DEBUG:XalanXMLFileReporter.initialize() complete with " + fileName);
        }
    }

    return m_ready;
}



bool
XalanXMLFileReporter::getFlushOnCaseClose()
{
    return(m_flushOnCaseClose);
}



const XalanDOMString& 
XalanXMLFileReporter::getFileName() const
{
    return(m_fileName);
}



void 
XalanXMLFileReporter::setFileName(const XalanDOMString& fileName)
{
    m_fileName = fileName;
}



bool 
XalanXMLFileReporter::checkError()
{
    // Ensure our underlying reporter, if one, is still OK
    if (m_fileHandle == 0)            
    {            
        m_error = true;        
    }
    return(m_error);
}



bool 
XalanXMLFileReporter::isReady() 
{
    // Ensure our underlying reporter, if one, is still OK
    if (m_fileHandle == 0) 
    {
        // NEEDSWORK: should we set m_ready = false in this case?
        //            errors in the PrintStream are not necessarily fatal
        m_error = true;
        m_ready = false;
    }
    return(m_ready);
}



void 
XalanXMLFileReporter::flush()
{
    if (isReady())
    {
        fflush(m_fileHandle);
    }
}



void 
XalanXMLFileReporter::close()
{
    fflush(m_fileHandle);
    if (isReady()) 
    {
        if (m_fileHandle != 0)
        {
            closeResultsFile();
            fclose(m_fileHandle);
        }
    }
    m_ready = false;
}



void 
XalanXMLFileReporter::logTestFileInit(const XalanDOMString& msg)
{
    XalanDOMString b(getMemoryManager());
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b += LESS_THAN;
        b += ELEM_TESTFILE;
        b += SPACE;
        b +=ATTR_DESC;
        b +=EQUALS_QUOTE;
        b +=escapestring(msg, buffer);
        b +=QUOTE_SPACE;
        b +=ATTR_TIME;
        b +=EQUALS_QUOTE;
        buffer.clear();
        b +=getDateTimeString(buffer);
        b +=QUOTE_GREATER_THAN;


        printToFile(b);
    }
}



void 
XalanXMLFileReporter::logTestFileClose(const XalanDOMString& /* msg */, const XalanDOMString& /* result */)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        b +=LESS_THAN_SOLIDUS;
        b +=ELEM_TESTFILE;
        b +=GREATER_THAN;

        printToFile(b);
    }
    flush();
}



void 
XalanXMLFileReporter::logTestCaseInit(const XalanDOMString& msg)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b +=TESTCASEINIT_HDR;
        b +=escapestring(msg, buffer);
        b +=QUOTE_GREATER_THAN;

        printToFile(b);
    }
}



void 
XalanXMLFileReporter::logTestCaseClose(const XalanDOMString& /* msg */, const XalanDOMString& /* result */)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        b +=LESS_THAN_SOLIDUS;
        b +=ELEM_TESTCASE;
        b +=GREATER_THAN;

        printToFile(b);
    }
    if (getFlushOnCaseClose())
    {
        flush();
    }
}



void 
XalanXMLFileReporter::logMessage(int level, const XalanDOMString& msg)
{
    char tmp[20];
    sprintf(tmp, "%d", level);

    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b +=LESS_THAN_SOLIDUS;
        b +=MESSAGE_HDR;
        b.append(tmp);
        b +=QUOTE_GREATER_THAN;

        printToFile(b);
        printToFile(escapestring(msg, buffer));
        b.clear();
        b +=LESS_THAN_SOLIDUS;
        b +=ELEM_MESSAGE;
        b +=GREATER_THAN;

        printToFile(b );
    }
}

void 
XalanXMLFileReporter::addMetricToAttrs(const char* desc, double theMetric, Hashtable& attrs)
{
    XalanDOMString  temp(getMemoryManager());

    NumberToDOMString(theMetric, temp);

    attrs.insert(XalanDOMString(desc,getMemoryManager()) ,temp);

}

void 
XalanXMLFileReporter::logElementWAttrs(int /* level */, const XalanDOMString& element, Hashtable& attrs, const XalanDOMString& msg)
{
    if (isReady() && !element.empty()&& !attrs.empty())
    {

        XalanDOMString b(getMemoryManager());
        b +=LESS_THAN;
        b +=element;
        b +=SPACE;
        printToFile( b);
    
        Hashtable::iterator theEnd = attrs.end();   
    
        for(Hashtable::iterator i = attrs.begin(); i != theEnd; ++i)
        {            
            b.erase();
            b +=(*i).first;
            b +=EQUALS_QUOTE;
            b +=(*i).second ;
            b +=QUOTE;
            printToFile(b);
        }

        printToFile(GREATER_THAN);
        if (msg.empty() != 0)
        {
            XalanDOMString theResult( getMemoryManager());
            printToFile(escapestring(msg,theResult ));
        }
        b.erase();
        b +=LESS_THAN_SOLIDUS;
        b +=element;
        b +=GREATER_THAN;
        printToFile(b);
    }
}

void 
XalanXMLFileReporter::logElement(const XalanDOMString& element, const XalanDOMString& msg)
{
    if (isReady() && !element.empty() && !msg.empty())
    {
        XalanDOMString theString( getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        theString += LESS_THAN;
        theString += element;
        theString +=GREATER_THAN;
        theString +=escapestring(msg, buffer);
        theString +=LESS_THAN_SOLIDUS;
        theString +=element;
        theString +=GREATER_THAN;

        printToFile(theString);
    }
}

void 
XalanXMLFileReporter::logStatistic (int level, long lVal, double dVal, const XalanDOMString& msg)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());
        char tmp[40];

        sprintf(tmp, "%d", level);
        b += STATISTIC_HDR;
        b += XalanDOMString(tmp, getMemoryManager());
        b += QUOTE_SPACE;
        b += ATTR_DESC;
        b += EQUALS_QUOTE;
        b += escapestring(msg,buffer);
        b += QUOTE_GREATER_THAN;

        printToFile(b);
        b.clear();
        
        sprintf(tmp, "%ld", lVal);

        b += LESS_THAN;
        b += ELEM_LONGVAL;
        b += GREATER_THAN;
        b += XalanDOMString(tmp, getMemoryManager());
        b += LESS_THAN_SOLIDUS;
        b += ELEM_LONGVAL;
        b += GREATER_THAN;

        printToFile(b);
        b.clear();

        sprintf(tmp, "%f", dVal);

        b += LESS_THAN;
        b += XalanDOMString(ELEM_DOUBLEVAL, getMemoryManager());
        b += GREATER_THAN;
        b += XalanDOMString(tmp, getMemoryManager());
        b += LESS_THAN_SOLIDUS;
        b += ELEM_DOUBLEVAL;
        b += GREATER_THAN;

        printToFile(b);
        b.clear(); 

        b += LESS_THAN_SOLIDUS;
        b += ELEM_STATISTIC;
        b += GREATER_THAN;

        printToFile(b);
        
    }
}



void 
XalanXMLFileReporter::logArbitraryMessage (int level, const XalanDOMString& msg)
{
    char tmp[20];
    sprintf(tmp, "%d", level);

    if (isReady())
    {   
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b += ARBITRARY_HDR;
        b += XalanDOMString(tmp,getMemoryManager());
        b += QUOTE_GREATER_THAN;

        printToFile(b);

        printToFile(escapestring(msg, buffer));

        b.clear();
        b +=LESS_THAN_SOLIDUS;
        b +=ELEM_ARBITRARY;
        b +=GREATER_THAN;

        printToFile(b);
    }
}

/*
void logHashtable (int level, Hashtable hash, XalanDOMString msg)
{
    if (isReady())
    {
        printToFile(HASHTABLE_HDR + level + QUOTE_SPACE + ATTR_DESC + EQUALS_QUOTE + msg + QUOTE_GREATER_THAN);
        if (hash == null)
        {
            printToFile(LESS_THAN + ELEM_HASHITEM + SPACE + ATTR_KEY + "=\"null\">");
            printToFile(LESS_THAN_SOLIDUS + ELEM_HASHITEM + GREATER_THAN);
        }
        try
        {
            for (Enumeration enum = hash.keys(); enum.hasMoreElements();)
            {
                Object key = enum.nextElement();
                // Ensure we'll have clean output by pre-fetching value before outputting anything
                XalanDOMString value = hash.get(key).tostring();
                printToFile(HASHITEM_HDR + key.tostring() + QUOTE_GREATER_THAN);
                printToFile(value);
                printToFile(LESS_THAN_SOLIDUS + ELEM_HASHITEM + GREATER_THAN);
            }
        } 
        catch (Exception e)
        {
            // No-op: should ensure we have clean output
        }
        printToFile(LESS_THAN_SOLIDUS + ELEM_HASHTABLE + GREATER_THAN);
    }
}
*/



void 
XalanXMLFileReporter::logCheckPass(const XalanDOMString& comment)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());
        b += CHECKPASS_HDR;
        b += escapestring(comment,buffer);
        b +=QUOTE_SOLIDUS_GREATER_THAN;

        printToFile(b);
    }
}


void 
XalanXMLFileReporter::logCheckFail(const XalanDOMString& comment)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b+= CHECKFAIL_HDR;
        b+=escapestring(comment, buffer);
        b+= QUOTE_SOLIDUS_GREATER_THAN;

        printToFile(b);

    }
}


void 
XalanXMLFileReporter::logCheckFail(const XalanDOMString& test, const Hashtable& actexp)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b+= CHECKFAIL_HDR;
        b+= escapestring(test, buffer);
        b+= QUOTE;

        printToFile(b);

        printToFile(GREATER_THAN);
        
        Hashtable::const_iterator aeEnd = actexp.end();
        for(Hashtable::const_iterator ii = actexp.begin(); ii != aeEnd; ++ii)
        {            
            logElement((*ii).first, (*ii).second);
        }

        printToFile(CHECKFAIL_FTR);
    }
}

void 
XalanXMLFileReporter::logCheckFail(const XalanDOMString& test, const Hashtable& attrs, const Hashtable& actexp)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());
        b+= CHECKFAIL_HDR;
        b+= escapestring(test, buffer);
        b+= QUOTE;
        printToFile(b);

        Hashtable::const_iterator fdEnd = attrs.end();    
        for(Hashtable::const_iterator i = attrs.begin(); i != fdEnd; ++i)
        {   
            b.clear();
            b += (*i).first;
            b += EQUALS_QUOTE;
            b += QUOTE;
            printToFile(b);
        }

        printToFile(GREATER_THAN);
        
        Hashtable::const_iterator aeEnd = actexp.end();
        for(Hashtable::const_iterator ii = actexp.begin(); ii != aeEnd; ++ii)
        {            
            logElement((*ii).first, (*ii).second);
        }

        printToFile(CHECKFAIL_FTR);
    }
}

void 
XalanXMLFileReporter::logCheckAmbiguous(const XalanDOMString& comment)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b += CHECKAMBG_HDR;
        b += escapestring(comment, buffer);
        b += QUOTE_SOLIDUS_GREATER_THAN;

        printToFile(b);
    }
}


void 
XalanXMLFileReporter::logErrorResult(const XalanDOMString& test, const XalanDOMString& reason)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b += CHECKFAIL_HDR;
        b += escapestring(test, buffer);
        b += QUOTE_SPACE;
        b += XalanDOMString(REASON_EQUALS_QUOTE, getMemoryManager());
        buffer.clear();
        b += escapestring(reason, buffer);
        b += QUOTE_SOLIDUS_GREATER_THAN;
        printToFile( b);

    }
}


void 
XalanXMLFileReporter::logCheckErr(const XalanDOMString& comment)
{
    if (isReady())
    {
        XalanDOMString b(getMemoryManager());
        XalanDOMString buffer(getMemoryManager());

        b +=  CHECKERRR_HDR;
        b += escapestring(comment, buffer);
        b += QUOTE_SOLIDUS_GREATER_THAN;
        printToFile(b);
    }
}



static const XalanDOMChar   theAmpersandString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_p,
    XalanUnicode::charSemicolon,
    0
};



static const XalanDOMChar   theApostropheString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_s,
    XalanUnicode::charSemicolon,
    0
};



static const XalanDOMChar   theLessThanString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSemicolon,
    0
};



static const XalanDOMChar   theGreaterThanString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSemicolon,
    0
};



static const XalanDOMChar   theQuoteString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_q,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSemicolon,
    0
};



XalanDOMString&
XalanXMLFileReporter::escapestring(const XalanDOMString&  s, XalanDOMString&      sb)
{

    const XalanDOMString::size_type     length = s.length();

    sb.reserve(length);

    for (XalanDOMString::size_type i = 0; i < length; i++)
    {
        const XalanDOMChar  ch = s[i];

        if (XalanUnicode::charLessThanSign == ch)
        {
            sb.append(theLessThanString);
        }
        else if (XalanUnicode::charGreaterThanSign == ch)
        {
            sb.append(theGreaterThanString);
        }
        else if (XalanUnicode::charAmpersand == ch) 
        {
            sb.append(theAmpersandString);
        }
        else if (XalanUnicode::charQuoteMark == ch) 
        {
            sb.append(theQuoteString);
        }
        else if (XalanUnicode::charApostrophe == ch) 
        {
            sb.append(theApostropheString);
        }
        else
        {
            sb.append(1, ch);
        }
    }

    return sb;
}



bool 
XalanXMLFileReporter::startResultsFile()
{
    if (isReady())
    {
        // Write out XML header and root test result element
        printToFile(XML_HEADER);

        // Note: this tag is closed in our .close() method, which the caller had better call!

        XalanDOMString b(getMemoryManager());
        b += LESS_THAN;
        b += ELEM_RESULTSFILE;
        b += SPACE;
        b += ATTR_FILENAME;
        b += EQUALS_QUOTE;
        b += m_fileName;
        b += QUOTE_GREATER_THAN;

        printToFile(b);

        return true;
    }
    else
    {
        return false;
    }
}



bool 
XalanXMLFileReporter::closeResultsFile()
{
    if (isReady() == false)
    {
        return false;
    }
    else
    {        
        XalanDOMString b(getMemoryManager());
        b += LESS_THAN_SOLIDUS;
        b += ELEM_RESULTSFILE;
        b += GREATER_THAN;

        printToFile(b);
        return true;
    }
}


bool 
XalanXMLFileReporter::printToFile(const XalanDOMString&  output) 
{
        const XalanDOMString::size_type     theLength =
            output.size();

    if (isReady() == false)
    {
        return false;
    }
    else if (theLength == 0)
    {
        return true;
    }
    else
    {
        m_buffer.resize((theLength * 4) + 1);

        typedef XalanOutputTranscoder::size_type        size_type;
        typedef XalanOutputTranscoder::XalanXMLByte     XalanXMLByte;

        XalanOutputTranscoder::size_type    theCharsTranscoded;
        XalanOutputTranscoder::size_type    theTargetBytesUsed;

        const XalanOutputTranscoder::eCode  theCode =
            m_transcoder->transcode(
                output.c_str(),
                theLength,
                reinterpret_cast<XalanXMLByte*>(&*m_buffer.begin()),
                m_buffer.size(),
                theCharsTranscoded,
                theTargetBytesUsed);

        if (theCode != XalanTranscodingServices::OK ||
            theCharsTranscoded != theLength)
        {
            fputs("Error transcoding text.", m_fileHandle);
        }
        else 
        {
            m_buffer[theTargetBytesUsed] = 0;

            fputs(&*m_buffer.begin(), m_fileHandle);
        }

        fputs("\n", m_fileHandle);

        return true;
    }
}



XalanDOMString &
XalanXMLFileReporter::getDateTimeString(XalanDOMString& theResult) 
{
    struct tm *tmNow;
    time_t time_tNow;

    time(&time_tNow);     
    tmNow = localtime(&time_tNow);
    
    const char* const   theTime = asctime(tmNow);

    theResult.assign(theTime, XalanDOMString::length(theTime) - 1);

    return theResult;
}



void
XalanXMLFileReporter::initStrings()
{
    OPT_FILENAME = XalanDOMString("filename", getMemoryManager());
    ELEM_RESULTSFILE = XalanDOMString("resultsfile", getMemoryManager());
    ELEM_TESTFILE = XalanDOMString("testfile", getMemoryManager());
    ELEM_FILERESULT = XalanDOMString("fileresult", getMemoryManager());
    ELEM_TESTCASE = XalanDOMString("Test_Dir", getMemoryManager());
    ELEM_CASERESULT = XalanDOMString("Dir-result", getMemoryManager());
    ELEM_CHECKRESULT = XalanDOMString("Testcase", getMemoryManager());
    ELEM_STATISTIC = XalanDOMString("statistic", getMemoryManager());
    ELEM_LONGVAL = XalanDOMString("longval", getMemoryManager());
    ELEM_DOUBLEVAL = XalanDOMString("doubleval", getMemoryManager());
    ELEM_MESSAGE = XalanDOMString("message", getMemoryManager());
    ELEM_ARBITRARY = XalanDOMString("arbitrary", getMemoryManager());
    ELEM_HASHTABLE = XalanDOMString("hashtable", getMemoryManager());
    ELEM_HASHITEM = XalanDOMString("hashitem", getMemoryManager());
    ATTR_LEVEL = XalanDOMString("level", getMemoryManager());
    ATTR_DESC = XalanDOMString("desc", getMemoryManager());
    ATTR_TIME = XalanDOMString("time", getMemoryManager());
    ATTR_RESULT = XalanDOMString("result", getMemoryManager());
    ATTR_KEY = XalanDOMString("key", getMemoryManager());
    ATTR_FILENAME = OPT_FILENAME;
    LESS_THAN = XalanDOMString("<", getMemoryManager());
    GREATER_THAN = XalanDOMString(">", getMemoryManager());
    EQUALS_QUOTE = XalanDOMString("=\"", getMemoryManager());
    SPACE = XalanDOMString(" ", getMemoryManager());
    QUOTE = XalanDOMString("\"", getMemoryManager());
    QUOTE_SPACE = XalanDOMString("\" ", getMemoryManager());
    QUOTE_GREATER_THAN = XalanDOMString("\">", getMemoryManager());
    QUOTE_SOLIDUS_GREATER_THAN = XalanDOMString("\"/>", getMemoryManager());
    PASS = XalanDOMString("PASS", getMemoryManager());
    AMBG = XalanDOMString("AMBG", getMemoryManager());
    ERRR = XalanDOMString("ERRR", getMemoryManager());
    FAIL = XalanDOMString("FAIL", getMemoryManager());
    LESS_THAN_SOLIDUS = XalanDOMString("</", getMemoryManager());
    XML_HEADER = XalanDOMString("<?xml version=\"1.0\"?>", getMemoryManager());
    REASON_EQUALS_QUOTE = XalanDOMString("reason=\"", getMemoryManager());

    TESTCASEINIT_HDR = LESS_THAN;
    TESTCASEINIT_HDR += ELEM_TESTCASE ;  
    TESTCASEINIT_HDR +=  SPACE;
    TESTCASEINIT_HDR += ATTR_DESC;
    TESTCASEINIT_HDR += EQUALS_QUOTE;

    TESTCASECLOSE_HDR = LESS_THAN;
    TESTCASECLOSE_HDR +=  ELEM_CASERESULT;
    TESTCASECLOSE_HDR += SPACE;   
    TESTCASECLOSE_HDR += ATTR_DESC;
    TESTCASECLOSE_HDR += EQUALS_QUOTE;

    MESSAGE_HDR = LESS_THAN;
    MESSAGE_HDR += ELEM_MESSAGE;    
    MESSAGE_HDR += SPACE;    
    MESSAGE_HDR += ATTR_LEVEL;    
    MESSAGE_HDR += EQUALS_QUOTE;

    STATISTIC_HDR +=LESS_THAN;
    STATISTIC_HDR +=ELEM_STATISTIC;
    STATISTIC_HDR +=SPACE;
    STATISTIC_HDR +=ATTR_LEVEL;
    STATISTIC_HDR +=EQUALS_QUOTE;

    ARBITRARY_HDR += LESS_THAN;
    ARBITRARY_HDR += ELEM_ARBITRARY;
    ARBITRARY_HDR += SPACE;
    ARBITRARY_HDR += ATTR_LEVEL;
    ARBITRARY_HDR += EQUALS_QUOTE;

    HASHTABLE_HDR += LESS_THAN;
    HASHTABLE_HDR += ELEM_HASHTABLE;
    HASHTABLE_HDR += SPACE;
    HASHTABLE_HDR += ATTR_LEVEL;
    HASHTABLE_HDR += EQUALS_QUOTE;
 
    HASHITEM_HDR += LESS_THAN;
    HASHITEM_HDR += ELEM_HASHITEM;
    HASHITEM_HDR += SPACE;
    HASHITEM_HDR += ATTR_KEY;
    HASHITEM_HDR += EQUALS_QUOTE;

    XalanDOMString prefix(getMemoryManager()), suffix(getMemoryManager());

    prefix += LESS_THAN;
    prefix += ELEM_CHECKRESULT;
    prefix += SPACE;
    prefix += ATTR_RESULT;
    prefix += EQUALS_QUOTE;

    suffix += QUOTE_SPACE;
    suffix += ATTR_DESC;
    suffix += EQUALS_QUOTE;

    CHECKPASS_HDR += prefix;
    CHECKPASS_HDR += PASS;
    CHECKPASS_HDR += suffix;

    CHECKAMBG_HDR += prefix;
    CHECKAMBG_HDR += AMBG;
    CHECKAMBG_HDR += suffix;

    CHECKERRR_HDR += prefix;
    CHECKERRR_HDR += ERRR;
    CHECKERRR_HDR += suffix;

    CHECKFAIL_HDR += prefix;
    CHECKFAIL_HDR += FAIL;
    CHECKFAIL_HDR += suffix;

    CHECKFAIL_FTR += LESS_THAN_SOLIDUS;
    CHECKFAIL_FTR += ELEM_CHECKRESULT;   
    CHECKFAIL_FTR += GREATER_THAN;     

}



XALAN_CPP_NAMESPACE_END
