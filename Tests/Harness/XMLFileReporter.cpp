/*
*
* XMLFileReporter.cpp
*
* Copyright 2000 Lotus Development Corporation. All rights reserved.
* This software is subject to the Lotus Software Agreement, Restricted
* Rights for U.S. government users and applicable export regulations.
*/
#include "XMLFileReporter.hpp" 


const string const XMLFileReporter::OPT_FILENAME = "filename";

const string const XMLFileReporter::ELEM_RESULTSFILE = "resultsfile";
const string const XMLFileReporter::ELEM_TESTFILE = "testfile";
const string const XMLFileReporter::ELEM_FILERESULT = "fileresult";
const string const XMLFileReporter::ELEM_TESTCASE = "testcase";
const string const XMLFileReporter::ELEM_CASERESULT = "caseresult";
const string const XMLFileReporter::ELEM_CHECKRESULT = "checkresult";
const string const XMLFileReporter::ELEM_STATISTIC = "statistic";
const string const XMLFileReporter::ELEM_LONGVAL = "longval";
const string const XMLFileReporter::ELEM_DOUBLEVAL = "doubleval";
const string const XMLFileReporter::ELEM_MESSAGE = "message";
const string const XMLFileReporter::ELEM_ARBITRARY = "arbitrary";
const string const XMLFileReporter::ELEM_HASHTABLE = "hashtable";
const string const XMLFileReporter::ELEM_HASHITEM = "hashitem";

const string const XMLFileReporter::ATTR_LEVEL = "level";
const string const XMLFileReporter::ATTR_DESC = "desc";
const string const XMLFileReporter::ATTR_TIME = "time";
const string const XMLFileReporter::ATTR_RESULT = "result";
const string const XMLFileReporter::ATTR_KEY = "key";
const string const XMLFileReporter::ATTR_FILENAME = XMLFileReporter::OPT_FILENAME;

const string XMLFileReporter::TESTCASEINIT_HDR = "<" + ELEM_TESTCASE + " " + ATTR_DESC + "=\"";
const string XMLFileReporter::TESTCASECLOSE_HDR = "<" + ELEM_CASERESULT + " " + ATTR_DESC + "=\"";
const string XMLFileReporter::MESSAGE_HDR = "<" + ELEM_MESSAGE + " " + ATTR_LEVEL + "=\"";
const string XMLFileReporter::STATISTIC_HDR = "<" + ELEM_STATISTIC + " " + ATTR_LEVEL + "=\"";
const string XMLFileReporter::ARBITRARY_HDR = "<" + ELEM_ARBITRARY + " " + ATTR_LEVEL + "=\"";
const string XMLFileReporter::HASHTABLE_HDR = "<" + ELEM_HASHTABLE + " " + ATTR_LEVEL + "=\"";
const string XMLFileReporter::HASHITEM_HDR = "  <" + ELEM_HASHITEM + " " + ATTR_KEY + "=\"";
const string XMLFileReporter::CHECKPASS_HDR = "<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "PASS" + "\" " + ATTR_DESC + "=\"";
const string XMLFileReporter::CHECKAMBG_HDR = "<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "AMBG" + "\" " + ATTR_DESC + "=\"";
const string XMLFileReporter::CHECKERRR_HDR = "<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "ERRR" + "\" " + ATTR_DESC + "=\"";
const string XMLFileReporter::CHECKFAIL_HDR = "<" + ELEM_CHECKRESULT + " " + ATTR_RESULT + "=\"" + "FAIL" + "\" " + ATTR_DESC + "=\"";



XMLFileReporter::XMLFileReporter():
	m_anyOutput(false),
	m_fileName(""),
	m_fileHandle(0),
	m_ready(false),
	m_error(false),
	m_flushOnCaseClose(true)
{        
}



