/*
* The Apache Software License, Version 1.1
*
*
* Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
* reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer. 
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. The end-user documentation included with the redistribution,
*    if any, must include the following acknowledgment:  
*       "This product includes software developed by the
*        Apache Software Foundation (http://www.apache.org/)."
*    Alternately, this acknowledgment may appear in the software itself,
*    if and wherever such third-party acknowledgments normally appear.
*
* 4. The names "Xalan" and "Apache Software Foundation" must
*    not be used to endorse or promote products derived from this
*    software without prior written permission. For written 
*    permission, please contact apache@apache.org.
*
* 5. Products derived from this software may not be called "Apache",
*    nor may "Apache" appear in their name, without prior written
*    permission of the Apache Software Foundation.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
*
* This software consists of voluntary contributions made by many
* individuals on behalf of the Apache Software Foundation and was
* originally based on software copyright (c) 1999, International
* Business Machines, Inc., http://www.ibm.com.  For more
* information on the Apache Software Foundation, please see
* <http://www.apache.org/>.
*/




// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include "MsgCreator.h"
#include "InMemHandler.hpp"
#include "ICUResHandler.hpp"
#include "NLSHandler.hpp"
#include <cassert>

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif


XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    cout << "\nUsage:\n"
		"    MsgCreator [options] <XML file>\n\n"
		"This program reads XML message file, and then creates C++ the " << endl <<
		" source / data files for message localization (for .dll/.so build) \n"
		
		"Options:\n"
		"   -TYPE xxx    Type of localization [ICU | NLS | InMem*]\n"
		"   -LOCAL       ex: [ fr | fr_FR ]; The default is an 'en' local \n"
		"                       If your .xlf file doesn't contain translation \n"
		"                       to 'xx_YY' local, the system will look for\n"
		"                       'xx' translation; "<< endl <<
		"   <XML file>   Can be provided optionally. If not provided, the \n"
		"                       program looks for ${XALANCROOT}/xalanc/NLS directory, \n"
		"                       and looking for XalanMsg_<given local>.xlf file. \n"
		"  * = Default if not provided explicitly.\n\n"
		"    -?          Show this help.\n\n"
		"\n"
		<<  endl;
}


class CmdLineParams
{
	
public:
	
	typedef enum{

		ICU_LOCALMSG = 0,
		INMEM_LOCALMSG,
		NLS_LOCALMSG

	}TypeOfLocalMsg;
	
	const char*		inXMLFileName;
	
	TypeOfLocalMsg  enTypeOfLocalMsg;
	
	const char*      encodingName;
	
	char*		localName;

	bool		isLocalGiven;

	CmdLineParams():
	inXMLFileName(0),
		enTypeOfLocalMsg(INMEM_LOCALMSG),
		encodingName("LATIN1"),
		localName(0),
		isLocalGiven(false){}

	~CmdLineParams() { delete[] localName; }
};

#if defined(OS390)
#include <strings.h>                                             

int
compareNoCase(
			  const char*		str1,
			  const char*		str2)     
{
	return strcasecmp(str1, str2);
}

#else

int
compareNoCase(
			  const char*		str1,
			  const char*		str2)     
{
	return stricmp(str1, str2);
}

#endif

bool
getArgs(
		int				argc,
		char*			argv[],
		CmdLineParams&	p,
		const char**	pErrorMsg)
{
	bool fSuccess = true;
	char* errorMessage = 0;
	if ( argc > 7)
	{
		fSuccess = false;
		errorMessage = "Too many parameters\n";
	}
	for (int i = 1; i < argc && fSuccess == true; ++i)
	{
		if (!compareNoCase("-h", argv[i]) || !compareNoCase("-?", argv[i]))
		{
			fSuccess = false;
		}
		else if (!compareNoCase("-LOCAL", argv[i]) )
		{
			++i;
			if( i < argc && argv[i][0] != '-' )
			{
				int localLen = strlen(argv[i]);
				if ( localLen != 2 && localLen !=5)
				{
					fSuccess = false;
					errorMessage = "Unrecognized local\n";
				}

				if ( fSuccess  && localLen == 5 && argv[i][2] != '_')
				{
					fSuccess = false;
					errorMessage = "Unrecognized form of local\n";
				}
				if ( fSuccess )
				{
					p.isLocalGiven = true;
					
					p.localName = new char[10];
					
					strcpy(p.localName,argv[i]);
					
					p.localName[9]='\0'; // just in case
				}
			}
		}
		else if (!compareNoCase("-TYPE", argv[i]) )
		{
			++i;
			
			if( i < argc && argv[i][0] != '-' )
			{
				if( !compareNoCase("ICU",argv[i] ))
				{
					p.enTypeOfLocalMsg = CmdLineParams::ICU_LOCALMSG;
					
				}else if( !compareNoCase("NLS",argv[i] ))
				{
					p.enTypeOfLocalMsg = CmdLineParams::NLS_LOCALMSG;
					
				}
				else if( !compareNoCase("InMem",argv[i] ))
				{
					p.enTypeOfLocalMsg = CmdLineParams::INMEM_LOCALMSG;
					
				}else 
				{ 
					fSuccess = false;
					errorMessage = "Unknown TYPE of localization\n";
				}
			}
			else
			{
				fSuccess = false;
			}
		}else if ( argv[i][0] != '-' )
		{
			p.inXMLFileName = argv[i];
		}
	}

	if ( fSuccess )
	{
		*pErrorMsg = 0;
	}
	else
	{
		*pErrorMsg = errorMessage;
	}

	return fSuccess;
}

