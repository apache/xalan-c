/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#if !defined(XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680)
#define XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <algorithm>
#if !defined(XPATH_FUNCTION_TABLE_NEW)
#include <map>
#endif



#include <XalanDOM/XalanDOMString.hpp>



#include <Include/STLHelper.hpp>



#include <XPath/Function.hpp>
#include <XPath/XalanXPathException.hpp>



class Locator;



/**
 * Exception class thrown when an unknown function is encountered
 */
class XALAN_XPATH_EXPORT XPathExceptionFunctionNotAvailable : public XalanXPathException
{
public:

	XPathExceptionFunctionNotAvailable(int	theFunctionNumber);

	XPathExceptionFunctionNotAvailable(const XalanDOMString&	theFunctionName);

	XPathExceptionFunctionNotAvailable(
		int					theFunctionNumber,
		const Locator&		theLocator);

	XPathExceptionFunctionNotAvailable(
		const XalanDOMString&	theFunctionName,
		const Locator&			theLocator);

	~XPathExceptionFunctionNotAvailable();
};



/**
 * Class defines a table of functions that can be called in XPath expresions.
 */
class XALAN_XPATH_EXPORT XPathFunctionTable
{
public:

#if !defined(XPATH_FUNCTION_TABLE_NEW)
#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const Function*>			CollectionType;
	typedef map<XalanDOMString,
				int,
				less<XalanDOMString> >		FunctionNameIndexMapType;
#else
	typedef std::vector<const Function*>	CollectionType;
	typedef std::map<XalanDOMString, int>	FunctionNameIndexMapType;
#endif
#endif

	enum { InvalidFunctionNumberID = -1, eTableSize = 36 };

	typedef size_t						SizeType;
	typedef XalanDOMString::size_type	StringSizeType;
	typedef DeleteFunctor<Function>		DeleteFunctorType;

	/**
	 * Constructor.
	 *
	 * @param fCreateTable If true, the internal table will be created.  Otherwise, CreateTable() must be called.
	 */
	XPathFunctionTable(bool		fCreateTable = true);

	~XPathFunctionTable();

	/**
	 * Set up the internal table.
	 */
	void
	CreateTable();

	/**
	 * Destroy the internal table.
	 */
	void
	DestroyTable();

	/**
	 * Retrieve the function object for a specified function name.
	 * 
	 * @param theFunctionName name of function
	 * @return function named
	 */
	const Function&
	operator[](const XalanDOMString&	theFunctionName) const
	{
#if defined(XPATH_FUNCTION_TABLE_NEW)
		const int	theFunctionID =
			getFunctionIndex(theFunctionName);

		if (theFunctionID != InvalidFunctionNumberID)
		{
			return *m_functionTable[theFunctionID];
		}
#else
		FunctionNameIndexMapType::const_iterator	i =
			m_FunctionNameIndex.find(theFunctionName);

		if (i != m_FunctionNameIndex.end())
		{
			return *m_FunctionCollection[(*i).second];
		}
#endif
		else
		{
			throw XPathExceptionFunctionNotAvailable(theFunctionName);
		}
	}

	/**
	 * Retrieve the function object for a specified function ID number.
	 * 
	 * @param theFunctionID ID number of the function
	 * @return function named
	 */
	const Function&
	operator[](int	theFunctionID) const
	{
#if defined(XPATH_FUNCTION_TABLE_NEW)
		assert(theFunctionID >= 0 && theFunction < eTableSize);

		return *m_functionTable[theFunctionID];
#else
		assert(theFunctionID >= 0 &&
			   CollectionType::size_type(theFunctionID) < m_FunctionCollection.size());

		return *m_FunctionCollection[theFunctionID];
#endif
	}

