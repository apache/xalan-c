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
#if !defined(XMLPARSERLIAISON_HEADER_GUARD_1357924680)
#define XMLPARSERLIAISON_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_DECLARE_XERCES_CLASS(DocumentHandler)
XALAN_DECLARE_XERCES_CLASS(EntityResolver)
XALAN_DECLARE_XERCES_CLASS(ErrorHandler)
XALAN_DECLARE_XERCES_CLASS(InputSource)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DocumentHandler	DocumentHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER EntityResolver	EntityResolverType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler		ErrorHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER InputSource		InputSourceType;

class ExecutionContext;
class FormatterListener;
class XalanAttr;
class XalanDocument;
class XalanElement;



class XALAN_XMLSUPPORT_EXPORT XMLParserLiaison
{
    
public:

	XMLParserLiaison();

	virtual
	~XMLParserLiaison();

	// These interfaces are inherited from Resettable...

	virtual void
	reset() = 0;

	// These interfaces are new to XMLParserLiaison

	virtual ExecutionContext*
	getExecutionContext() const = 0;

    virtual MemoryManagerType&
    getMemoryManager() = 0;

	virtual void
	setExecutionContext(ExecutionContext&	theContext) = 0;

	/**
	 * Parse the text pointed at by the reader as XML, and return a DOM
	 * Document interface.  It is recommended that you pass in some sort of
	 * recognizable name, such as the filename or URI, with which the reader
	 * can be recognized if the parse fails.
	 *
	 * The liaison owns the XalanDocument instance, and will delete it when
	 * when asked (see DestroyDocument()), or when the liaison is reset, or
	 * goes out of scope.
	 *
	 * @param reader     stream that should hold valid XML
	 * @param identifier used for diagnostic purposes only, some sort of
	 *                   identification for error reporting, default an empty
	 *                   string
	 * @return DOM document created
	 */
	virtual XalanDocument*
	parseXMLStream(
			const InputSourceType&	inputSource,
			const XalanDOMString&	identifier) = 0;

	/**
	 * Parse the text pointed at by the reader as XML. It is recommended that
	 * you pass in some sort of recognizable name, such as the filename or URI,
	 * with which the reader can be recognized if the parse fails.
	 *
	 * @param inputSource input source that should hold valid XML
	 * @param handler        instance of a DocumentHandler
	 * @param identifier     used for diagnostic purposes only, some sort of
	 *                       identification for error reporting, default an
	 *                       empty string
	 */
	virtual void
	parseXMLStream(
			const InputSourceType&	inputSource,
			DocumentHandlerType&	handler,
			const XalanDOMString&	identifier) = 0;

	/**
	 * Destroy the supplied XalanDocument instance.  It must be an instance that
	 * was created by a previous call to parseXMLStream().
	 *
	 * @param theDocument The XalanDocument instance to destroy.
	 */
	virtual void
	destroyDocument(XalanDocument*	theDocument) = 0;

	/**
	 * Get the amount to indent when indent-result="yes".
	 *
	 * @deprecated
	 *
	 * @return number of characters to indent
	 */
	virtual int
	getIndent() const = 0;

	/**
	 * Set the amount to indent when indent-result="yes".
	 *
	 * @deprecated
	 *
	 * @param i number of characters to indent
	 */
	virtual void
	setIndent(int	i) = 0;

	/**
	 * Get whether or not validation will be performed.  Validation is off by
	 * default.
	 *
	 * @return true to perform validation
	 */
	virtual bool
	getUseValidation() const = 0;

	/**
	 * If set to true, validation will be performed.  Validation is off by
	 * default.
	 *
	 * @param b true to perform validation
	 */
	virtual void
	setUseValidation(bool	b) = 0;

	/**
	 * Return a string suitable for telling the user what parser is being used.
	 *
	 * @return string describing parser
	 */
	virtual const XalanDOMString&
	getParserDescription(XalanDOMString& theResult) const = 0;

	/**
	  * This method returns the installed entity resolver.
	  *
	  * @return The pointer to the installed entity resolver object.
	  */
	virtual EntityResolverType*
	getEntityResolver() const = 0;

	/**
	  * This method installs the user specified entity resolver on the
	  * parser. It allows applications to trap and redirect calls to
	  * external entities.
	  *
	  * @param handler A pointer to the entity resolver to be called
	  * 			   when the parser comes across references to
	  * 			   entities in the XML file.
	  */
	virtual void
	setEntityResolver(EntityResolverType*	resolver) = 0;

	/**
	  * This method returns the installed error handler.
	  *
	  * @return The pointer to the installed error handler object.
	  */
	virtual ErrorHandlerType*
	getErrorHandler() const = 0;

	/**
	  * This method installs the user-specified error handler.
	  *
	  * @param handler A pointer to the error handler to be called upon error.
	  */
	virtual void
	setErrorHandler(ErrorHandlerType*	handler) = 0;

private:

	// Not implemented
	XMLParserLiaison(const XMLParserLiaison&);

	XMLParserLiaison&
	operator=(const XMLParserLiaison&);
};



XALAN_CPP_NAMESPACE_END



#endif	// XMLPARSERLIAISON_HEADER_GUARD_1357924680
