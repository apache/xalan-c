/*
*
* XMLFileReporter.cpp
*
* Copyright 2000 Lotus Development Corporation. All rights reserved.
* This software is subject to the Lotus Software Agreement, Restricted
* Rights for U.S. government users and applicable export regulations.
*/

#include <stdlib.h>
#include "XMLFileReporter.hpp" 


#include "PlatformSupport/XalanUnicode.hpp"


const XalanDOMString  XMLFileReporter::OPT_FILENAME("filename");
const XalanDOMString  XMLFileReporter::ELEM_RESULTSFILE("resultsfile");
const XalanDOMString  XMLFileReporter::ELEM_TESTFILE("testfile");
const XalanDOMString  XMLFileReporter::ELEM_FILERESULT("fileresult");
const XalanDOMString  XMLFileReporter::ELEM_TESTCASE("Test_Dir");
const XalanDOMString  XMLFileReporter::ELEM_CASERESULT("Dir-result");
const XalanDOMString  XMLFileReporter::ELEM_CHECKRESULT("Testcase");
const XalanDOMString  XMLFileReporter::ELEM_STATISTIC("statistic");
const XalanDOMString  XMLFileReporter::ELEM_LONGVAL("longval");
const XalanDOMString  XMLFileReporter::ELEM_DOUBLEVAL("doubleval");
const XalanDOMString  XMLFileReporter::ELEM_MESSAGE("message");
const XalanDOMString  XMLFileReporter::ELEM_ARBITRARY("arbitrary");
const XalanDOMString  XMLFileReporter::ELEM_HASHTABLE("hashtable");
const XalanDOMString  XMLFileReporter::ELEM_HASHITEM("hashitem");
const XalanDOMString  XMLFileReporter::ATTR_LEVEL("level");
const XalanDOMString  XMLFileReporter::ATTR_DESC("desc");
const XalanDOMString  XMLFileReporter::ATTR_TIME("time");
const XalanDOMString  XMLFileReporter::ATTR_RESULT("result");
const XalanDOMString  XMLFileReporter::ATTR_KEY("key");
const XalanDOMString  XMLFileReporter::ATTR_FILENAME = XMLFileReporter::OPT_FILENAME;

const XalanDOMString XMLFileReporter::TESTCASEINIT_HDR("<" + ELEM_TESTCASE + " " + ATTR_DESC + "=\"");
const XalanDOMString XMLFileReporter::TESTCASECLOSE_HDR("<" + ELEM_CASERESULT + " " + ATTR_DESC + "=\"");
const XalanDOMString XMLFileReporter::MESSAGE_HDR("<" + ELEM_MESSAGE + " " + ATTR_LEVEL + "=\"");
const XalanDOMString XMLFileReporter::STATISTIC_HDR("<" + ELEM_STATISTIC + " " + ATTR_LEVEL + "=\"");
const XalanDOMString XMLFileReporter::ARBITRARY_HDR("<" + ELEM_ARBITRARY + " " + ATTR_LEVEL + "=\"");
const XalanDOMString XMLFileReporter::HASHTABLE_HDR("<" + ELEM_HASHTABLE + " " + ATTR_LEVEL + "=\"");
const XalanDOMString XMLFileReporter::HASHITEM_HDR("<" + ELEM_HASHITEM + " " + ATTR_KEY + "=\"");
const XalanDOMString XMLFileReporter::CHECKPASS_HDR("<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "PASS" + "\" " + ATTR_DESC + "=\"");
const XalanDOMString XMLFileReporter::CHECKAMBG_HDR("<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "AMBG" + "\" " + ATTR_DESC + "=\"");
const XalanDOMString XMLFileReporter::CHECKERRR_HDR("<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "ERRR" + "\" " + ATTR_DESC + "=\"");
const XalanDOMString XMLFileReporter::CHECKFAIL_HDR("<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "FAIL" + "\" " + ATTR_DESC + "=\"");
const XalanDOMString XMLFileReporter::CHECKFAIL_FTR("</" + ELEM_CHECKRESULT + ">");


XMLFileReporter::XMLFileReporter():
	m_anyOutput(false),
	m_fileName(""),
	m_fileHandle(0),
	m_ready(false),
	m_error(false),
	m_flushOnCaseClose(true)
{        
}



XMLFileReporter::XMLFileReporter(const XalanDOMString& fileName):
	m_anyOutput(false),
	m_fileName(fileName),
	m_fileHandle(0),
	m_ready(false),
	m_error(false),
	m_flushOnCaseClose(true)
{         
    m_ready = initialize();
}