static char* buildInputFileName(const CmdLineParams&	p)
{

	char* result = new char[85];
	result[0] = '\0';

	// if file name was given expisitly, don't build it
	if( p.inXMLFileName != 0)
	{
		assert(strlen(p.inXMLFileName) < 84);
		strcpy(result,p.inXMLFileName);
		return result;
	}

	strcat(result,s_xalanMsgFileName);

	if( p.isLocalGiven )
	{

		strncat(result,p.localName,2);

	}
	else
	{
		strcat(result,s_en);

	}
	strcat(result,s_xliffExten);

	return result;
}

static char* buildOutputFileName(const CmdLineParams&	p)
{
	char* pOutputFileName = new char[80];
	pOutputFileName[0] = '\0';

	switch(p.enTypeOfLocalMsg){
	case CmdLineParams::ICU_LOCALMSG:
		{
			if (p.isLocalGiven )
			{
				strcpy(pOutputFileName, p.localName);
			}
			else
			{
				strcpy(pOutputFileName, s_en);
			}

			strcat(pOutputFileName, s_txtExten);

			break;
		}
	case CmdLineParams::INMEM_LOCALMSG:
		{
			strcpy(pOutputFileName, DATA_FILE_NAME);
			break;
		}

	case CmdLineParams::NLS_LOCALMSG:
		{
			strcpy(pOutputFileName,s_xalanMsgFileName);

			if (p.isLocalGiven )
			{
				strncat(pOutputFileName, p.localName,6);
			}
			else
			{
				strncat(pOutputFileName, s_en,3);
			}

			strcat(pOutputFileName, s_msgExten);

			break;
		}
	default:
		{
			// in that stage the local should be defined
			assert ( 0 );
		}
	}
	return pOutputFileName;
}




// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
	
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif
    
	CmdLineParams	theParams;
	int iReturnValue = 0;
	const char* errorMessage = 0;
	if (getArgs(argC, argV, theParams, &errorMessage) == false)
	{
		if ( errorMessage != 0)
		{
			cerr << errorMessage;
		}
		usage();
		iReturnValue = 1;
		
	}
	else
	{
		
		try
		{
			XMLPlatformUtils::Initialize();
		}
		
		catch (const XMLException& toCatch)
		{
			cerr << "Error during initialization! :\n"
				<< StrX(toCatch.getMessage()) << endl;
			iReturnValue = 2;
		}
		if (iReturnValue == 0)
		{
			
			{	//  Create a SAX parser object. 
				SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
				
				parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
				
				parser->setFeature(XMLUni::fgXercesDynamic, true);
				
				parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, false);
				
				parser->setFeature(XMLUni::fgXercesSchema, true);
				
				parser->setFeature(XMLUni::fgXercesSchemaFullChecking, false);
				
				parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
				

				//  Create the handler object and install it as the document and error
				//  handler for the parser. Then parse the file and catch any exceptions
				//  that propogate out
				//

				int errorCount = 0;

				char* const		fileName = buildInputFileName(theParams);

				char* const		pOutputFileName = buildOutputFileName(theParams);

				SAX2Handler* handler = 0;
				try
				{
					switch(theParams.enTypeOfLocalMsg){
					case CmdLineParams::ICU_LOCALMSG:
						{
							handler = new ICUResHandler(pOutputFileName);
							break;
						}
					case CmdLineParams::INMEM_LOCALMSG:
						{
							handler = new InMemHandler(pOutputFileName);
							break;
						}
					case CmdLineParams::NLS_LOCALMSG:
						{
							handler = new NLSHandler(pOutputFileName);
							break;
						}
					default:
						{
							assert (0);
						}
					}
				}
				
				catch(const XalanFileOutputStream::XalanFileOutputStreamOpenException& e)
				{
					cerr << "\nAn error occurred\n  Error: ";
					if (e.m_pMessage != 0)
					{
						cerr << e.m_pMessage <<  endl;
					}
					else
					{
						cerr << "unknown (XalanFileOutputStreamOpenException) " << "\n" << endl;
						
					}
					
					iReturnValue = 4;	
					
					
				}
				catch(const XalanFileOutputStream::XalanFileOutputStreamWriteException& e)
				{
					cerr << "\nAn error occurred\n  Error: ";
					if (e.m_pMessage != 0)
					{
						cerr << e.m_pMessage << endl;
					}
					else
					{
						cerr << "unknown (XalanFileOutputStreamWriteException) " << "\n" << endl;
						
					}
					
					iReturnValue = 5;	
					
				}
				catch (...)
				{
					cerr << "Unknown error occured." << endl;
					iReturnValue = 6;
				}
				
				// run the parser
				if ( iReturnValue == 0)
				{
					try
					{				
						if ( theParams.isLocalGiven == true )
						{
							handler->setXML_Lang(theParams.localName);
						}
						
						parser->setContentHandler(handler);

						parser->setErrorHandler(handler);
						
						parser->parse(fileName);
						
						errorCount = parser->getErrorCount();
					}
					
					catch (const XMLException& toCatch)
					{
						cerr << "\nAn error occurred\n  Error: "
							<< StrX(toCatch.getMessage())
							<< "\n" << endl;
						iReturnValue = 7;
					}
					catch (...)
					{	
						cerr << endl << "General error occured." << endl;
						iReturnValue = 8;
					}
				}
				

				delete handler;

				delete[] pOutputFileName;

				delete[] fileName;
				//
				//  Delete the parser itself.  Must be done prior to calling Terminate, below.
				//
				delete parser;

				if (errorCount != 0)
					iReturnValue = 9;

			}
			// And call the termination method
			XMLPlatformUtils::Terminate();
			
			
			
		}
		
	}
	return iReturnValue;
}

