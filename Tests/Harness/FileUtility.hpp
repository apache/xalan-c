/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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
 *
 * FileUtility.hpp
 */
#if !defined(FILEUTILITY_HEADER_GUARD_1357924680)
#define FILEUTILITY_HEADER_GUARD_1357924680

#include <Harness/HarnessDefinitions.hpp>



#include <vector>


#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <strstream.h>
#else
#include <strstream>
#endif



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;
class PrintWriter;
class StylesheetRoot;
class XalanCompiledStylesheet;
class XalanNode;
class XalanSourceTreeDocument;
class XalanTransformer;
class XMLFileReporter;
class XSLTInputSource;



// This class is exported from the Harness.dll
class HARNESS_API FileUtility 
{
public:

// A vector to hold directory names and file names.
#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XalanDOMString>			FileNameVectorType;
#else
	typedef std::vector<XalanDOMString>		FileNameVectorType;
#endif

	/**
	 * Initialize static data.
	 * Should be called only once per process before creating any
	 * instances of FileUtility.
	 */
	static void
	initialize();

	/**
	 * Clean-up static data.
	 * Should be called only once per process after deleting all
	 * instances of FileUtility.
	 */
	static void
	terminate();

	struct HARNESS_API reportStruct
	{
		XalanDOMString  theDrive;
		XalanDOMString	testOrFile;
		XalanDOMString  xmlFileURL;
		XalanDOMString  xslFileURL;
		XalanDOMString  xmlFormat;
		const char*		msg;
		XalanDOMString	currentNode;
		XalanDOMString	actual;
		XalanDOMString	expected;
		int				pass;
		int				fail;
		int				nogold;

		reportStruct();

		void
		reset();

	} data;

	struct HARNESS_API cmdParams
	{
	private:

#if defined(XALAN_NO_STD_NAMESPACE)
		typedef ostrstream			StreamType;
#else
		typedef std::ostrstream		StreamType;
#endif

		StreamType	help;

	public:

		XalanDOMString  base;
		XalanDOMString	output;
		XalanDOMString  gold;
		XalanDOMString  sub;
		int				source;
		bool			skip;
		long			iters;


		cmdParams();

		~cmdParams()
		{
		}

		const char*
		getHelpMessage();

		StreamType&
		getHelpStream()
		{
			return help;
		}

	} args;

	/** Simple constructor, does not perform initialization.  */
	FileUtility();
	
	/** 
	* Utility method used to get test files from a specific directory.
	* @returns a vector containing test files.
	*/

	XalanDOMString
	getDrive();

	bool
	getParams(	int			argc, 
				const char*	argv[], 
				char*		outDir,
				bool		fsetGold = true);


	FileNameVectorType
	getTestFileNames(
			const XalanDOMString&	baseDir,
			const XalanDOMString&	relDir,
			bool					useDirPrefix);

	/** 
	* Utility method used to get subdirectories from a specific directory.
	* @returns a vector containing directory files.
	*/	
	FileNameVectorType
	getDirectoryNames(const XalanDOMString&		rootDirectory);

	/** 
	* Utility method used to create default directories when neccessary
	*/
    void
	checkAndCreateDir(const XalanDOMString&		directory);

	/** 
	* Utility method determines if directory exists.
	*/
	bool
	checkDir(const XalanDOMString&	directory);

	/** 
	* Utility method used to get XSL file based on XML file.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	getXSLFileName(const XalanDOMString&	theXMLFileName);

	/** 
	* Utility method used to get OUT file based on XML file.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	generateFileName(
			const XalanDOMString&  theXMLFileName,
			const char*				suffix,
			bool*					status = 0);

	/** 
	* Utility method used to generate UniqRunid.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	generateUniqRunid();

	/** 
	* Utility methods used to get Xerces Version number.
	* @returns a XalanDOMString.
	*/
	XalanDOMString
	getXercesVersion();


	void
	checkResults(
			const XalanDOMString&	outputFile, 
			const XalanDOMString&	goldFile, 
			XMLFileReporter&		logfile);