XMLFileReporter::XMLFileReporter(const string& fileName):
	m_anyOutput(false),
	m_fileName(fileName),
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
    if (m_fileName.length == 0)
    {
        // We don't have a valid file, so bail
        m_error = true;
        m_ready = false;
        fprintf(stderr, "XMLFileReporter.initialize() ERROR: file name is blank");
        return(false);
    }

    // Create a file and ensure it has a place to live
	m_fileHandle = fopen(m_fileName.c_str(), "w");
	if (m_fileHandle == 0)
	{
        // Couldn't create or find the directory for the file to live in, so bail
        m_error = true;
        m_ready = false;
        fprintf(stderr, "XMLFileReporter.initialize() ERROR: unble to open file, %s", m_fileName);
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



const string& 
XMLFileReporter::getFileName() const
{
    return(m_fileName);
}



void 
XMLFileReporter::setFileName(const string& fileName)
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
XMLFileReporter::logTestFileInit(const string& msg)
{
    if (isReady())
    {
        printToFile("<" + ELEM_TESTFILE 
                              + " " + ATTR_DESC + "=\"" + escapestring(msg) + "\" " + ATTR_TIME + "=\"" + getDateTimeString() + "\">");
    }
}



void 
XMLFileReporter::logTestFileClose(const string& msg, const string& result)
{
    if (isReady())
    {
        printToFile("<" + ELEM_FILERESULT 
                              + " " + ATTR_DESC + "=\"" + escapestring(msg) + "\" " + ATTR_RESULT + "=\"" + result + "\" " + ATTR_TIME + "=\"" + getDateTimeString() + "\"/>");
        printToFile("</" + ELEM_TESTFILE + ">");
    }
    flush();
}



void 
XMLFileReporter::logTestCaseInit(const string& msg)
{
    if (isReady())
    {
        printToFile(TESTCASEINIT_HDR + escapestring(msg) + "\">");
    }
}



void 
XMLFileReporter::logTestCaseClose(const string& msg, const string& result)
{
    if (isReady())
    {
        printToFile(TESTCASECLOSE_HDR + escapestring(msg) + "\" " + ATTR_RESULT + "=\"" + result + "\"/>");
        printToFile("</" + ELEM_TESTCASE + ">");
    }
    if (getFlushOnCaseClose())
    {
        flush();
    }
}



void 
XMLFileReporter::logMessage(int level, const string& msg)
{
    if (isReady())
    {
        printToFile(MESSAGE_HDR + (char)level + "\">");
        printToFile(escapestring(msg));
        printToFile("</" + ELEM_MESSAGE +">");
    }
}



void 
XMLFileReporter::logStatistic (int level, long lVal, double dVal, const string& msg)
{
    if (isReady())
    {
        //printToFile(STATISTIC_HDR + level + "\" " + ATTR_DESC + "=\"" + escapestring(msg) + "\">");
        fprintf(m_fileHandle, "%s%d%s%s%s%s", STATISTIC_HDR, level, "\" ", ATTR_DESC, "=\"", escapestring(msg), "\">\n");
		//printToFile("<" + ELEM_LONGVAL + ">" + lVal + "</" + ELEM_LONGVAL + ">");
		fprintf(m_fileHandle, "%s%s%d%s%s", "<", ELEM_LONGVAL, ">", lVal, "</", ELEM_LONGVAL, ">\n");
        //printToFile("<" + ELEM_DOUBLEVAL + ">" + dVal + "</" + ELEM_DOUBLEVAL + ">");
		fprintf(m_fileHandle, "%s%s%d%s%s", "<", ELEM_DOUBLEVAL, ">", dVal, "</", ELEM_DOUBLEVAL + ">\n");
        printToFile("</" + ELEM_STATISTIC + ">");
		
    }
}



void 
XMLFileReporter::logArbitraryMessage (int level, const string& msg)
{
    if (isReady())
    {            
		fprintf(m_fileHandle, "%s%d%s", ARBITRARY_HDR, level, "\">\n");
        printToFile(escapestring(msg));
        printToFile("</" + ELEM_ARBITRARY +">");
    }
}



/*
void logHashtable (int level, Hashtable hash, string msg)
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
                string value = hash.get(key).tostring();
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
XMLFileReporter::logCheckPass(const string& comment)
{
    if (isReady())
    {
        printToFile(CHECKPASS_HDR + escapestring(comment) + "\"/>");
    }
}



void 
XMLFileReporter::logCheckAmbiguous(const string& comment)
{
    if (isReady())
    {
        printToFile(CHECKAMBG_HDR + escapestring(comment) + "\"/>");
    }
}



void 
XMLFileReporter::logCheckFail(const string& comment)
{
    if (isReady())
    {
        printToFile(CHECKFAIL_HDR + escapestring(comment) + "\"/>");
    }
}



void 
XMLFileReporter::logCheckErr(const string& comment)
{
    if (isReady())
    {
        printToFile(CHECKERRR_HDR + escapestring(comment) + "\"/>");
    }
}



string 
XMLFileReporter::escapestring(const string&  s)
{
    string sb;
    const int length = s.length();

    for (int i = 0; i < length; i++)
    {
        char ch = s.at(i);
        if ('<' == ch)
        {
            sb.append("&lt;");
        }
        else if ('>' == ch)
        {
            sb.append("&gt;");
        }
        // Note: Skipping escaping of UTF-16 surrogates and & ampersands, since 
        //  I don't think we'll be outputting them or they won't affect our output
        else
        {
            sb += ch;
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
        printToFile("<?xml version=\"1.0\"?>");
        // Note: this tag is closed in our .close() method, which the caller had better call!
        printToFile("<" + ELEM_RESULTSFILE + " " + ATTR_FILENAME + "=\"" + m_fileName + "\">");
        return true;
    }
    else
        return false;
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
XMLFileReporter::printToFile(const string& output) 
{
    if (isReady())
    {
        fprintf(m_fileHandle, output.c_str());
		fprintf(m_fileHandle, "\n");
        return true;
    }
    else
        return false;
}



string 
XMLFileReporter::getDateTimeString() 
{
	struct tm *tmNow;
	time_t time_tNow;
	string dateTimeString;

	time(&time_tNow);     
	tmNow = localtime(&time_tNow);  							
	dateTimeString = asctime(tmNow); 
	dateTimeString.replace(dateTimeString.length()-1, 1, "");

	return dateTimeString;       
}
