/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#include <cstdio>
#include <cstring>
#include <iostream>
#include <util/StdOut.hpp>

#include <dom/DOM_Node.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NodeList.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/NullPrintWriter.hpp>
#include <DOMSupport/DOMSupportDefault.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <Xslt/XSLTEngineImpl.hpp>
#include <Xslt/StylesheetRoot.hpp>

#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesPlatformSupport/TextFileOutputStream.hpp>
#include <XercesPlatformSupport/NullTextOutputStream.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>

#include <DOMSupport/DOMSupport.hpp>
#include <XMLSupport/Formatter.hpp>

#include <sax/SAXException.hpp>



#include <Xslt/XSLTProcessorEnvSupportDefault.hpp>

/**
 * Print argument options.
 */ 
void printArgOptions(XercesDOMPrintWriter&		writer)
{
	writer.println("xslproc options: ");
	writer.println("		-IN inputXMLURL");
	writer.println("	 [-XSL XSLTransformationURL]");
	writer.println("	 [-OUT outputFileName]");
	writer.println("	 [-E (Do not expand entity refs)]");
	writer.println("	 [-V (Version info)]");
	writer.println("	 [-QC (Quiet Pattern Conflicts Warnings)]");
	writer.println("	 [-Q	(Quiet Mode)]");
	writer.println("	 [-ESCAPE (Which characters to escape {default is <>&\"\'\\r\\n}]");
	writer.println("	 [-INDENT (Control how many spaces to indent {default is 0})]");
	writer.println("	 [-TT (Trace the templates as they are being called.)]");
	writer.println("	 [-TG (Trace each generation event.)]");
	writer.println("	 [-TS (Trace each selection event.)]");
	writer.println("	 [-TTC (Trace the template children as they are being processed.)]");
	writer.println("	 [-TCLASS (TraceListener class for trace extensions.)]");
	writer.println("	 [-VALIDATE (Set whether validation occurs.	Validation is off by default.)]");
	writer.println("	 [-XML (Use XML formatter and add XML header.)]");
	writer.println("	 [-TEXT (Use simple Text formatter.)]");
	writer.println("	 [-HTML (Use HTML formatter.)]");
	writer.println("	 [-PARAM name expression (Set a stylesheet parameter)]");
}

void printChildren(const DOM_Node& theNode, int level=0) 
{
	XMLCh *nodeTypeNames[] =
	{
		L"", L"ELEMENT_NODE", L"ATTRIBUTE_NODE", L"TEXT_NODE", L"CDATA_SECTION_NODE",
		L"ENTITY_REFERENCE_NODE", L"ENTITY_NODE", L"PROCESSING_INSTRUCTION_NODE",
		L"COMMENT_NODE", L"DOCUMENT_NODE", L"DOCUMENT_TYPE_NODE",
		L"DOCUMENT_FRAGMENT_NODE", L"NOTATION_NODE",
	};

	DOM_NodeList nl = theNode.getChildNodes();

	level++;
	for (int i=0; i< nl.getLength(); i++)
	{
		for (int j=1; j< level; j++) printf("\t");
		printf("%d.%d: ", level, i);
		const DOM_Node n = nl.item(i);
		std::basic_string<XMLCh> s(c_wstr(n.getNodeName()));
		s.append(L"(");
		int t = n.getNodeType();
		s.append(nodeTypeNames[t]);
		s.append(L")");
		printf("%-20ws ", s.c_str());
		printf( t==3 ? "= %ws\n": "\n", c_wstr(n.getNodeValue()));
		printChildren(n, level);
	}
}


