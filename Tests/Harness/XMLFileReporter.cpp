/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include "XMLFileReporter.hpp" 



#include <cstdlib>
#include <ctime>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XMLFileReporter::XMLFileReporter(const XalanDOMString&	fileName) :
	OPT_FILENAME(),
	ELEM_RESULTSFILE(),
	ELEM_TESTFILE(),
	ELEM_FILERESULT(),
	ELEM_TESTCASE(),
	ELEM_CASERESULT(),
	ELEM_CHECKRESULT(),
	ELEM_STATISTIC(),
	ELEM_LONGVAL(),
	ELEM_DOUBLEVAL(),
	ELEM_MESSAGE(),
	ELEM_ARBITRARY(),
	ELEM_HASHTABLE(),
	ELEM_HASHITEM(),
	ATTR_LEVEL(),
	ATTR_DESC(),
	ATTR_TIME(),
	ATTR_RESULT(),
	ATTR_KEY(),
	ATTR_FILENAME(),
	LESS_THAN(),
	GREATER_THAN(),
	EQUALS_QUOTE(),
	SPACE(),
	QUOTE(),
	QUOTE_SPACE(),
	QUOTE_GREATER_THAN(),
	QUOTE_SOLIDUS_GREATER_THAN(),
	PASS(),
	AMBG(),
	ERRR(),
	FAIL(),
	LESS_THAN_SOLIDUS(),
	XML_HEADER(),
	REASON_EQUALS_QUOTE(),
	TESTCASEINIT_HDR(),
	TESTCASECLOSE_HDR(),
	MESSAGE_HDR(),
	STATISTIC_HDR(),
	ARBITRARY_HDR(),
	HASHTABLE_HDR(),
	HASHITEM_HDR(),
	CHECKPASS_HDR(),
	CHECKAMBG_HDR(),
	CHECKERRR_HDR(),
	CHECKFAIL_HDR(),
	CHECKFAIL_FTR(),
	m_anyOutput(false),
	m_fileName(fileName),
	m_fileHandle(0),
	m_ready(false),
	m_error(false),
	m_flushOnCaseClose(true)
{
	if (m_fileName.empty() == false)
	{
		m_ready = initialize();
	}
}

bool
XMLFileReporter::initialize()
{   	
    if (m_fileName.empty() == true)
    {
        // We don't have a valid file, so bail
        m_error = true;
        m_ready = false;
        fprintf(stderr, "XMLFileReporter.initialize() ERROR: No file name specified");
    }
	else
	{
		// Transcode down the file name...
		const CharVectorType	theTranscodedFileName(m_fileName.transcode());
		const char* const		theTranscodedFileNamePointer = &theTranscodedFileName.front();

		// Create a file and ensure it has a place to live
		m_fileHandle = fopen(theTranscodedFileNamePointer, "w");
		if (m_fileHandle == 0)
		{
			// Couldn't create or find the directory for the file to live in, so bail
			m_error = true;
			m_ready = false;
			fprintf(stderr, "XMLFileReporter.initialize() ERROR: unble to open file, %s", theTranscodedFileNamePointer);
			return(false);
		}
		else
		{
			m_ready = true;

			initStrings();

			startResultsFile();

			// fprintf(stderr, "DEBUG:XMLFileReporter.initialize() complete with " + fileName);
		}
	}

    return m_ready;
}



bool
XMLFileReporter::getFlushOnCaseClose()
{
    return(m_flushOnCaseClose);
}



const XalanDOMString& 
XMLFileReporter::getFileName() const
{
    return(m_fileName);
}



void 
XMLFileReporter::setFileName(const XalanDOMString& fileName)
{
	m_fileName = fileName;
}



bool 
XMLFileReporter::checkError()
{
    // Ensure our underlying reporter, if one, is still OK
    if (m_fileHandle == 0)            
    {            
        m_error = true;        
    }
    return(m_error);
}



bool 
XMLFileReporter::isReady() 
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
XMLFileReporter::flush()
{
    if (isReady())
    {
		fflush(m_fileHandle);
    }
}