XMLFileReporter::XMLFileReporter(const char* fileName):
	m_anyOutput(false),
	m_fileName(XalanDOMString(fileName)),
	m_fileHandle(0),
	m_ready(false),
	m_error(false),
	m_flushOnCaseClose(true)
{         
    m_ready = initialize();
}

bool 
XMLFileReporter::initialize()
{   	
    if (length(m_fileName) == 0)
    {
        // We don't have a valid file, so bail
        m_error = true;
        m_ready = false;
        fprintf(stderr, "XMLFileReporter.initialize() ERROR: No file name specified");
        return(false);
    }

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

    m_ready = true;
    startResultsFile();
    // fprintf(stderr, "DEBUG:XMLFileReporter.initialize() complete with " + fileName);

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
        printToFile("<" + ELEM_TESTFILE 
                              + " " + ATTR_DESC + "=\"" + escapestring(msg) + "\" " + ATTR_TIME + "=\"" + getDateTimeString() + "\">");
    }
}



void 
XMLFileReporter::logTestFileClose(const XalanDOMString& msg, const XalanDOMString& result)
{
    if (isReady())
    {
//        printToFile("<" + ELEM_FILERESULT 
//                             + " " + ATTR_DESC + "=\"" + escapestring(msg) + "\" " + ATTR_RESULT + "=\"" + result + "\" " + ATTR_TIME + "=\"" + getDateTimeString() + "\"/>");
        printToFile("</" + ELEM_TESTFILE + ">");
    }
    flush();
}



void 
XMLFileReporter::logTestCaseInit(const XalanDOMString& msg)
{
    if (isReady())
    {
        printToFile(TESTCASEINIT_HDR + escapestring(msg) + "\">");
    }
}



void 
XMLFileReporter::logTestCaseClose(const XalanDOMString& msg, const XalanDOMString& result)
{
    if (isReady())
    {
        //printToFile(TESTCASECLOSE_HDR + escapestring(msg) + "\" " + ATTR_RESULT + "=\"" + result + "\"/>");
        printToFile("</" + ELEM_TESTCASE + ">");
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
        printToFile(MESSAGE_HDR + tmp + "\">");
        printToFile(escapestring(msg));
        printToFile("</" + ELEM_MESSAGE +">");
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
XMLFileReporter::logElementWAttrs(int level, const XalanDOMString& element, Hashtable attrs, const XalanDOMString& msg)
{
	if (isReady()
        && (element.empty() == 0)
        && (attrs.empty() == 0)
       )
    {
		char tmp[20];
		sprintf(tmp, "%d", level);
//
//		Took out this level attribute cuz we don't use it.
//      printToFile("<" + escapestring(element) + " " + ATTR_LEVEL + "=\""
//                      + tmp + "\"");
        printToFile("<" + escapestring(element) + " ");
	
		Hashtable::iterator theEnd = attrs.end();	
    
       	for(Hashtable::iterator i = attrs.begin(); i != theEnd; ++i)
        {            
			
            printToFile((*i).first + "=\""
                                  + (*i).second + "\"");
        }

        printToFile(XalanDOMString(">"));
        if (msg.empty() != 0)
            printToFile(escapestring(msg));
        printToFile("</" + escapestring(element) + ">");
    }
}

void 
XMLFileReporter::logElement(const XalanDOMString& element, const XalanDOMString& msg)
{
	if (isReady()
        && (element.empty() == 0)
        && (msg.empty() == 0)
       )
    {

        printToFile("<" + escapestring(element) + ">");

        if (msg.empty() == 0)
            //printToFile(escapestring(msg));
			printToFile(msg);
        printToFile("</" + escapestring(element) + ">");
    }
}

void 
XMLFileReporter::logStatistic (int level, long lVal, double dVal, const XalanDOMString& msg)
{
 	char tmp[20];


	if (isReady())
    {
		sprintf(tmp, "%d", level);
        printToFile(STATISTIC_HDR + tmp + "\" " + ATTR_DESC + "=\"" + escapestring(msg) + "\">");
		
		sprintf(tmp, "%d", lVal);
		printToFile("<" + ELEM_LONGVAL + ">" + tmp + "</" + ELEM_LONGVAL + ">");
		
		sprintf(tmp, "%d", dVal);
        printToFile("<" + ELEM_DOUBLEVAL + ">" + tmp + "</" + ELEM_DOUBLEVAL + ">");
        
		printToFile("</" + ELEM_STATISTIC + ">");
		
    }
}



void 
XMLFileReporter::logArbitraryMessage (int level, const XalanDOMString& msg)
{
	char tmp[20];
	sprintf(tmp, "%d", level);

    if (isReady())
    {            
		printToFile(ARBITRARY_HDR + tmp + "\">");
        printToFile(escapestring(msg));
        printToFile("</" + ELEM_ARBITRARY +">");
    }
}

/*
void logHashtable (int level, Hashtable hash, XalanDOMString msg)
{
    if (isReady())
    {
        printToFile(HASHTABLE_HDR + level + "\" " + ATTR_DESC + "=\"" + msg + "\">");
        if (hash == null)
        {
            printToFile("<" + ELEM_HASHITEM + " " + ATTR_KEY + "=\"null\">");
            printToFile("</" + ELEM_HASHITEM + ">");
        }
        try
        {
            for (Enumeration enum = hash.keys(); enum.hasMoreElements();)
            {
                Object key = enum.nextElement();
                // Ensure we'll have clean output by pre-fetching value before outputting anything
                XalanDOMString value = hash.get(key).tostring();
                printToFile(HASHITEM_HDR + key.tostring() + "\">");
                printToFile(value);
                printToFile("</" + ELEM_HASHITEM + ">");
            }
        } 
        catch (Exception e)
        {
            // No-op: should ensure we have clean output
        }
        printToFile("</" + ELEM_HASHTABLE +">");
    }
}
*/



void 
XMLFileReporter::logCheckPass(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKPASS_HDR + escapestring(comment) + "\"/>");
    }
}