void xsltMain(int argc, const char* argv[] ) throw(XMLException)
{

/**
 * Command line interface to transform the XML according to 
 * the instructions found in the XSL document.
 *		-in inputXMLURL
 *		-xsl XSLTransformationURL
 *		-out outputFileName
 *		-F (Format output pretty-printed)
 */
	bool doStackDumpOnError = false;
	bool setQuietMode = false;
	
	// @@ This should become a command line switch
	bool shouldWriteXMLHeader = false;
	
	// Runtime.getRuntime().traceMethodCalls(false);	
	// Runtime.getRuntime().traceInstructions(false);
	/**
	* The default diagnostic writer...
	*/
	XMLStdOut				theStdOut;
	XMLStdErr				theStdErr;
	NullTextOutputStream	theNullStream;
	XercesDOMPrintWriter		diagnosticsWriter(theStdErr);
	XercesDOMPrintWriter		dumpWriter(theStdErr);

	
	if(argc < 2)
	{
		printArgOptions(diagnosticsWriter);
	}
	else
	{
		DOMSupportDefault theDOMSupport;
		XercesParserLiaison xmlParserLiaison(theDOMSupport);

		DOMStringPrintWriter pw;

		XPathSupportDefault theXPathSupport(theDOMSupport);
		XSLTProcessorEnvSupportDefault	theXSLProcessorSupport;
		XObjectFactoryDefault theXObjectFactory(theXSLProcessorSupport, theXPathSupport);
		XPathFactoryDefault theXPathFactory( theXObjectFactory, theXSLProcessorSupport,
			theXPathSupport);

		XSLTEngineImpl processor(
				xmlParserLiaison, theXPathSupport,
				theXSLProcessorSupport,
				&theXObjectFactory,
				&theXPathFactory);

		theXSLProcessorSupport.setProcessor(&processor);

		processor.setFormatter(&xmlParserLiaison);

		bool formatOutput = false;
		
		QName mode;
		DOMString inFileName;
		DOMString outFileName;
		DOMString dumpFileName;
		DOMString xslFileName;
		DOMString compiledStylesheetFileNameOut;
		DOMString compiledStylesheetFileNameIn;
		DOMString treedumpFileName;
		bool didSetCR = false;
		bool didSetLF = false;
		bool stripCData = false;
		bool escapeCData = false;
		std::auto_ptr<TextFileOutputStream>	outputFileStream;
		TextOutputStream*					outputStream = &theStdOut;

	
		int outputType = -1;

		for (int i = 1;	i < argc;	i ++) 
		{
			if (!stricmp("-ESCAPE", argv[i])) 
			{
				xmlParserLiaison.setSpecialCharacters(argv[++i]);
			}
			else if (!stricmp("-INDENT", argv[i])) 
			{
				int indentAmount;
				if(((i+1) < argc) && (argv[i+1][0] != '-'))
				{
					indentAmount = atoi( argv[++i] );
				}
				else
				{
					indentAmount = 0;
				}
				xmlParserLiaison.setIndent(indentAmount);
			} 
			else if (!stricmp("-IN", argv[i])) 
			{
				inFileName = argv[++i];
			}
			else if (!stricmp("-OUT", argv[i])) 
			{
				outFileName = argv[++i];
			}
			else if (!stricmp("-XSL", argv[i])) 
			{
				xslFileName = argv[++i];
			}
			else if (!stricmp("-PARAM", argv[i])) 
			{
				DOMString name = argv[++i];
				DOMString expression = argv[++i];
				processor.setStylesheetParam(name, expression);
			}
			else if (!stricmp("-treedump", argv[i])) 
			{
				treedumpFileName = argv[++i];
			}
			else if(!stricmp("-F", argv[i]))
			{
				formatOutput = true;
			}
			else if(!stricmp("-E", argv[i]))
			{
				xmlParserLiaison.SetShouldExpandEntityRefs(false);
			}
			else if(!stricmp("-V", argv[i]))
			{
			}
			else if(!stricmp("-QC", argv[i]))
			{
				processor.setQuietConflictWarnings(true);
			}
			else if(!stricmp("-Q", argv[i]))
			{
				setQuietMode = true;
			}
			else if(!stricmp("-VALIDATE", argv[i]))
			{
				DOMString shouldValidate;
				if(((i+1) < argc) && (argv[i+1][0] != '-'))
				{
					shouldValidate = argv[++i];
				}
				else
				{
					shouldValidate = "yes";
				}
			 
			}
			else if(!stricmp("-PARSER", argv[i]))
			{
				i++;
				// Handled above
			}
			else if(!stricmp("-XML", argv[i]))
			{
				outputType = Formatter::OUTPUT_METH_XML;
			}
			else if(!stricmp("-TEXT", argv[i]))
			{
				outputType = Formatter::OUTPUT_METH_TEXT;
			}
			else if(!stricmp("-HTML", argv[i]))
			{
				outputType = Formatter::OUTPUT_METH_HTML;
			}
			else if(!stricmp("-STRIPCDATA", argv[i]))
			{
				stripCData = true;
			}
			else if(!stricmp("-ESCAPECDATA", argv[i]))
			{
				escapeCData = true;
			}
			else if(!stricmp("-EDUMP", argv[i]))
			{
				doStackDumpOnError = true;
				if(((i+1) < argc) && (argv[i+1][0] != '-'))
				{
					dumpFileName = argv[++i];
				}
			}
		}
		
		// The main XSL transformation occurs here!
			if (! setQuietMode)
				processor.setDiagnosticsOutput( &diagnosticsWriter );
			
							
			StylesheetRoot* stylesheet = 0;
			
			if(0 != xslFileName.length())
			{
				stylesheet = processor.processStylesheet(xslFileName);
			}

			if (length(outFileName) != 0)
			{
				outputFileStream =
					std::auto_ptr<TextFileOutputStream>(new TextFileOutputStream(outFileName));

				outputStream = outputFileStream.get();
			}

			XercesDOMPrintWriter	resultWriter(*outputStream);
			DOMString mimeEncoding("UTF-8");
			DOMString encoding("UTF-8");
			
			FormatterListener* formatter = 0;

			assert(inFileName.length());
			XSLTInputSource theInputSource(c_wstr(inFileName));
			DOM_Node sourceTree = processor.getSourceTreeFromInput(&theInputSource);

			/*
			 * Output the source tree
			 */
			if (0)
			if (! setQuietMode)
				printChildren(sourceTree);

			
			if(0 != stylesheet)
			{
				if(Formatter::OUTPUT_METH_XML == outputType)
				{
					FormatterToXML* fToXML = new FormatterToXML(resultWriter,
							stylesheet->m_version,
							stylesheet->getOutputIndent(),
							xmlParserLiaison.getIndent(),
							mimeEncoding,
							stylesheet->m_mediatype,
							stylesheet->getOutputDoctypeSystem(),
							stylesheet->getOutputDoctypePublic(),
							true,	// xmlDecl
							stylesheet->m_standalone,
							&(stylesheet->getCdataSectionElems()));
					fToXML->m_shouldWriteXMLHeader = shouldWriteXMLHeader;
					fToXML->m_attrSpecialChars = xmlParserLiaison.getSpecialCharacters();
					fToXML->m_stripCData = stripCData;
					fToXML->m_escapeCData = escapeCData;
					formatter = fToXML;
				}
				else if(Formatter::OUTPUT_METH_TEXT == outputType)
				{
					FormatterToText* fToText = new FormatterToText(resultWriter);
					formatter = fToText;
				}
				else if(Formatter::OUTPUT_METH_HTML == outputType)
				{
					FormatterToHTML* fToHTML
						= new FormatterToHTML(resultWriter, 
							stylesheet->m_version,
							stylesheet->getOutputIndent(),
							xmlParserLiaison.getIndent(), mimeEncoding,
							stylesheet->m_mediatype,
							stylesheet->getOutputDoctypeSystem(),
							stylesheet->getOutputDoctypePublic(),
							false,	// xmlDecl
							stylesheet->m_standalone,
							&(stylesheet->getCdataSectionElems()));

					fToHTML->m_attrSpecialChars = xmlParserLiaison.getSpecialCharacters();
					fToHTML->m_stripCData = stripCData;
					formatter = fToHTML;
				}
			}
			XSLTResultTarget* rTreeTarget = 0;
			if(0 == formatter)
			{
				rTreeTarget = new XSLTResultTarget(&resultWriter);
			}
			else
			{
				rTreeTarget = new XSLTResultTarget();
				rTreeTarget->setFormatterListener(formatter);
				xmlParserLiaison.setFormatterListener(formatter);
			}
			

			stylesheet->process(sourceTree, rTreeTarget);

			delete formatter;
			delete rTreeTarget;

	}
}	


int main(int argc, const char* argv[]) throw()
{
	try
	{
		xsltMain(argc, argv);
	}

	catch (XSLException& e)
	{
		std::cout << "\nXSLException ";

		std::string type = DOMStringToStdString(e.getType());

		if (!type.empty())
			std::cout << "Type is : " << type << std::endl;

		std::string msg = DOMStringToStdString(e.getMessage());

		if (!msg.empty())
			std::cout << "Message is : " << msg << std::endl;

		return -1;
	}

	catch (SAXException& e)
	{
		std::cout << "\nSAXException ";

		std::string msg = DOMStringToStdString(e.getMessage());

		if (!msg.empty())
			std::cout << "Message is : " << msg << std::endl;

		return -1;
	}



	catch (XMLException& e)
	{
		std::cout << "\nXMLException ";

		std::string type = DOMStringToStdString(e.getType());

		if (!type.empty())
			std::cout << "Type is : " << type << std::endl;

		std::string msg = DOMStringToStdString(e.getMessage());

		if (!msg.empty())
			std::cout << "Message is : " << msg << std::endl;

		return -1;
	}

	return 0;
}