void 
XMLFileReporter::close()
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
XMLFileReporter::logTestFileInit(const XalanDOMString& msg)
{
    if (isReady())
    {
        printToFile(LESS_THAN + ELEM_TESTFILE 
                              + SPACE + ATTR_DESC + EQUALS_QUOTE + escapestring(msg) + QUOTE_SPACE + ATTR_TIME + EQUALS_QUOTE + getDateTimeString() + QUOTE_GREATER_THAN);
    }
}



void 
XMLFileReporter::logTestFileClose(const XalanDOMString& /* msg */, const XalanDOMString& /* result */)
{
    if (isReady())
    {
//        printToFile(LESS_THAN + ELEM_FILERESULT 
//                             + SPACE + ATTR_DESC + EQUALS_QUOTE + escapestring(msg) + QUOTE_SPACE + ATTR_RESULT + EQUALS_QUOTE + result + QUOTE_SPACE + ATTR_TIME + EQUALS_QUOTE + getDateTimeString() + QUOTE_SOLIDUS_GREATER_THAN);
        printToFile(LESS_THAN_SOLIDUS + ELEM_TESTFILE + GREATER_THAN);
    }
    flush();
}



void 
XMLFileReporter::logTestCaseInit(const XalanDOMString& msg)
{
    if (isReady())
    {
        printToFile(TESTCASEINIT_HDR + escapestring(msg) + QUOTE_GREATER_THAN);
    }
}



void 
XMLFileReporter::logTestCaseClose(const XalanDOMString& /* msg */, const XalanDOMString& /* result */)
{
    if (isReady())
    {
        //printToFile(TESTCASECLOSE_HDR + escapestring(msg) + QUOTE_SPACE + ATTR_RESULT + EQUALS_QUOTE + result + QUOTE_SOLIDUS_GREATER_THAN);
        printToFile(LESS_THAN_SOLIDUS + ELEM_TESTCASE + GREATER_THAN);
    }
    if (getFlushOnCaseClose())
    {
        flush();
    }
}



void 
XMLFileReporter::logMessage(int level, const XalanDOMString& msg)
{
	char tmp[20];
	sprintf(tmp, "%d", level);

    if (isReady())
    {
        printToFile(MESSAGE_HDR + tmp + QUOTE_GREATER_THAN);
        printToFile(escapestring(msg));
        printToFile(LESS_THAN_SOLIDUS + ELEM_MESSAGE + GREATER_THAN);
    }
}

void XMLFileReporter::addMetricToAttrs(char* desc, double theMetric, Hashtable& attrs)
{
	XalanDOMString	temp;

	DoubleToDOMString(theMetric, temp);
	attrs.insert(Hashtable::value_type(XalanDOMString(desc), temp));

	return;
}

void 
XMLFileReporter::logElementWAttrs(int /* level */, const XalanDOMString& element, Hashtable attrs, const XalanDOMString& msg)
{
	if (isReady() && !element.empty()&& !attrs.empty())
    {
//		char tmp[20];
//		sprintf(tmp, "%d", level);
//
//		Took out this level attribute cuz we don't use it.
//      printToFile(LESS_THAN + element + SPACE + ATTR_LEVEL + EQUALS_QUOTE
//                      + tmp + QUOTE);
        printToFile(LESS_THAN + element + SPACE);
	
		Hashtable::iterator theEnd = attrs.end();	
    
       	for(Hashtable::iterator i = attrs.begin(); i != theEnd; ++i)
        {            
			
            printToFile((*i).first + EQUALS_QUOTE
                                  + (*i).second + QUOTE);
        }

        printToFile(GREATER_THAN);
        if (msg.empty() != 0)
            printToFile(escapestring(msg));
        printToFile(LESS_THAN_SOLIDUS + element + GREATER_THAN);
    }
}

void 
XMLFileReporter::logElement(const XalanDOMString& element, const XalanDOMString& msg)
{
	if (isReady() && !element.empty() && !msg.empty())
    {
		printToFile(LESS_THAN + element + GREATER_THAN + escapestring(msg) + LESS_THAN_SOLIDUS + element + GREATER_THAN);
    }
}

