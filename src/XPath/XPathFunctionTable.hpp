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
#if !defined(XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680)
#define XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <algorithm>
#include <map>



#include <dom/DOMString.hpp>



#include <PlatformSupport/STLHelper.hpp>



#include <XPath/Function.hpp>
#include <XPath/XPathException.hpp>



/**
 * Exception class thrown when a function table problem is encountered
 */
class XALAN_XPATH_EXPORT XPathFunctionTableException : public XPathException
{
public:

	virtual
	~XPathFunctionTableException();

protected:

	/**
	 * Construct an XPathFunctionTableException object
	 * 
	 * @param theMessage string error message
	 */
	XPathFunctionTableException(const DOMString&	theMessage);
};



/**
 * Exception class thrown when an invalid function is encountered
 */
class XALAN_XPATH_EXPORT XPathFunctionTableInvalidFunctionException : public XPathFunctionTableException
{
public:

	/**
	 * Construct an XPathFunctionTableException object
	 * 
	 * @param theMessage function name string
	 */
	XPathFunctionTableInvalidFunctionException(const DOMString&		theFunctionName); 

	virtual
	~XPathFunctionTableInvalidFunctionException();

private:

	static DOMString
	FormatMessage(const DOMString&	theFunctionName);
};


/**
 * Class defines a table of functions that can be called in XPath expresions.
 */
class XALAN_XPATH_EXPORT XPathFunctionTable
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef map<DOMString, Function*>			CollectionType;
#else
	typedef std::map<DOMString, Function*>			CollectionType;
#endif
	typedef MapValueDeleteFunctor<CollectionType>	DeleteFunctorType;

	XPathFunctionTable();

	~XPathFunctionTable();

	/**
	 * Retrieve the function object for a specified function name.
	 * 
	 * @param theFunctionName name of function
	 * @return function named
	 */
	Function&
	operator[](const DOMString&		theFunctionName) const
	{
		CollectionType::const_iterator	i =
			m_FunctionCollection.find(theFunctionName);

		if (i != m_FunctionCollection.end())
		{
			return *(*i).second;
		}
		else
		{
			throw XPathFunctionTableInvalidFunctionException(theFunctionName);
		}
	}

	/**
	 * Insert a named function into the function table.
	 * 
	 * @param theFunctionName name of function
	 * @param theFunction     function object corresponding to name
	 */
	void
	InstallFunction(
			const DOMString&	theFunctionName,
			const Function&		theFunction);


	/**
	 * Whether a named function is in the function table.
	 * 
	 * @param theFunctionName name of function
	 * @return true if function is in table
	 */
	bool
	isInstalledFunction(const DOMString&	theFunctionName) const
	{
		if (m_FunctionCollection.find(theFunctionName) != m_FunctionCollection.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

#if defined(XALAN_NO_MEMBER_TEMPLATES)
	typedef vector<DOMString>	InstalledFunctionNameVectorType;

	/**
	 * Add a list of the names of installed functions to a vector of names.
	 * 
	 * @param theVector vector of function name strings added to
	 */
	void
	getInstalledFunctionNames(InstalledFunctionNameVectorType&	theVector) const
	{
		CollectionType::const_iterator	i =
			m_FunctionCollection.begin();

		while(i != m_FunctionCollection.end())
		{
			theVector.push_back((*i).first);

			++i;
		}
	}
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
		copy(m_FunctionCollection.begin(),
			 m_FunctionCollection.end(),
			 PairIsolatorOutputIterator<OutputIteratorType,
										PairFirstIsolator<CollectionType::value_type> >(theIterator,
																					    PairFirstIsolator<CollectionType::value_type>()));
	}
#endif

protected:

	void
	CreateTable();

	void
	DestroyTable();

private:

	CollectionType						m_FunctionCollection;
};



#endif	// XPATHFUNCTIONTABLE_HEADER_GUARD_1357924680
