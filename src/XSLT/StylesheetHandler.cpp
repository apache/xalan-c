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
#include "StylesheetHandler.hpp"

#include <sax/SaxException.hpp>

#include <XMLSupport/Formatter.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>

#include "ElemLiteralResult.hpp"
#include "ElemText.hpp"
#include "ElemTextLiteral.hpp"
#include "ElemParam.hpp"
#include "ElemEmpty.hpp"
#include "ElemApplyTemplates.hpp"
#include "ElemCallTemplate.hpp"
#include "ElemWithParam.hpp"
#include "ElemForEach.hpp"
#include "ElemApplyImport.hpp"
#include "ElemValueOf.hpp"
#include "ElemNumber.hpp"
#include "ElemIf.hpp"
#include "ElemChoose.hpp"
#include "ElemWhen.hpp"
#include "ElemOtherwise.hpp"
#include "ElemCopyOf.hpp"
#include "ElemCopy.hpp"
#include "ElemText.hpp"
#include "ElemUse.hpp"
#include "ElemElement.hpp"
#include "ElemPI.hpp"
#include "ElemComment.hpp"
#include "ElemMessage.hpp"
#include "ElemAttribute.hpp"
#include "ElemExtensionCall.hpp"

#include "StylesheetRoot.hpp"

#include "ExtensionNSHandler.hpp"



/**
 * This class processes a stylesheet via SAX events, and inits
 * the given stylesheet.	If you need to alter the code in here, 
 * it is not for the faint-of-heart, due to the state tracking 
 * that has to be done due to the SAX event model.
 */

StylesheetHandler::StylesheetHandler(XSLTEngineImpl& processor,	Stylesheet& stylesheetTree) :
	FormatterListener(),
	m_processor(processor),
	m_stylesheet(stylesheetTree),
	m_includeBase(m_stylesheet.getBaseIdentifier()),
	m_pTemplate(0),
	m_pLastPopped(0),
	m_inTemplate(false),
	m_foundStylesheet(false),
	m_foundNotImport(false),
	m_inLXSLTScript(false),
	m_LXSLTScriptBody(),
	m_LXSLTScriptLang(),
	m_LXSLTScriptSrcURL(),
	m_pLXSLTExtensionNSH(0),
	m_pElemStack(new ElemTemplateStackType())
{

}



StylesheetHandler::~StylesheetHandler()
{
	m_pElemStack = 0;
	m_pTemplate = 0;
	m_pLastPopped = 0;
	m_pLXSLTExtensionNSH = 0;
}



/**
 * Receive an object for locating the origin of SAX document events.
 *
 * <p>SAX parsers are strongly encouraged (though not absolutely
 * required) to supply a locator: if it does so, it must supply
 * the locator to the application by invoking this method before
 * invoking any of the other methods in the DocumentHandler
 * interface.</p>
 *
 * <p>The locator allows the application to determine the end
 * position of any document-related event, even if the parser is
 * not reporting an error.  Typically, the application will
 * use this information for reporting its own errors (such as
 * character content that does not match an application's
 * business rules).  The information returned by the locator
 * is probably not sufficient for use with a search engine.</p>
 *
 * <p>Note that the locator will return correct information only
 * during the invocation of the events in this interface.  The
 * application should not attempt to use it at any other time.</p>
 *
 * @param locator An object that can return the location of
 *                any SAX document event.
 * @see org.xml.sax.Locator
 */
void StylesheetHandler::setDocumentLocator(Locator* const locator)
{
	m_processor.m_stylesheetLocatorStack.push_back(locator);
}

/**
 * Receive notification of the beginning of a document.
 *
 * <p>The SAX parser will invoke this method only once, before any
 * other methods in this interface or in DTDHandler (except for
 * setDocumentLocator).</p>
 *
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 */
void StylesheetHandler::startDocument()
{
	m_pendingException = "";

  // No other action for the moment.
}


/**
 * Receive notification of the end of a document.
 *
 * <p>The SAX parser will invoke this method only once, and it will
 * be the last method invoked during the parse.  The parser shall
 * not invoke this method until it has either abandoned parsing
 * (because of an unrecoverable error) or reached the end of
 * input.</p>
 *
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 */
void StylesheetHandler::endDocument()
{
	if(!m_processor.m_stylesheetLocatorStack.empty())
	{
		m_processor.m_stylesheetLocatorStack.pop_back();
	}

	if (!isEmpty(m_pendingException))
	{
		throw SAXException(toCharArray(m_pendingException));
	}
}

/** 
 * See if this is a xmlns attribute, and, if so, process it.
 * 
 * @param attrName Qualified name of attribute.
 * @param atts The attribute list where the element comes from (not used at 
 *      this time).
 * @param which The index into the attribute list (not used at this time).
 * @return True if this is a namespace name.
 */
bool StylesheetHandler::isAttrOK(const DOMString& attrName, const AttributeList& atts, int which)
{
	return m_stylesheet.isAttrOK(attrName, atts, which);
}

/** 
 * Tell whether or not this is a xml:space attribute and, if so, process it.
 * 
 * @param aname The name of the attribute in question.
 * @param atts The attribute list that owns the attribute.
 * @param which The index of the attribute into the attribute list.
 * @return True if this is a xml:space attribute.
 */
bool StylesheetHandler::processSpaceAttr(const DOMString& aname, const AttributeList& atts, int which)
{
	bool isSpaceAttr = equals(aname, "xml:space");

	if(isSpaceAttr)
	{
		const DOMString spaceVal = atts.getValue(which);
		if(equals(spaceVal, "default"))
		{
			m_stylesheet.setDefaultSpaceProcessing(true);
		}
		else if(equals(spaceVal, "preserve"))
		{
			m_stylesheet.setDefaultSpaceProcessing(false);
		}
		else
		{
			throw SAXException("xml:space has an illegal value");
		}
	}

	return isSpaceAttr;
}