void 
XMLFileReporter::logStatistic (int level, long lVal, double dVal, const XalanDOMString& msg)
{
	if (isReady())
    {
		char tmp[40];

		sprintf(tmp, "%d", level);
        printToFile(STATISTIC_HDR + tmp + QUOTE_SPACE + ATTR_DESC + EQUALS_QUOTE + escapestring(msg) + QUOTE_GREATER_THAN);
		
		sprintf(tmp, "%ld", lVal);
		printToFile(LESS_THAN + ELEM_LONGVAL + GREATER_THAN + tmp + LESS_THAN_SOLIDUS + ELEM_LONGVAL + GREATER_THAN);
		
		sprintf(tmp, "%f", dVal);
        printToFile(LESS_THAN + ELEM_DOUBLEVAL + GREATER_THAN + tmp + LESS_THAN_SOLIDUS + ELEM_DOUBLEVAL + GREATER_THAN);
        
		printToFile(LESS_THAN_SOLIDUS + ELEM_STATISTIC + GREATER_THAN);
		
    }
}



void 
XMLFileReporter::logArbitraryMessage (int level, const XalanDOMString& msg)
{
	char tmp[20];
	sprintf(tmp, "%d", level);

    if (isReady())
    {            
		printToFile(ARBITRARY_HDR + tmp + QUOTE_GREATER_THAN);
        printToFile(escapestring(msg));
        printToFile(LESS_THAN_SOLIDUS + ELEM_ARBITRARY + GREATER_THAN);
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
XMLFileReporter::logCheckPass(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKPASS_HDR + escapestring(comment) + QUOTE_SOLIDUS_GREATER_THAN);
    }
}


void 
XMLFileReporter::logCheckFail(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(comment) + QUOTE_SOLIDUS_GREATER_THAN);

    }
}


void 
XMLFileReporter::logCheckFail(const XalanDOMString& test, Hashtable actexp)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(test) + QUOTE);

		printToFile(GREATER_THAN);
		
		Hashtable::iterator aeEnd = actexp.end();
       	for(Hashtable::iterator ii = actexp.begin(); ii != aeEnd; ++ii)
        {            
			logElement((*ii).first, (*ii).second);
        }

		printToFile(CHECKFAIL_FTR);
    }
}

void 
XMLFileReporter::logCheckFail(const XalanDOMString& test, Hashtable attrs, Hashtable actexp)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(test) + QUOTE);

		Hashtable::iterator fdEnd = attrs.end();	
       	for(Hashtable::iterator i = attrs.begin(); i != fdEnd; ++i)
        {            
            printToFile((*i).first + EQUALS_QUOTE
                                  + (*i).second + QUOTE);
        }

		printToFile(GREATER_THAN);
		
		Hashtable::iterator aeEnd = actexp.end();
       	for(Hashtable::iterator ii = actexp.begin(); ii != aeEnd; ++ii)
        {            
			logElement((*ii).first, (*ii).second);
        }

		printToFile(CHECKFAIL_FTR);
    }
}

void 
XMLFileReporter::logCheckAmbiguous(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKAMBG_HDR + escapestring(comment) + QUOTE_SOLIDUS_GREATER_THAN);
    }
}


void 
XMLFileReporter::logErrorResult(const XalanDOMString& test, const XalanDOMString& reason)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(test) + QUOTE_SPACE + XalanDOMString(REASON_EQUALS_QUOTE) + escapestring(reason)  + QUOTE_SOLIDUS_GREATER_THAN);

    }
}


void 
XMLFileReporter::logCheckErr(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKERRR_HDR + escapestring(comment) + QUOTE_SOLIDUS_GREATER_THAN);
    }
}



static const XalanDOMChar	theAmpersandString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charSemicolon,
	0
};



static const XalanDOMChar	theApostropheString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSemicolon,
	0
};



static const XalanDOMChar	theLessThanString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSemicolon,
	0
};



static const XalanDOMChar	theGreaterThanString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSemicolon,
	0
};



static const XalanDOMChar	theQuoteString[] =
{
	XalanUnicode::charAmpersand,
	XalanUnicode::charLetter_q,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSemicolon,
	0
};