	/**
	 * Map a function ID to the corresponding name.
	 * 
	 * @param theFunctionID The ID number of the function
	 * @return The name of the function, or an empty string if the function doesn't exist.
	 */
	const XalanDOMString
	idToName(int	theFunctionID) const
	{
		XalanDOMString	theName;

#if defined(XPATH_FUNCTION_TABLE_NEW)
		if (theFunctionID >= 0 && theFunction < eTableSize)
		{
			theName = s_functionNames[theFunctionID];
		}
#else
		if (theFunctionID >= 0 &&
			CollectionType::size_type(theFunctionID) < m_FunctionCollection.size())
		{
			FunctionNameIndexMapType::const_iterator	i =
				m_FunctionNameIndex.begin();

			while (i != m_FunctionNameIndex.end())
			{
				if ((*i).second == theFunctionID)
				{
					theName = (*i).first;

					break;
				}
			}
		}
#endif

		return theName;
	}

	/**
	 * Map a function name to the corresponding ID number.
	 * 
	 * @param theName name of function
	 * @return The ID number of function, or InvalidFunctionNumberID if the function doesn't exist.
	 */
	int
	nameToID(const XalanDOMString&	theName) const
	{
#if defined(XPATH_FUNCTION_TABLE_NEW)
		return getFunctionIndex(theName);
#else
		const FunctionNameIndexMapType::const_iterator	i =
			m_FunctionNameIndex.find(theName);

		if (i != m_FunctionNameIndex.end())
		{
			return (*i).second;
		}
		else
		{
			return InvalidFunctionNumberID;
		}
#endif
	}

	/**
	 * Insert a named function into the function table.
	 * 
	 * @param theFunctionName name of function
	 * @param theFunction     function object corresponding to name
	 */
	void
	InstallFunction(
			const XalanDOMString&	theFunctionName,
			const Function&			theFunction);

	/**
	 * Remove a named function from the function table.
	 * 
	 * @param theFunctionName name of function
	 * @return true if the function was found and removed.
	 */
	bool
	UninstallFunction(const XalanDOMString&		theFunctionName);

	/**
	 * Whether a named function is in the function table.
	 * 
	 * @param theFunctionName name of function
	 * @return true if function is in table
	 */
	bool
	isInstalledFunction(const XalanDOMString&	theFunctionName) const
	{
#if defined(XPATH_FUNCTION_TABLE_NEW)
		if (getFunctionIndex(theFunctionName) != InvalidFunctionNumberID)
#else
		if (m_FunctionNameIndex.find(theFunctionName) != m_FunctionNameIndex.end())
#endif
		{
			return true;
		}
		else
		{
			return false;
		}
	}

#if defined(XALAN_NO_MEMBER_TEMPLATES)

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>			InstalledFunctionNameVectorType;
#else
	typedef std::vector<XalanDOMString>		InstalledFunctionNameVectorType;
#endif

	/**
	 * Add a list of the names of installed functions to a vector of names.
	 * 
	 * @param theVector vector of function name strings added to
	 */
	void
	getInstalledFunctionNames(InstalledFunctionNameVectorType&	theVector) const
	{
#if defined(XPATH_FUNCTION_TABLE_NEW)
		XalanDOMString	theString;

		for (int i = 0; i < TableSize; ++i)
		{
			if (m_functionTable[i] != 0)
			{
				theString.assign(
					s_functionNames[i].m_name,
					s_functionNames[i].size);

				theVector.push_back(theString);
			}
		}
#else
		FunctionNameIndexMapType::const_iterator	i =
			m_FunctionNameIndex.begin();

		while(i != m_FunctionNameIndex.end())
		{
			theVector.push_back((*i).first);

			++i;
		}
	}
#endif
#else

	/**
	 * Add a list of the names of installed functions to a vector of names.
	 * 
	 * @param theIterator function table iterator to append names to
	 */
	template<class OutputIteratorType>
	void
	getInstalledFunctionNames(OutputIteratorType	theIterator) const
	{
#if defined(XPATH_FUNCTION_TABLE_NEW)
		XalanDOMString	theString;

		for (int i = 0; i < TableSize; ++i)
		{
			if (m_functionTable[i] != 0)
			{
				theString.assign(
					s_functionNames[i].m_name,
					s_functionNames[i].size);

				*theIterator = theString;

				++theIterator;
			}
		}
#else
		FunctionNameIndexMapType::const_iterator	i =
			m_FunctionNameIndex.begin();

		while(i != m_FunctionNameIndex.end())
		{
			*theIterator = (*i).first;

			++i;
			++theIterator;
		}
#endif
	}
#endif