	void
	checkAPIResults(
			const XalanDOMString&	actual, 
			const XalanDOMString&	expected,
			const char*				msg,
			XMLFileReporter&		logfile,
			const XalanDOMString&	outputFile,
			const XalanDOMString&	goldFile,
			bool					containsOnly = false);

	void
	checkAPIResults(
			const char*				actual,
			const char*				expected,
			const char*				msg,
			XMLFileReporter&		logfile,
			const XalanDOMString&	outputFile,
			const XalanDOMString&	goldFile,
			bool					containsOnly = false)
	{
		checkAPIResults(
			XalanDOMString(actual), 
			XalanDOMString(expected),
			msg,
			logfile,
			outputFile,
			goldFile,
			containsOnly);
	}

	/**
	* Utility method used to compare the results. It inturn
	* call domCompare.  
	* @returns Void.
	*/
	void
	checkDOMResults(
			const XalanDOMString&			theOutputFile, 
			const XalanCompiledStylesheet*	compiledSS,
			const XalanSourceTreeDocument*	dom,
			const XSLTInputSource&			goldInputSource,
			XMLFileReporter&				logfile);

	bool
	compareSerializedResults(
			const XalanDOMString&	transformResult,
			const XalanDOMString&	goldInputSource);
	/**
	* Utility method used to create a FormatterToXML FormatterListener.
	* This is required to DOM comparisions. 
	* @returns a pointer to a FormatterListener.
	*/
	FormatterListener* 
	getXMLFormatter(
			PrintWriter&			resultWriter,
			int						indentAmount,
			const XalanDOMString&	mimeEncoding,
			const StylesheetRoot*	stylesheet);


	bool
	fileCompare(
			const char*		goldFile,
			const char*		outputFile);

	/** 
	* Utility methods used to perform a DOM Compare
	* @returns boolean
	*/
	bool 
	domCompare(const XalanNode& gold, const XalanNode& doc);

	/** 
	* Utility methods used to diff two Element nodes.
	* @returns boolean.
	*/
	bool 
	diffElement(const XalanNode& gold, const XalanNode& doc);

	/** 
	* Utility methods used to diff two attribute nodes.
	* @returns boolean.
	*/
	bool 
	diffAttr(const XalanNode* gAttr, const XalanNode* dAttr);

	/** 
	* Utility methods used to report Pass/Fail numbers.
	* @returns void.
	*/
	void
	reportPassFail(XMLFileReporter& logfile);

	void
	reportPassFail(XMLFileReporter& logfile, const XalanDOMString& runid);

	void
	analyzeResults(XalanTransformer& xalan, const XalanDOMString& resultsFile);

	static const XalanDOMString&	s_xmlSuffix;

	static const XalanDOMString&	s_pathSep;

private:

	XalanDOMString
	getProgramName(const char* fullName);

	/** 
	* Utility methods used to collect information about compare failures.
	* @returns void.
	*/
	void 
	collectData(
			const char*				errmsg,
			const XalanDOMString&	currentnode,
			const XalanDOMString&	actdata,
			const XalanDOMString&	expdata);

	/** 
	* Utility methods used to report DOM compare errors.
	* @returns void.
	*/
	void
	reportError();

#if defined(NDEBUG)
	void
	debugNodeData(const XalanDOMString&		/* value */) const
	{
	}

	void
	debugNodeData(
			const XalanDOMString&	/* node */,
			const XalanDOMString&	/* value */) const
	{
	}

	void
	debugAttributeData(const XalanDOMString&	/* value */) const
	{
	}

#else

	void
	debugNodeData(const XalanDOMString&		value) const;

	void
	debugNodeData(
			const XalanDOMString&	node,
			const XalanDOMString&	value) const;

	void
	debugAttributeData(const XalanDOMString&	value) const;

#endif

};        // end of class FileUtility



XALAN_CPP_NAMESPACE_END



#endif