XalanDOMString
XMLFileReporter::escapestring(const XalanDOMString&  s)
{
    XalanDOMString		sb;

    const XalanDOMString::size_type		length = s.length();

	sb.reserve(length);

    for (XalanDOMString::size_type i = 0; i < length; i++)
    {
        const XalanDOMChar	ch = charAt(s, i);

        if (XalanUnicode::charLessThanSign == ch)
        {
			append(sb, theLessThanString);
        }
        else if (XalanUnicode::charGreaterThanSign == ch)
        {
			append(sb, theGreaterThanString);
        }
		else if (XalanUnicode::charAmpersand == ch) 
		{
			append(sb, theAmpersandString);
		}
		else if (XalanUnicode::charQuoteMark == ch) 
		{
			append(sb, theQuoteString);
		}
		else if (XalanUnicode::charApostrophe == ch) 
		{
			append(sb, theApostropheString);
		}
        else
        {
            append(sb, ch);
        }
    }

	return sb;
}



bool 
XMLFileReporter::startResultsFile()
{
    if (isReady())
    {
        // Write out XML header and root test result element
        printToFile(XML_HEADER);

        // Note: this tag is closed in our .close() method, which the caller had better call!
        printToFile(LESS_THAN + ELEM_RESULTSFILE + SPACE + ATTR_FILENAME + EQUALS_QUOTE + m_fileName + QUOTE_GREATER_THAN);

        return true;
    }
    else
	{
        return false;
	}
}



bool 
XMLFileReporter::closeResultsFile()
{
    if (isReady() == false)
	{
        return false;
	}
	else
    {            
		printToFile(LESS_THAN_SOLIDUS + ELEM_RESULTSFILE + GREATER_THAN);
        return true;
    }
}


bool 
XMLFileReporter::printToFile(const XalanDOMString&	output) 
{
    if (isReady() == false)
	{
		return false;
	}
	else
    {
		const CharVectorType	theResult(TranscodeToLocalCodePage(output));

		if(!theResult.size())
		{
			fputs("Error transcoding text to local codepage", m_fileHandle);
		}
		else 
		{
			fputs(c_str(theResult), m_fileHandle);
		}

		fputs("\n", m_fileHandle);

        return true;
    }
}



XalanDOMString 
XMLFileReporter::getDateTimeString() 
{
#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::tm;
	using std::time;
	using std::localtime;
	using std::asctime;
	using std::strlen;
#endif

	struct tm *tmNow;
	time_t time_tNow;

	time(&time_tNow);     
	tmNow = localtime(&time_tNow);
	
	const char* const	theTime = asctime(tmNow);

	return XalanDOMString(theTime, strlen(theTime) - 1);
}



