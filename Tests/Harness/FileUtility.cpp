/*
*
* FileUtility.cpp
*
* Copyright 2000 Lotus Development Corporation. All rights reserved.
* This software is subject to the Lotus Software Agreement, Restricted
* Rights for U.S. government users and applicable export regulations.
*/

#include <stdlib.h>
#include <cstdio>
#include <ctime>
#include <vector>
#include <string.h>
#include <time.h>

// Added for directory creation 
#include <strstream>
#include <stdio.h>
#include <direct.h>


#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

//#include <Include/PlatformDefinitions.hpp>

#include <framework/URLInputSource.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XercesDefs.hpp>

#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanNodeList.hpp>

#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/DirectoryEnumerator.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanUnicode.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanStdOutputStream.hpp>
#include "FileUtility.hpp"


/*	This routine retrieves test file names from specified directories.
//	Inputs: baseDir:	typically "conf" or "perf"
//			relDir:		sub-directory to search.
//
//	Notes:	It builds the searchSpecification by concatenating all the 
//			necessary components.
//																			*/	
FileNameVectorType FileUtility::getTestFileNames(XalanDOMString baseDir, XalanDOMString relDir, bool useDirPrefix)
{
	const XalanDOMString	searchSuffix(XALAN_STATIC_UCODE_STRING("*.xsl"));
	XalanDOMString	searchSpecification;

	// Allow directory search w/o mandating files start with directory name. Required for files
	// garnered from XSLTMARK performance directory exm.
	if (useDirPrefix)
	{
		assign(searchSpecification, baseDir + relDir + pathSep + relDir + searchSuffix);
	}
	else
	{
		assign(searchSpecification, baseDir + relDir + pathSep + searchSuffix); 
	}


	DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString>	theEnumerator;
	FileNameVectorType	theFiles;
	theEnumerator(searchSpecification, theFiles);

	return theFiles;
}

/*	This routine retrieves all sub-directories from the specified directories.
//	Inputs: rootDirectory:	typically "conf" or "perf"
//
//	Notes:	The searchSpecification in this case is just "*". 
//																			*/	
FileNameVectorType FileUtility::getDirectoryNames(XalanDOMString rootDirectory)
{
	const XalanDOMString	dirSpec(XALAN_STATIC_UCODE_STRING("*"));

	DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString, DirectoryFilterPredicate> theEnumerator;
	FileNameVectorType	theFiles;
	theEnumerator(XalanDOMString(rootDirectory), XalanDOMString(dirSpec), theFiles);

	return theFiles;
}

bool FileUtility::checkDir(XalanDOMString directory )
{
char buffer[_MAX_PATH];

	_getcwd( buffer, _MAX_PATH );

	if ( _chdir(c_str(TranscodeToLocalCodePage(directory))) )
	{
		_chdir(buffer);
		return false;
	}
	else
	{
		return true;
	}
}


void FileUtility::checkAndCreateDir(XalanDOMString directory )
{
char buffer[_MAX_PATH];

	_getcwd( buffer, _MAX_PATH );


	if ( (_chdir(c_str(TranscodeToLocalCodePage(directory)))) )
	{
		//cout << "Couldn't change to " << directory << ", will create it." << endl;
		if ( !(_mkdir(c_str(TranscodeToLocalCodePage(directory)))))
		{
			cout << directory << " created." << endl;
		}
	}

	_chdir(buffer);
}

/*	This routine generates file names based on the provide suffix
//	Inputs: theXMLFileName:	typically "conf" or "perf"
//			suffix:			typically "xsl" or "out".
//
//	Notes:	
//											*/	

XalanDOMString FileUtility::GenerateFileName(const XalanDOMString&  theXMLFileName, char* suffix)
{
	XalanDOMString	theResult;
	int				thePeriodIndex = -1;
	const int		theLength = length(theXMLFileName);

	for (int i = theLength - 1; i > 0; i--)
	{
		if (charAt(theXMLFileName, i) == XalanUnicode::charFullStop)
		{
			thePeriodIndex = i;        // charFullStop is the dot (x2E)
			break;
		}
	}

	if (thePeriodIndex != -1)
	{
		theResult = substring(theXMLFileName,
							  0,
							  thePeriodIndex + 1);


		theResult += XalanDOMString(suffix);
	}

	return theResult;
}


/*	This routine generates a Unique Runid. 
//	Inputs: None
//			
//	Notes: The format is mmddhhmm. For example
//		   03151046 is "Mar 15 10:46"	
*/

XalanDOMString FileUtility::GenerateUniqRunid()
{

		struct tm *newtime;
		time_t long_time;
		char tmpbuf[10];

		time( &long_time );                /* Get time as long integer. */
		newtime = localtime( &long_time ); /* Convert to local time. */

		strftime( tmpbuf, 10,"%m%d%H%M",newtime );

		return(XalanDOMString(tmpbuf));

}

/*	This routine get Xerces Version number. 
//	Inputs: None
//				
*/
XalanDOMString FileUtility::getXercesVersion()
{

	return(XalanDOMString(gXercesFullVersionStr));

}