/**
 * Receive notification of the beginning of an element.
 *
 * <p>The Parser will invoke this method at the beginning of every
 * element in the XML document; there will be a corresponding
 * endElement() event for every startElement() event (even when the
 * element is empty). All of the element's content will be
 * reported, in order, before the corresponding endElement()
 * event.</p>
 *
 * <p>If the element name has a namespace prefix, the prefix will
 * still be attached.  Note that the attribute list provided will
 * contain only attributes with explicit values (specified or
 * defaulted): #IMPLIED attributes will be omitted.</p>
 *
 * @param name The element type name.
 * @param atts The attributes attached to the element, if any.
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 * @see #endElement
 * @see org.xml.sax.AttributeList
 */
void StylesheetHandler::startElement (const XMLCh* const name, AttributeList& atts)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

	try
	{

	m_whiteSpaceElems.erase(m_whiteSpaceElems.begin(),m_whiteSpaceElems.end());

	Locator* locator = m_processor.m_stylesheetLocatorStack.empty() ? 0 : 
		(m_processor.m_stylesheetLocatorStack.back());
	
	int lineNumber = (0 != locator) ? locator->getLineNumber() : 0;
	int columnNumber = (0 != locator) ? locator->getColumnNumber() : 0;
	
	if(false)
	{
		DOMString id;

		if (locator)
			if (locator->getPublicId())
				id = locator->getPublicId();
			else 
				id = locator->getSystemId();

		assert(locator);
		
//		if(0 != locator)
//			System.out.println(id + "; line " + lineNumber + 	"; "+columnNumber);
	}
	
	// First push namespaces
	m_stylesheet.pushNamespaces(atts);

	DOMString ns = m_stylesheet.getNamespaceFromStack(name);

	int index = indexOf(name,':');

	DOMString localName = (index < 0) ? DOMString(name) : substring(name,index+1);

	ElemTemplateElement* elem = 0;

	unsigned origStackSize = m_pElemStack->size();

	if(startsWith(ns,m_processor.getXSLNameSpaceURLPre()))
	{
		if(!isEmpty(m_processor.getXSLNameSpaceURL()))
	        m_processor.setXSLNameSpaceURL(ns);

		if(false == m_foundStylesheet)
		{
			m_stylesheet.getStylesheetRoot()->initDefaultRule();
			m_stylesheet.setWrapperless(false);
		}
      
		XSLTEngineImpl::AttributeKeysMapType::const_iterator iter=
			XSLTEngineImpl::getElementKeys().find(localName);

		int xslToken = (iter!= XSLTEngineImpl::getElementKeys().end()) ? (*iter).second : -2;

      if(!m_inTemplate)
      {
		  if(m_foundStylesheet && (Constants::ELEMNAME_IMPORT != xslToken))
        {
          m_foundNotImport = true;
        }
        
        switch(xslToken)
        {            
        case Constants::ELEMNAME_TEMPLATE:
          m_pTemplate = new ElemTemplate(m_processor,
                                        m_stylesheet,
                                        name, atts, lineNumber, columnNumber);
          m_pElemStack->push_back(m_pTemplate);
          m_inTemplate = true;
          m_stylesheet.addTemplate(m_pTemplate);
          break;

        case Constants::ELEMNAME_CSSSTYLECONVERSION:
          m_processor.setTranslateCSS(true);
          break;

        case Constants::ELEMNAME_EXTENSION:
          {
            if(!equalsIgnoreCase(ns,m_processor.getXSLT4JNameSpaceURL()))
            {
              m_processor.warn("Old syntax: the functions instruction should use a url of "+m_processor.getXSLT4JNameSpaceURL());
            }
            // m_processor.handleFunctionsInstruction((Element)child);
          }
          break;
          
        case Constants::ELEMNAME_VARIABLE:
        case Constants::ELEMNAME_PARAMVARIABLE:
          {
			  ElemVariable* varelem = (Constants::ELEMNAME_PARAMVARIABLE == xslToken) 
                                   ? new ElemParam(m_processor,
                                                   m_stylesheet,
                                                   name, atts, 
                                                   lineNumber, columnNumber)
                                     : new ElemVariable(m_processor,
                                                        m_stylesheet,
                                                        name, atts, 
                                                        lineNumber, columnNumber);
            m_pElemStack->push_back(varelem);
            m_inTemplate = true; // fake it out
            m_stylesheet.setTopLevelVariable(varelem);
            varelem->setTopLevel(true);
          }
          break;
          
        case Constants::ELEMNAME_LOCALE:
          m_processor.warn("xsl:locale not yet supported!");
          break;
          
        case Constants::ELEMNAME_PRESERVESPACE:
        case Constants::ELEMNAME_STRIPSPACE:
          {
            ElemEmpty nsNode(m_processor, m_stylesheet, name, atts, lineNumber, columnNumber);
            
            int nAttrs = atts.getLength();
            bool foundIt = false;
            for(int i = 0; i < nAttrs; i++)
            {
              const DOMString aname = atts.getName(i);
              if(equals(aname, Constants::ATTRNAME_ELEMENTS))
              {
                foundIt = true;
                StringTokenizer tokenizer(atts.getValue(i), " \t\n\r");
                while(tokenizer.hasMoreTokens())
                {
                  // Use only the root, at least for right now.
                  const DOMString wildcardName = tokenizer.nextToken();
                  
                  /**
                  * Creating a match pattern is too much overhead, but it's a reasonably 
                  * easy and safe way to do this right now.  TODO: Validate the pattern 
                  * to make sure it's a WildcardName.
                  */
                  XPath* matchPat = m_processor.createMatchPattern(wildcardName, nsNode);
                  
                  if(Constants::ELEMNAME_PRESERVESPACE == xslToken)
                  {
                    m_stylesheet.getStylesheetRoot()->m_whitespacePreservingElements.push_back(matchPat);
                  }
                  else
                  {
                    m_stylesheet.getStylesheetRoot()->m_whitespaceStrippingElements.push_back(matchPat);
                  }
                }
              }
              else if(!isAttrOK(aname, atts, i))
              {
				m_stylesheet.error(DOMString(name) + " has an illegal attribute: " + aname);
              }
            }
            if(!foundIt)
            {
				DOMString msg("(StylesheetHandler) " + DOMString(name) +
					" requires a " + Constants::ATTRNAME_ELEMENTS + " attribute!");

				throw SAXException(toCharArray(msg));
            }
          }
          break;
          
        case Constants::ELEMNAME_KEY:
          {
            ElemEmpty nsContext(m_processor, m_stylesheet, name, atts, lineNumber, columnNumber);
            m_stylesheet.processKeyElement(&nsContext, atts);
          }
          break;
          
        case Constants::ELEMNAME_DEFINEATTRIBUTESET:
		{
          m_inTemplate = true; // fake it out
          ElemAttributeSet* attrSet = new ElemAttributeSet(m_processor,
                                                          m_stylesheet,
                                                          name, atts, 
                                                          lineNumber, 
                                                          columnNumber);
          m_pElemStack->push_back(attrSet);
		}
          break;
          
        case Constants::ELEMNAME_INCLUDE:
          processInclude(name, atts);
          break;
          
        case Constants::ELEMNAME_IMPORT:
          processImport(name, atts);
          break;
          
        case Constants::ELEMNAME_OUTPUT:
          m_stylesheet.getStylesheetRoot()->processOutputSpec(name, atts);
          break;
          
        case Constants::ELEMNAME_WITHPARAM:
        case Constants::ELEMNAME_ATTRIBUTE:
        case Constants::ELEMNAME_APPLY_TEMPLATES:
        case Constants::ELEMNAME_USE:
        case Constants::ELEMNAME_CHILDREN:
        case Constants::ELEMNAME_CHOOSE:
        case Constants::ELEMNAME_COMMENT:
        case Constants::ELEMNAME_CONSTRUCT:
        case Constants::ELEMNAME_CONTENTS:
        case Constants::ELEMNAME_COPY:
        case Constants::ELEMNAME_COPY_OF:
        case Constants::ELEMNAME_DISPLAYIF:
        case Constants::ELEMNAME_EVAL:
        case Constants::ELEMNAME_EXPECTEDCHILDREN:
        case Constants::ELEMNAME_FOREACH:
        case Constants::ELEMNAME_IF:
        case Constants::ELEMNAME_CALLTEMPLATE:
        case Constants::ELEMNAME_MESSAGE:
        case Constants::ELEMNAME_NUMBER:
        case Constants::ELEMNAME_OTHERWISE:
        case Constants::ELEMNAME_PI:
        case Constants::ELEMNAME_REMOVEATTRIBUTE:
        case Constants::ELEMNAME_SORT:
        case Constants::ELEMNAME_TEXT:
        case Constants::ELEMNAME_VALUEOF:
        case Constants::ELEMNAME_WHEN:
        case Constants::ELEMNAME_ELEMENT:
        case Constants::ELEMNAME_COUNTER:
        case Constants::ELEMNAME_COUNTERS:
        case Constants::ELEMNAME_COUNTERINCREMENT:
        case Constants::ELEMNAME_COUNTERRESET:
        case Constants::ELEMNAME_COUNTERSCOPE:
        case Constants::ELEMNAME_APPLY_IMPORTS:
		{

			DOMString msg("(StylesheetHandler) " + DOMString(name) + " not allowed inside a stylesheet!");
			throw SAXException(toCharArray(msg));
		}
          // break;

        case Constants::ELEMNAME_STYLESHEET:
		{
          m_stylesheet.setWrapperless(false);
          m_foundStylesheet = true;
          int nAttrs = atts.getLength();
//          bool didSpecifiyIndent = false;	//doesn't seem to be used
          for(int i = 0; i < nAttrs; i++)
          {
            const DOMString	aname = atts.getName(i);
            if(equals(aname, "result-ns"))
            {
              throw SAXException("result-ns no longer supported!  Use xsl:output instead.");
            }
            else if(equals(aname, Constants::ATTRNAME_DEFAULTSPACE))
            {
              throw SAXException("default-space no longer supported!  Use xsl:strip-space or xsl:preserve-space instead.");
            }
            else if(equals(aname, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
            {
              // BEGIN SANJIVA CODE
              StringTokenizer tokenizer(atts.getValue (i), " \t\n\r", false);

              while(tokenizer.hasMoreTokens ()) 
              {
                const DOMString prefix = tokenizer.nextToken ();
                // SANJIVA: ask Scott: is the line below correct?
                const DOMString extns = m_stylesheet.getNamespaceForPrefixFromStack (prefix);
                ExtensionNSHandler* nsh = new ExtensionNSHandler (m_processor, extns);
                m_stylesheet.addExtensionNamespace(extns, nsh);
              }
              // END SANJIVA CODE
            }
            else if(equals(aname, "id"))
            {
              //
            }
            else if(equals(aname, "indent-result"))
            {
              throw SAXException("indent-result no longer supported!  Use xsl:output instead.");
            }
            else if(equals(aname, "version"))
            {
              const DOMString versionStr = atts.getValue(i);
			  Stylesheet::setXSLTVerDeclared(DOMStringToDouble(versionStr));
            }
            else if(!(isAttrOK(aname, atts, i) || processSpaceAttr(aname, atts, i)))
            {
              if(false == m_stylesheet.isWrapperless())
			  {
				DOMString msg("(StylesheetHandler) " + DOMString(name) + 
					" has an illegal attribute: " + aname);

				throw SAXException(toCharArray(msg));
			  }
            }
            else if(startsWith(aname,"xmlns:"))
			{
// rcw to do: in the final recommendation version is specified as an attribute
#if 0
              const DOMString nsDeclVal = atts.getValue(i);
              if(startsWith(nsDeclVal,m_processor.getXSLNameSpaceURLPre()))
              {
				int lenOfPre = m_processor.getXSLNameSpaceURLPre().length();
				const DOMString versionStr = substring(nsDeclVal,lenOfPre+1);
				if(versionStr.length() > 1)
				{
					Stylesheet::setXSLTVerDeclared(DOMStringToDouble(versionStr));
				}
              }
#endif
            }

            if(!m_stylesheet.getNamespaces().empty())
            {
              m_stylesheet.setNamespaceDecls(m_stylesheet.getNamespaces().back());
            }

            /*
            default:
            if((null != ns) && (ns.equalsIgnoreCase(m_processor.m_XSLNameSpaceURL) || 
            ns.equalsIgnoreCase(m_processor.m_XML4JNameSpaceURL)))
            {
            m_processor.warn(name
            +" unknown XSL instruction inside context of the stylesheet element!");
            }
            // be tolerant of other namespaces
            break;
            */
          }
        }  
          break;
        default:
		{
			DOMString msg("Unknown XSL element: " + localName);
			throw SAXException(toCharArray(msg));
		}
        }
      }
      else
      {
        switch(xslToken)
        {            
        case Constants::ELEMNAME_APPLY_TEMPLATES:
          elem = new ElemApplyTemplates(m_processor,
                                        m_stylesheet,
                                        name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_CALLTEMPLATE:
          elem = new ElemCallTemplate(m_processor,
                                      m_stylesheet,
                                      name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_WITHPARAM:
          elem = new ElemWithParam(m_processor,
                                   m_stylesheet,
                                   name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_FOREACH:
          elem = new ElemForEach(m_processor,
                                 m_stylesheet,
                                 name, atts, lineNumber, columnNumber, true);
          break;
          
        case Constants::ELEMNAME_SORT:
		{
            ElemForEach* foreach = dynamic_cast<ElemForEach*>(m_pElemStack->back());

            ElemSort* sortElem = new ElemSort(m_processor,
                                             m_stylesheet,
                                             name, atts, lineNumber, columnNumber);
            
            foreach->getSortElems().push_back(sortElem);
            sortElem->setParentNode(foreach);
		}
          break;
          
        case Constants::ELEMNAME_APPLY_IMPORTS:
          elem = new ElemApplyImport(m_processor,
                                     m_stylesheet,
                                     name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_VALUEOF:
          elem = new ElemValueOf(m_processor,
                                 m_stylesheet,
                                 name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_NUMBER:
          elem = new ElemNumber(m_processor,
                                m_stylesheet,
                                name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_VARIABLE:
          elem = new ElemVariable(m_processor,
                                  m_stylesheet,
                                  name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_PARAMVARIABLE:
          elem = new ElemParam(m_processor,
                               m_stylesheet,
                               name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_IF:
          elem = new ElemIf(m_processor,
                            m_stylesheet,
                            name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_CHOOSE:
          elem = new ElemChoose(m_processor,
                                m_stylesheet,
                                name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_WHEN:
          {
            ElemTemplateElement* parent = m_pElemStack->back();
            if(Constants::ELEMNAME_CHOOSE == parent->getXSLToken())
            {
              ElemTemplateElement* lastChild = dynamic_cast<ElemTemplateElement*>(parent->getLastChild());
              if((0 == lastChild) || 
                 (Constants::ELEMNAME_WHEN == lastChild->getXSLToken()))
              {
                elem = new ElemWhen(m_processor,
                                    m_stylesheet,
                                    name, atts, lineNumber, columnNumber);
              }
              else
              {
                throw SAXException("(StylesheetHandler) misplaced xsl:when!");
              }
            }
            else
            {
              throw SAXException("(StylesheetHandler) xsl:when not parented by xsl:choose!");
            }
          }
          break;
          
        case Constants::ELEMNAME_OTHERWISE:
          {
            ElemTemplateElement* parent = m_pElemStack->back();
            if(Constants::ELEMNAME_CHOOSE == parent->getXSLToken())
            {
              ElemTemplateElement* lastChild = dynamic_cast<ElemTemplateElement*>(parent->getLastChild());
              if((0 == lastChild) || 
                 (Constants::ELEMNAME_WHEN == lastChild->getXSLToken()))
              {
                elem = new ElemOtherwise(m_processor,
                                         m_stylesheet,
                                         name, atts, lineNumber, columnNumber);
              }
              else
              {
                throw SAXException("(StylesheetHandler) misplaced xsl:otherwise!");
              }
            }
            else
            {
              throw SAXException("(StylesheetHandler) xsl:otherwise not parented by xsl:choose!");
            }
          }
          break;

        case Constants::ELEMNAME_COPY_OF:
          elem = new ElemCopyOf(m_processor,
                                m_stylesheet,
                                name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_COPY:
          elem = new ElemCopy(m_processor,
                              m_stylesheet,
                              name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_TEXT:
          // Just push the element on the stack to signal
          // that space should be preserved.
          m_pElemStack->push_back(new ElemText(m_processor,
                                    m_stylesheet,
                                    name, atts, lineNumber, columnNumber));
          break;

        case Constants::ELEMNAME_USE:
          elem = new ElemUse(m_processor,
                             m_stylesheet,
                             name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_ATTRIBUTE:
          elem = new ElemAttribute(m_processor,
                                   m_stylesheet,
                                   name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_ELEMENT:
          elem = new ElemElement(m_processor,
                                 m_stylesheet,
                                 name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_PI:
          elem = new ElemPI(m_processor,
                            m_stylesheet,
                            name, atts, lineNumber, columnNumber);
          break;

        case Constants::ELEMNAME_COMMENT:
          elem = new ElemComment(m_processor,
                                 m_stylesheet,
                                 name, atts, lineNumber, columnNumber);
          break;
          
        case Constants::ELEMNAME_MESSAGE:
          elem = new ElemMessage(m_processor,
                                 m_stylesheet,
                                 name, atts, lineNumber, columnNumber);

          break;
          
        case Constants::ELEMNAME_TEMPLATE:
        case Constants::ELEMNAME_LOCALE:
        case Constants::ELEMNAME_DEFINEATTRIBUTESET:
        case Constants::ELEMNAME_DEFINESCRIPT:
        case Constants::ELEMNAME_EXTENSION:
        case Constants::ELEMNAME_EXTENSIONHANDLER:
        case Constants::ELEMNAME_KEY:
        case Constants::ELEMNAME_IMPORT:
        case Constants::ELEMNAME_INCLUDE:
        case Constants::ELEMNAME_PRESERVESPACE:
        case Constants::ELEMNAME_STRIPSPACE:
		{
			DOMString msg("(StylesheetHandler) " + DOMString(name) + " is not allowed inside a template!");
			throw SAXException(toCharArray(msg));
			break;
		}
        default:
          // If this stylesheet is declared to be of a higher version than the one
			  // supported, don't flag an error.
			if(XSLTEngineImpl::getXSLTVerSupported() < Stylesheet::getXSLTVerDeclared())
			{
				DOMString msg("Unknown XSL element: " + localName);
				throw SAXException(toCharArray(msg));
			}
        }
      }
    }
    // BEGIN SANJIVA CODE
    else if (!m_inTemplate && startsWith(ns,m_processor.getXSLT4JNameSpaceURL()))
    {
      if (localName.equals ("component")) 
      {
        DOMString prefix;
        DOMString elements;
        DOMString functions;

        int nAttrs = atts.getLength ();
        for (int i = 0; i < nAttrs; i++) 
        {
          DOMString aname = atts.getName (i);
          if (aname.equals ("prefix")) 
          {
            prefix = atts.getValue (i);
          }
          else if (aname.equals ("elements")) 
          {
            elements = atts.getValue (i);
          }
          else if (aname.equals ("functions")) 
          {
            functions = atts.getValue (i);
          }
          else if(!isAttrOK(aname, atts, i))
          {
            m_stylesheet.error(DOMString(name) + " has an illegal attribute: " + aname);
          }
        }
        if (isEmpty(prefix)) 
        {
			DOMString msg("StylesheetHandler) " + DOMString(name) + " attribute 'prefix' is missing");
			throw SAXException(toCharArray(msg));
        }
        // SCOTT: is the line below correct?
        DOMString extns = m_stylesheet.getNamespaceForPrefixFromStack (prefix);
        ExtensionNSHandler* nsh = m_stylesheet.lookupExtensionNSHandler(extns);
        if (nsh == 0) 
        {
			DOMString msg("(StylesheetHandler) " + DOMString(name) + " extension namespace prefix '" + prefix + "' unknown");
			throw SAXException(toCharArray(msg));
        }
        if (!isEmpty(elements)) 
        {
          nsh->setElements(elements);
        }
        if (!isEmpty(functions)) 
        {
          nsh->setFunctions(functions);
        }
        m_pLXSLTExtensionNSH = nsh; // hang on to it for processing 
        // endElement on lxslt:script
      }
      else if (localName.equals ("script")) 
      {
        // process this in end element so that I can see whether I had 
        // a body as well. The default pushing logic will save the 
        // attributes for me. The body will be accumulated into the
        // following string buffer
        m_inLXSLTScript = true;
        m_LXSLTScriptBody = DOMString();
        int nAttrs = atts.getLength ();
        for (int i = 0; i < nAttrs; i++) 
        {
          DOMString aname = atts.getName (i);
          if (aname.equals ("lang")) 
          {
            m_LXSLTScriptLang = atts.getValue (i);
          }
          else if (aname.equals ("src")) 
          {
            m_LXSLTScriptSrcURL = atts.getValue (i);
          }
          else if(!isAttrOK(aname, atts, i))
          {
            m_stylesheet.error(DOMString(name) + " has an illegal attribute: " + aname);
          }
        }
      }
      else 
      {
        // other xslt4j: element. Not my business.
      }
    }
    // END SANJIVA CODE
    else
    {
      if(!m_inTemplate && !m_foundStylesheet)
      {
        elem = initWrapperless(name, atts, lineNumber, columnNumber);
      }
      else
      {
        // BEGIN SANJIVA CODE
        // is this an extension element call?
        ExtensionNSHandler* nsh = 0;
        if (!isEmpty(ns) && 
            ((nsh = m_stylesheet.lookupExtensionNSHandler (ns)) != 0)) 
        {
          elem = new ElemExtensionCall (m_processor,
                                        m_stylesheet,
                                        *nsh,
                                        name,
                                        localName,
                                        atts, lineNumber, columnNumber);
        }
        else 
        {
          elem = new ElemLiteralResult(m_processor,
                                       m_stylesheet,
                                       name, 
                                       atts, lineNumber, columnNumber);
        }
        // BEGIN SANJIVA CODE
      }
      // END SANJIVA CODE
    }
    if(m_inTemplate && (0 != elem))
    {
      if(!m_pElemStack->empty())
      {
        ElemTemplateElement* parent = m_pElemStack->back();
        parent->appendChild(elem);
      }
      m_pElemStack->push_back(elem);
    }

    // If for some reason something didn't get pushed, push an empty 
    // object.
    if(origStackSize == m_pElemStack->size())
    {
      m_pElemStack->push_back(new ElemEmpty(m_processor,
                                 m_stylesheet,
                                 name, atts, lineNumber, columnNumber));
    }

	} // end try

	// Here's the story.  startElement throws exceptions for certain malformed constructs.  These
	// exceptions need to reach the end user.  But the parser eats all exceptions and we lose
	// the error messages and exit silently.  So, I'll eat the exceptions first, store the message
	// and then throw the exception during endDocument
	catch(SAXException& e)
	{
		m_pendingException = e.getMessage();

	}
}


/**
 * Init the wrapperless template
 */

ElemTemplateElement* StylesheetHandler::initWrapperless (const DOMString& name,
	const AttributeList& atts, int lineNumber, int columnNumber)
{
	m_stylesheet.getStylesheetRoot()->initDefaultRule();

	AttributeListImpl templateAttrs;

	templateAttrs.addAttribute(c_wstr("name"), c_wstr("CDATA"), c_wstr("simple"));

	m_pTemplate = new ElemTemplate(m_processor, m_stylesheet, "xsl:template", 
		templateAttrs, lineNumber, columnNumber);

	ElemTemplateElement* pElem = new ElemLiteralResult(m_processor,
		m_stylesheet, name,  atts, lineNumber, columnNumber);

	m_pTemplate->appendChild(pElem);
	m_inTemplate = true;
	
	m_stylesheet.setWrapperlessTemplate(m_pTemplate);

	m_foundStylesheet = true;
	m_stylesheet.setWrapperless(true);

	if(name.equals("HTML"))
	{
		m_stylesheet.getStylesheetRoot()->setIndentResult(true);
		m_stylesheet.getStylesheetRoot()->setOutputMethod(Formatter::OUTPUT_METH_HTML);
	}

	return pElem;
}


/**
 * Process xsl:import.
 */
void StylesheetHandler::processImport(const DOMString& name, const AttributeList& atts)
{
	int nAttrs = atts.getLength();
	bool foundIt = false;

	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname = atts.getName(i);

		if(aname.equals(Constants::ATTRNAME_HREF))
		{
			foundIt = true;
			
			if(m_foundNotImport)
			{
				throw SAXException("Imports can only occur as the first elements in the stylesheet!");
			}
			
			const DOMString saved_XSLNameSpaceURL = m_processor.getXSLNameSpaceURL();
			
			const DOMString href = atts.getValue(i);

			const URL& hrefUrl = *(m_processor.getURLFromString(href, m_stylesheet.getBaseIdentifier()));

			StylesheetRoot::ImportStackType& importStack = m_stylesheet.getStylesheetRoot()->getImportStack();


			if(stackContains(importStack, hrefUrl))
			{
				DOMString msg(DOMString(hrefUrl.getURL()) + " is directly or indirectly importing itself!");
				throw SAXException(toCharArray(msg));
			}
			
			importStack.push_back(hrefUrl);
			
	
			Stylesheet* pImportedStylesheet = new Stylesheet(
				*(m_stylesheet.getStylesheetRoot()), 
				&m_processor, 
				m_stylesheet.getBaseIdentifier());

			StylesheetHandler tp(m_processor, *pImportedStylesheet);
				
			pImportedStylesheet->setBaseIdentifier(hrefUrl.getURL());

			m_processor.parseXML(hrefUrl, &tp, DOM_UnimplementedDocument(pImportedStylesheet));
			
			// I'm going to insert the elements in backwards order, 
			// so I can walk them 0 to n.
			m_stylesheet.getImports().insert(m_stylesheet.getImports().begin(),
				pImportedStylesheet);
			
			importStack.pop_back();
			
			m_processor.setXSLNameSpaceURL(saved_XSLNameSpaceURL);
		}
		else if(!isAttrOK(aname, atts, i))
		{
			m_stylesheet.error(name + " has an illegal attribute: " + aname);
		}
	}
	if(!foundIt)
	{
		DOMString msg("Could not find href attribute for " + name);
		throw SAXException(toCharArray(msg));
	}
}

/**
 * Test to see if the stack contains the given URL.
 */

bool StylesheetHandler::stackContains(const Stylesheet::URLStackType& stack, 
	const URL& url) const
{
	int n = stack.size();
	bool contains = false;
	for(int i = 0; i < n; i++)
	{
		URL url2 = stack[i];
		if(url2 == url)
		{
			contains = true;
			break;
		}
	}
	return contains;	
}


/**
 * Process xsl:include.
 */
void StylesheetHandler::processInclude(const DOMString& name, const AttributeList& atts)
{
	int nAttrs = atts.getLength();
	bool foundIt = false;

	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname = atts.getName(i);
		if(equals(aname, Constants::ATTRNAME_HREF))
		{
			foundIt = true;
			
			// Save state, so this class can be reused.
			ElemTemplateStackType* saved_pElemStack = m_pElemStack;
			m_pElemStack = new ElemTemplateStackType();

			ElemTemplate* saved_pTemplate = m_pTemplate;
			m_pTemplate = 0;

			ElemTemplateElement* saved_pLastPopped = m_pLastPopped;
			m_pLastPopped = 0;

			bool saved_inTemplate = m_inTemplate;
			m_inTemplate = false;

			bool saved_foundStylesheet = m_foundStylesheet;
			m_foundStylesheet = false;

			DOMString saved_XSLNameSpaceURL = m_processor.getXSLNameSpaceURL();

			bool saved_foundNotImport = m_foundNotImport;
			m_foundNotImport = false;
			
			const DOMString href = atts.getValue(i);

			const URL* hrefUrl = m_processor.getURLFromString(href, m_stylesheet.getIncludeStack().back().getURL());
			
			if(stackContains(m_stylesheet.getIncludeStack(), *hrefUrl))
			{
				DOMString msg(DOMString(hrefUrl->getURL()) + " is directly or indirectly including itself!");
				throw SAXException(toCharArray(msg));
			}
			
			m_stylesheet.getIncludeStack().push_back(*hrefUrl);

			m_processor.parseXML(*hrefUrl, this, DOM_UnimplementedDocument(&m_stylesheet));
			
			m_stylesheet.getIncludeStack().pop_back();
			
			delete m_pElemStack;
			m_pElemStack = saved_pElemStack;
			m_pTemplate = saved_pTemplate;
			m_pLastPopped = saved_pLastPopped;
			m_inTemplate = saved_inTemplate;
			m_foundStylesheet = saved_foundStylesheet;
			m_processor.setXSLNameSpaceURL(saved_XSLNameSpaceURL);
			m_foundNotImport = saved_foundNotImport;
		}
		else if(!isAttrOK(aname, atts, i))
		{
			m_stylesheet.error(name+ " has an illegal attribute: " + aname);
		}
	}
	if(!foundIt)
	{
		DOMString msg("Could not find href attribute for " + DOMString(name));
		throw SAXException(toCharArray(msg));
	}
}


/**
 * Receive notification of the end of an element.
 *
 * <p>The SAX parser will invoke this method at the end of every
 * element in the XML document; there will be a corresponding
 * startElement() event for every endElement() event (even when the
 * element is empty).</p>
 *
 * <p>If the element name has a namespace prefix, the prefix will
 * still be attached to the name.</p>
 *
 * @param name The element type name
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 */
void StylesheetHandler::endElement(const XMLCh* const name) 
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

	m_stylesheet.popNamespaces();

	m_pLastPopped = m_pElemStack->back();
	m_pElemStack->pop_back();
	m_pLastPopped->setFinishedConstruction(true);

	int tok = m_pLastPopped->getXSLToken();

	if(Constants::ELEMNAME_TEMPLATE == tok)
	{
		m_inTemplate = false;
	}
	else if((Constants::ELEMNAME_PARAMVARIABLE == tok) ||
		Constants::ELEMNAME_VARIABLE == tok)
	{
		ElemVariable* var = dynamic_cast<ElemVariable *>(m_pLastPopped);
		if(var->isTopLevel())
		{
			// Top-level param or variable
			m_inTemplate = false;
		}
	}
	else if(Constants::ELEMNAME_DEFINEATTRIBUTESET == tok)
	{
		m_inTemplate = false;
	}
	// BEGIN SANJIVA CODE
	if (m_inLXSLTScript) 
	{
		if (isEmpty(m_LXSLTScriptLang)) 
		{
			DOMString msg(DOMString(name) + " attribute \'lang\' is missing");
			throw SAXException(toCharArray(msg));
		}
		if (m_pLXSLTExtensionNSH == 0) 
		{
			DOMString msg("(StylesheetHandler) misplaced " + DOMString(name) + " element?? Missing container element " + "'component'");
			throw SAXException(toCharArray(msg));
		}

		m_pLXSLTExtensionNSH->setScript(m_LXSLTScriptLang, m_LXSLTScriptSrcURL, m_LXSLTScriptBody);
		
		// reset state
		m_inLXSLTScript = false;
		m_LXSLTScriptLang = 0;
		m_LXSLTScriptSrcURL = 0;
		m_LXSLTScriptBody = 0;
		m_pLXSLTExtensionNSH = 0;
	}
	// END SANJIVA CODE
}

/**
 * Receive notification of character data.
 *
 * <p>The Parser will call this method to report each chunk of
 * character data.  SAX parsers may return all contiguous character
 * data in a single chunk, or they may split it into several
 * chunks; however, all of the characters in any single event
 * must come from the same external entity, so that the Locator
 * provides useful information.</p>
 *
 * <p>The application must not attempt to read from the array
 * outside of the specified range.</p>
 *
 * <p>Note that some parsers will report whitespace using the
 * ignorableWhitespace() method rather than this one (validating
 * parsers must do so).</p>
 *
 * @param ch The characters from the XML document.
 * @param start The start position in the array.
 * @param length The number of characters to read from the array.
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 * @see #ignorableWhitespace 
 * @see org.xml.sax.Locator
 */
void StylesheetHandler::characters (const XMLCh* const chars, const unsigned int length)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

	if(m_inTemplate)
	{
		ElemTemplateElement* parent = m_pElemStack->back();
		bool preserveSpace = false;
		bool disableOutputEscaping = false;

		if(Constants::ELEMNAME_TEXT == parent->getXSLToken())
		{
			disableOutputEscaping = (dynamic_cast<ElemText*>(parent))->getDisableOutputEscaping();
			parent = (*m_pElemStack)[m_pElemStack->size()-2];
			preserveSpace = true;
		}

		Locator* locator = (m_processor.m_stylesheetLocatorStack.size() == 0)
			? 0 : m_processor.m_stylesheetLocatorStack.back();

		int lineNumber = (0 != locator) ? locator->getLineNumber() : 0;
		int columnNumber = (0 != locator) ? locator->getColumnNumber() : 0;

		ElemTextLiteral *elem = new ElemTextLiteral(m_processor,
			m_stylesheet,
			chars, 0, length, 
			true, preserveSpace, 
			disableOutputEscaping,
			lineNumber, columnNumber);

		bool isWhite = isWhiteSpace(chars, 0, length);

		if(preserveSpace || (!preserveSpace && !isWhite))
		{
			while(!m_whiteSpaceElems.empty())
			{
				ElemTextLiteral* whiteElem = m_whiteSpaceElems.back();
				m_whiteSpaceElems.pop_back();

				parent->appendChild(whiteElem);
			}
			parent->appendChild(elem);
			elem=0;

		}
		else if(isWhite)
		{
			bool shouldPush = true;
			NodeImpl* last = parent->getLastChild();
			if(0 != last)
			{
				ElemTemplateElement* lastElem = dynamic_cast<ElemTemplateElement *>(last);
				if(Constants::ELEMNAME_TEXTLITERALRESULT == lastElem->getXSLToken() &&
					!(dynamic_cast<ElemTextLiteral *>(lastElem))->isPreserveSpace())
				{
					parent->appendChild(elem);
					shouldPush = false;
				}
			}
			if(shouldPush)
			{
				m_whiteSpaceElems.push_back(elem);
				elem=0;
			}
		}

		delete elem;
	}
	// BEGIN SANJIVA CODE
	else if (m_inLXSLTScript)
	{
		DOMString tmpStr(chars, length);
		append(m_LXSLTScriptBody,tmpStr);
	}
	// END SANJIVA CODE
	// TODO: Flag error if text inside of stylesheet


}

/**
 * Receive notification of cdata.
 *
 * <p>The Parser will call this method to report each chunk of
 * character data.  SAX parsers may return all contiguous character
 * data in a single chunk, or they may split it into several
 * chunks; however, all of the characters in any single event
 * must come from the same external entity, so that the Locator
 * provides useful information.</p>
 *
 * <p>The application must not attempt to read from the array
 * outside of the specified range.</p>
 *
 * <p>Note that some parsers will report whitespace using the
 * ignorableWhitespace() method rather than this one (validating
 * parsers must do so).</p>
 *
 * @param ch The characters from the XML document.
 * @param start The start position in the array.
 * @param length The number of characters to read from the array.
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 * @see #ignorableWhitespace 
 * @see org.xml.sax.Locator
 */
void StylesheetHandler::cdata(const XMLCh* const chars, const unsigned int length)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

	if(m_inTemplate)
	{
		ElemTemplateElement* parent = m_pElemStack->back();
		bool preserveSpace = false;
		bool disableOutputEscaping = false;

		if(Constants::ELEMNAME_TEXT == parent->getXSLToken())
		{
			disableOutputEscaping = (static_cast<ElemText*>(parent))->getDisableOutputEscaping();
			parent = (*m_pElemStack)[m_pElemStack->size()-2];
			preserveSpace = true;
		}
		Locator* locator = (m_processor.m_stylesheetLocatorStack.size()==0) 
			? 0 : m_processor.m_stylesheetLocatorStack.back();

		int lineNumber = (0 != locator) ? locator->getLineNumber() : 0;
		int columnNumber = (0 != locator) ? locator->getColumnNumber() : 0;

		ElemTextLiteral* elem = new ElemTextLiteral(m_processor,
			m_stylesheet,
			chars, 0, length, 
			true, preserveSpace, 
			disableOutputEscaping,
			lineNumber, columnNumber);

		bool isWhite = isWhiteSpace(chars, 0, length);

		if(preserveSpace || (!preserveSpace && !isWhite))
		{
			while(!m_whiteSpaceElems.empty())
			{
				ElemTextLiteral *whiteElem = m_whiteSpaceElems.back();
				m_whiteSpaceElems.pop_back();

				parent->appendChild(whiteElem);
			}
			parent->appendChild(elem);
		}
		else if(isWhite)
		{
			bool shouldPush = true;
			NodeImpl* last = parent->getLastChild();
			if(0 != last)
			{
				ElemTemplateElement* lastElem = dynamic_cast<ElemTemplateElement*>(last);

				if(Constants::ELEMNAME_TEXTLITERALRESULT == lastElem->getXSLToken() &&
					!dynamic_cast<ElemTextLiteral*>(lastElem)->isPreserveSpace())
				{
					parent->appendChild(elem);
					shouldPush = false;
				}
			}
			if(shouldPush)
				m_whiteSpaceElems.push_back(elem);
		}
	}
	// BEGIN SANJIVA CODE
	else if (m_inLXSLTScript)
	{
		append(m_LXSLTScriptBody, chars);
	}
	// END SANJIVA CODE
	// TODO: Flag error if text inside of stylesheet
}

/**
 * Receive notification of ignorable whitespace in element content.
 *
 * <p>Validating Parsers must use this method to report each chunk
 * of ignorable whitespace (see the W3C XML 1.0 recommendation,
 * section 2.10): non-validating parsers may also use this method
 * if they are capable of parsing and using content models.</p>
 *
 * <p>SAX parsers may return all contiguous whitespace in a single
 * chunk, or they may split it into several chunks; however, all of
 * the characters in any single event must come from the same
 * external entity, so that the Locator provides useful
 * information.</p>
 *
 * <p>The application must not attempt to read from the array
 * outside of the specified range.</p>
 *
 * @param ch The characters from the XML document.
 * @param start The start position in the array.
 * @param length The number of characters to read from the array.
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 * @see #characters
 */
void StylesheetHandler::ignorableWhitespace (const XMLCh* const /*chars*/, const unsigned int /*length*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

  // Ignore!
}


/**
 * Receive notification of a processing instruction.
 *
 * <p>The Parser will invoke this method once for each processing
 * instruction found: note that processing instructions may occur
 * before or after the main document element.</p>
 *
 * <p>A SAX parser should never report an XML declaration (XML 1.0,
 * section 2.8) or a text declaration (XML 1.0, section 4.3.1)
 * using this method.</p>
 *
 * @param target The processing instruction target.
 * @param data The processing instruction data, or null if
 *        none was supplied.
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 */
void StylesheetHandler::processingInstruction (const XMLCh* const /*target*/, const XMLCh* const /*data*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

  // No action for the moment.
}

/**
 * Called when a Comment is to be constructed.
 * @param   data  The comment data.
 * @exception org.xml.sax.SAXException Any SAX exception, possibly
 *            wrapping another exception.
 */
void StylesheetHandler::comment(const XMLCh* const /*data*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

  // No action for the moment.
}

/**
 * Receive notification of a entityReference.
 */
void StylesheetHandler::entityReference(const XMLCh* const /*name*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (!isEmpty(m_pendingException))
		return;

  // No action for the moment.
}