	struct FunctionNameTableEntry
	{
		const XalanDOMChar*		m_name;

		StringSizeType			m_size;
	};

private:

	static int
	getFunctionIndex(const XalanDOMString&	theName);

#if !defined(XPATH_FUNCTION_TABLE_NEW)
	CollectionType				m_FunctionCollection;

	FunctionNameIndexMapType	m_FunctionNameIndex;
#endif

	const Function*				m_functionTable[eTableSize];

	const Function** const		m_functionTableEnd;

	// These are static strings for the functions supported.
	// Note that the XSLT functions are also here, since it's
	// just easier to do it this way.

	// The string "id"
	static const XalanDOMChar	s_id[];

	// The string "key"
	static const XalanDOMChar	s_key[];

	// The string "not"
	static const XalanDOMChar	s_not[];

	// The string "sum"
	static const XalanDOMChar	s_sum[];

	// The string "lang"
	static const XalanDOMChar	s_lang[];

	// The string "last"
	static const XalanDOMChar	s_last[];

	// The string "name"
	static const XalanDOMChar	s_name[];

	// The string "true"
	static const XalanDOMChar	s_true[];

	// The string "count"
	static const XalanDOMChar	s_count[];

	// The string "false"
	static const XalanDOMChar	s_false[];

	// The string "floor"
	static const XalanDOMChar	s_floor[];

	// The string "round"
	static const XalanDOMChar	s_round[];

	// The string "concat"
	static const XalanDOMChar	s_concat[];

	// The string "number"
	static const XalanDOMChar	s_number[];

	// The string "string"
	static const XalanDOMChar	s_string[];

	// The string "boolean"
	static const XalanDOMChar	s_boolean[];

	// The string "ceiling"
	static const XalanDOMChar	s_ceiling[];

	// The string "current"
	static const XalanDOMChar	s_current[];

	// The string "contains"
	static const XalanDOMChar	s_contains[];

	// The string "document"
	static const XalanDOMChar	s_document[];

	// The string "position"
	static const XalanDOMChar	s_position[];

	// The string "substring"
	static const XalanDOMChar	s_substring[];

	// The string "translate"
	static const XalanDOMChar	s_translate[];

	// The string "local-name"
	static const XalanDOMChar	s_localName[];

	// The string "generate-id"
	static const XalanDOMChar	s_generateId[];

	// The string "starts-with"
	static const XalanDOMChar	s_startsWith[];

	// The string "format-number"
	static const XalanDOMChar	s_formatNumber[];

	// The string "namespace-uri"
	static const XalanDOMChar	s_namespaceUri[];

	// The string "string-length"
	static const XalanDOMChar	s_stringLength[];

	// The string "normalize-space"
	static const XalanDOMChar	s_normalizeSpace[];

	// The string "substring-after"
	static const XalanDOMChar	s_substringAfter[];

	// The string "system-property"
	static const XalanDOMChar	s_systemProperty[];

	// The string "substring-before"
	static const XalanDOMChar	s_substringBefore[];

	// The string "element-available"
	static const XalanDOMChar	s_elementAvailable[];

	// The string "function-available"
	static const XalanDOMChar	s_functionAvailable[];

	// The string "unparsed-entity-uri"
	static const XalanDOMChar	s_unparsedEntityUri[];

	// A table of function names.
	static const FunctionNameTableEntry			s_functionNames[];

	// The last one in the table of function names.
	static const FunctionNameTableEntry* const	s_lastFunctionName;

	// The size of the table.
	static const SizeType						s_functionNamesSize;
};



#endif	// XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680