void 
XMLFileReporter::logCheckAmbiguous(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKAMBG_HDR + escapestring(comment) + "\"/>");
    }
}



void 
XMLFileReporter::logCheckFail(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(comment) + "\"/>");

    }
}


void 
XMLFileReporter::logErrorResult(const XalanDOMString& test, const XalanDOMString& reason)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(test) + "\" " + XalanDOMString("reason=\"") + escapestring(reason)  + "\"/>");

    }
}

void 
XMLFileReporter::logCheckFail(const XalanDOMString& test, Hashtable attrs, Hashtable actexp)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(test) + "\"");

		Hashtable::iterator fdEnd = attrs.end();	
       	for(Hashtable::iterator i = attrs.begin(); i != fdEnd; ++i)
        {            
            printToFile((*i).first + "=\""
                                  + (*i).second + "\"");
        }

		printToFile(XalanDOMString(">"));
		
		Hashtable::iterator aeEnd = actexp.end();
       	for(Hashtable::iterator ii = actexp.begin(); ii != aeEnd; ++ii)
        {            
			logElement((*ii).first, (*ii).second);
        }

		printToFile(CHECKFAIL_FTR);
    }
}

void 
XMLFileReporter::logCheckErr(const XalanDOMString& comment)
{
    if (isReady())
    {
        printToFile(CHECKERRR_HDR + escapestring(comment) + "\"/>");
    }
}


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



XalanDOMString
XMLFileReporter::escapestring(const XalanDOMString&  s)
{
    XalanDOMString		sb;

    const unsigned int	length = s.length();

    for (unsigned int i = 0; i < length; i++)
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
        // Note: Skipping escaping of UTF-16 surrogates and & ampersands, since 
        //  I don't think we'll be outputting them or they won't affect our output
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
        printToFile(XalanDOMString("<?xml version=\"1.0\"?>"));

        // Note: this tag is closed in our .close() method, which the caller had better call!
        printToFile("<" + ELEM_RESULTSFILE + " " + ATTR_FILENAME + "=\"" + m_fileName + "\">");

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
    if (isReady())
    {            
		printToFile("</" + ELEM_RESULTSFILE + ">");
        return true;
    }
    else
        return false;
}


bool 
XMLFileReporter::printToFile(const XalanDOMString&	output) 
{
	CharVectorType	theResult(TranscodeToLocalCodePage(output));

    if (isReady())
    {
		if(!theResult.size())
		{
			fprintf(m_fileHandle, "Error transcoding text to local codepage");
			fprintf(m_fileHandle, "\n");
		}
		else 
		{
			fprintf(m_fileHandle, c_str(theResult));
			fprintf(m_fileHandle, "\n");
		}

        return true;
    }
    else
        return false;
}



XalanDOMString 
XMLFileReporter::getDateTimeString() 
{
	struct tm *tmNow;
	time_t time_tNow;

	time(&time_tNow);     
	tmNow = localtime(&time_tNow);
	
	const char* const	theTime = asctime(tmNow);

	return XalanDOMString(theTime, strlen(theTime) - 1);
}