void
XMLFileReporter::initStrings()
{
	OPT_FILENAME = XALAN_STATIC_UCODE_STRING("filename");
	ELEM_RESULTSFILE = XALAN_STATIC_UCODE_STRING("resultsfile");
	ELEM_TESTFILE = XALAN_STATIC_UCODE_STRING("testfile");
	ELEM_FILERESULT = XALAN_STATIC_UCODE_STRING("fileresult");
	ELEM_TESTCASE = XALAN_STATIC_UCODE_STRING("Test_Dir");
	ELEM_CASERESULT = XALAN_STATIC_UCODE_STRING("Dir-result");
	ELEM_CHECKRESULT = XALAN_STATIC_UCODE_STRING("Testcase");
	ELEM_STATISTIC = XALAN_STATIC_UCODE_STRING("statistic");
	ELEM_LONGVAL = XALAN_STATIC_UCODE_STRING("longval");
	ELEM_DOUBLEVAL = XALAN_STATIC_UCODE_STRING("doubleval");
	ELEM_MESSAGE = XALAN_STATIC_UCODE_STRING("message");
	ELEM_ARBITRARY = XALAN_STATIC_UCODE_STRING("arbitrary");
	ELEM_HASHTABLE = XALAN_STATIC_UCODE_STRING("hashtable");
	ELEM_HASHITEM = XALAN_STATIC_UCODE_STRING("hashitem");
	ATTR_LEVEL = XALAN_STATIC_UCODE_STRING("level");
	ATTR_DESC = XALAN_STATIC_UCODE_STRING("desc");
	ATTR_TIME = XALAN_STATIC_UCODE_STRING("time");
	ATTR_RESULT = XALAN_STATIC_UCODE_STRING("result");
	ATTR_KEY = XALAN_STATIC_UCODE_STRING("key");
	ATTR_FILENAME = OPT_FILENAME;
	LESS_THAN = XALAN_STATIC_UCODE_STRING("<");
	GREATER_THAN = XALAN_STATIC_UCODE_STRING(">");
	EQUALS_QUOTE = XALAN_STATIC_UCODE_STRING("=\"");
	SPACE = XALAN_STATIC_UCODE_STRING(" ");
	QUOTE = XALAN_STATIC_UCODE_STRING("\"");
	QUOTE_SPACE = XALAN_STATIC_UCODE_STRING("\" ");
	QUOTE_GREATER_THAN = XALAN_STATIC_UCODE_STRING("\">");
	QUOTE_SOLIDUS_GREATER_THAN = XALAN_STATIC_UCODE_STRING("\"/>");
	PASS = XALAN_STATIC_UCODE_STRING("PASS");
	AMBG = XALAN_STATIC_UCODE_STRING("AMBG");
	ERRR = XALAN_STATIC_UCODE_STRING("ERRR");
	FAIL = XALAN_STATIC_UCODE_STRING("FAIL");
	LESS_THAN_SOLIDUS = XALAN_STATIC_UCODE_STRING("</");
	XML_HEADER = XALAN_STATIC_UCODE_STRING("<?xml version=\"1.0\"?>");
	REASON_EQUALS_QUOTE = XALAN_STATIC_UCODE_STRING("reason=\"");

	TESTCASEINIT_HDR = LESS_THAN + ELEM_TESTCASE + SPACE + ATTR_DESC + EQUALS_QUOTE;
	TESTCASECLOSE_HDR = LESS_THAN + ELEM_CASERESULT + SPACE + ATTR_DESC + EQUALS_QUOTE;
	MESSAGE_HDR = LESS_THAN + ELEM_MESSAGE + SPACE + ATTR_LEVEL + EQUALS_QUOTE;
	STATISTIC_HDR = LESS_THAN + ELEM_STATISTIC + SPACE + ATTR_LEVEL + EQUALS_QUOTE;
	ARBITRARY_HDR = LESS_THAN + ELEM_ARBITRARY + SPACE + ATTR_LEVEL + EQUALS_QUOTE;
	HASHTABLE_HDR = LESS_THAN + ELEM_HASHTABLE + SPACE + ATTR_LEVEL + EQUALS_QUOTE;
	HASHITEM_HDR = LESS_THAN + ELEM_HASHITEM + SPACE + ATTR_KEY + EQUALS_QUOTE;
	CHECKPASS_HDR = LESS_THAN + ELEM_CHECKRESULT + SPACE + ATTR_RESULT + EQUALS_QUOTE + PASS + QUOTE_SPACE + ATTR_DESC + EQUALS_QUOTE;
	CHECKAMBG_HDR = LESS_THAN + ELEM_CHECKRESULT + SPACE + ATTR_RESULT + EQUALS_QUOTE + AMBG + QUOTE_SPACE + ATTR_DESC + EQUALS_QUOTE;
	CHECKERRR_HDR = LESS_THAN + ELEM_CHECKRESULT + SPACE + ATTR_RESULT + EQUALS_QUOTE + ERRR + QUOTE_SPACE + ATTR_DESC + EQUALS_QUOTE;
	CHECKFAIL_HDR = LESS_THAN + ELEM_CHECKRESULT + SPACE + ATTR_RESULT + EQUALS_QUOTE + FAIL + QUOTE_SPACE + ATTR_DESC + EQUALS_QUOTE;
	CHECKFAIL_FTR = LESS_THAN_SOLIDUS + ELEM_CHECKRESULT + GREATER_THAN;
}



XALAN_CPP_NAMESPACE_END
