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
// Base class header file
#include "XPathFunctionTable.hpp"



#include <cstring>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "FunctionBoolean.hpp"
#include "FunctionCeiling.hpp"
#include "FunctionConcat.hpp"
#include "FunctionContains.hpp"
#include "FunctionCount.hpp"
#include "FunctionFalse.hpp"
#include "FunctionFloor.hpp"
#include "FunctionID.hpp"
#include "FunctionLang.hpp"
#include "FunctionLast.hpp"
#include "FunctionLocalName.hpp"
#include "FunctionName.hpp"
#include "FunctionNamespaceURI.hpp"
#include "FunctionNormalizeSpace.hpp"
#include "FunctionNot.hpp"
#include "FunctionNumber.hpp"
#include "FunctionPosition.hpp"
#include "FunctionRound.hpp"
#include "FunctionStartsWith.hpp"
#include "FunctionString.hpp"
#include "FunctionStringLength.hpp"
#include "FunctionSubstring.hpp"
#include "FunctionSubstringAfter.hpp"
#include "FunctionSubstringBefore.hpp"
#include "FunctionSum.hpp"
#include "FunctionTranslate.hpp"
#include "FunctionTrue.hpp"



XPathFunctionTable::XPathFunctionTable(bool		fCreateTable) :
	m_FunctionCollection(),
	m_FunctionNameIndex(),
	m_functionTable(),
	m_functionTableEnd(m_functionTable + (sizeof(m_functionTable) / sizeof(m_functionTable[0])) - 1)
{
	assert(int(s_functionNamesSize) == eTableSize);

#if defined(XALAN_STRICT_ANSI_HEADERS)
	std::memset(m_functionTable, 0, sizeof(m_functionTable));
#else
	memset(m_functionTable, 0, sizeof(m_functionTable));
#endif

	if (fCreateTable == true)
	{
		CreateTable();
	}
}



XPathFunctionTable::~XPathFunctionTable()
{
	DestroyTable();
}



void
XPathFunctionTable::InstallFunction(
			const XalanDOMString&	theFunctionName,
			const Function&			theFunction)
{
	assert(length(theFunctionName) != 0);

	// See if a function of that name is already installed...
	const FunctionNameIndexMapType::iterator	i =
		m_FunctionNameIndex.find(theFunctionName);

	if (i != m_FunctionNameIndex.end())
	{
		assert(CollectionType::size_type((*i).second) < m_FunctionCollection.size());

		// It is, so delete the old one, and add the new one...
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (Function*)m_FunctionCollection[(*i).second];
#else
		delete m_FunctionCollection[(*i).second];
#endif

		m_FunctionCollection[(*i).second] = theFunction.clone();
	}
	else
	{
		const CollectionType::size_type		theIndex = m_FunctionCollection.size();

		m_FunctionCollection.push_back(theFunction.clone());

		m_FunctionNameIndex[theFunctionName] = int(theIndex);
	}
}



bool
XPathFunctionTable::UninstallFunction(const XalanDOMString&		theFunctionName)
{
	assert(length(theFunctionName) != 0);

	// See if a function of that name is installed...
	const FunctionNameIndexMapType::iterator	i =
		m_FunctionNameIndex.find(theFunctionName);

	if (i == m_FunctionNameIndex.end())
	{
		return false;
	}
	else
	{
		assert(CollectionType::size_type((*i).second) < m_FunctionCollection.size());

#if !defined(XALAN_NO_NAMESPACES)
		using std::find;
#endif

		// Delete the function...
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (Function*)m_FunctionCollection[(*i).second];
#else
		delete m_FunctionCollection[(*i).second];
#endif

		// Set the entry in the table to 0...
		m_FunctionCollection[(*i).second] = 0;

		return true;
	}
}


#if 0
#include <fstream>

void
dumpTable(
			const XPathFunctionTable::FunctionNameIndexMapType&		theTable,
			std::ostream&											theSourceStream,
			std::ostream&											theHeaderStream)
{
	XPathFunctionTable::FunctionNameIndexMapType::const_iterator	i = theTable.begin();

	while(i != theTable.end())
	{
		theSourceStream << "const XalanDOMChar\tXPathFunctionTable::s_";

		const XalanDOMString&	theString = (*i).first;

		theHeaderStream << "\t// The string \"" << theString << "\"\n\tstatic const XalanDOMChar\ts_";

		bool	nextCap = false;

		XalanDOMString::const_iterator	j = theString.begin();

		while(*j)
		{
			if (*j == '-')
			{
				nextCap = true;
			}
			else
			{
				assert(*j >= 'a' && *j <= 'z');

				if (nextCap)
				{
					theSourceStream << char(*j -'a' + 'A');
					theHeaderStream << char(*j -'a' + 'A');

					nextCap = false;
				}
				else
				{
					theSourceStream << char(*j);
					theHeaderStream << char(*j);
				}
			}

			++j;
		}

		j = theString.begin();

		theSourceStream << "[] =\n{\n";
		theHeaderStream << "[];\n\n";

		while(*j)
		{
			if (*j == '-')
			{
				theSourceStream << "\tXalanUnicode::charHyphenMinus,\n";
			}
			else
			{
				assert(*j >= 'a' && *j <= 'z');

				theSourceStream << "\tXalanUnicode::charLetter_";

				theSourceStream << char(*j) << ",\n";
			}

			++j;
		}

		theSourceStream << "\t0\n};\n\n";

		++i;
	}
}
#endif



void
XPathFunctionTable::CreateTable()
{
	try
	{
		m_FunctionCollection.reserve(eTableSize);

		// Start with the longest function name, so we only have
		// one allocation for this string.
		XalanDOMString	theFunctionName;

		theFunctionName = s_substringBefore;

		InstallFunction(
				theFunctionName,
				FunctionSubstringBefore());

		theFunctionName = s_last;

		InstallFunction(
				theFunctionName,
				FunctionLast());

		theFunctionName = s_position;

		InstallFunction(
				theFunctionName,
				FunctionPosition());

		theFunctionName = s_count;

		InstallFunction(
				theFunctionName,
				FunctionCount());

		theFunctionName = s_id;

		InstallFunction(
				theFunctionName,
				FunctionID());

		theFunctionName = s_localName;

		InstallFunction(
				theFunctionName,
				FunctionLocalName());

		theFunctionName = s_namespaceUri;

		InstallFunction(
				theFunctionName,
				FunctionNamespaceURI());

		theFunctionName = s_name;

		InstallFunction(
				theFunctionName,
				FunctionName());

		theFunctionName = s_string;

		InstallFunction(
				theFunctionName,
				FunctionString());

		theFunctionName = s_concat;

		InstallFunction(
				theFunctionName,
				FunctionConcat());

		theFunctionName = s_startsWith;

		InstallFunction(
				theFunctionName,
				FunctionStartsWith());

		theFunctionName = s_contains;

		InstallFunction(
				theFunctionName,
				FunctionContains());

		theFunctionName = s_substringAfter;

		InstallFunction(
				theFunctionName,
				FunctionSubstringAfter());

		theFunctionName = s_substring;

		InstallFunction(
				theFunctionName,
				FunctionSubstring());

		theFunctionName = s_stringLength;

		InstallFunction(
				theFunctionName,
				FunctionStringLength());

		theFunctionName = s_normalizeSpace;

		InstallFunction(
				theFunctionName,
				FunctionNormalizeSpace());

		theFunctionName = s_translate;

		InstallFunction(
				theFunctionName,
				FunctionTranslate());

		theFunctionName = s_boolean;

		InstallFunction(
				theFunctionName,
				FunctionBoolean());

		theFunctionName = s_not;

		InstallFunction(
				theFunctionName,
				FunctionNot());

		theFunctionName = s_true;

		InstallFunction(
				theFunctionName,
				FunctionTrue());

		theFunctionName = s_false;

		InstallFunction(
				theFunctionName,
				FunctionFalse());

		theFunctionName = s_lang;

		InstallFunction(
				theFunctionName,
				FunctionLang());

		theFunctionName = s_number;

		InstallFunction(
				theFunctionName,
				FunctionNumber());

		theFunctionName = s_sum;

		InstallFunction(
				theFunctionName,
				FunctionSum());

		theFunctionName = s_floor;

		InstallFunction(
				theFunctionName,
				FunctionFloor());

		theFunctionName = s_ceiling;

		InstallFunction(
				theFunctionName,
				FunctionCeiling());

		theFunctionName = s_round;

		InstallFunction(
				theFunctionName,
				FunctionRound());

#if 0
		std::ofstream	theSourceStream("\\foo.cpp");
		std::ofstream	theHeaderStream("\\foo.hpp");

		dumpTable(m_FunctionNameIndex, theSourceStream, theHeaderStream);
#endif
	}
	catch(...)
	{
		DestroyTable();

		throw;
	}
}



void
XPathFunctionTable::DestroyTable()
{
	try
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		for_each(m_FunctionCollection.begin(),
				 m_FunctionCollection.end(),
				 DeleteFunctorType());

		CollectionType().swap(m_FunctionCollection);

		FunctionNameIndexMapType().swap(m_FunctionNameIndex);
	}
	catch(...)
	{
	}
}



int
XPathFunctionTable::getFunctionIndex(const XalanDOMString&	theName)
{
	// Do a binary search...
	const FunctionNameTableEntry*	theFirst = s_functionNames;
	const FunctionNameTableEntry*	theLast = s_lastFunctionName;

	while(theFirst <= theLast)
	{
		const FunctionNameTableEntry* const		theCurrent =
			theFirst + (theLast - theFirst) / 2;
		assert(theCurrent->m_size == length(theCurrent->m_name));

		const int	theResult = compare(
				theName.c_str(),
				theName.length(),
				theCurrent->m_name,
				theCurrent->m_size);

		if (theResult < 0)
		{
			theLast = theCurrent - 1;
		}
		else if (theResult > 0)
		{
			theFirst = theCurrent + 1;
		}
		else
		{
			return theCurrent - s_functionNames;
		}
	}

	return InvalidFunctionNumberID;
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(int	theFunctionNumber) :
	XalanXPathException(TranscodeFromLocalCodePage("The specified function number is not available: ") + LongToDOMString(theFunctionNumber))
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(const XalanDOMString&	theFunctionName) :
	XalanXPathException(TranscodeFromLocalCodePage("The specified function is not available: ") + theFunctionName)
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(
			int				theFunctionNumber,
			const Locator&	theLocator) :
	XalanXPathException(
					theLocator,
					TranscodeFromLocalCodePage("The specified function number is not available: ") + LongToDOMString(theFunctionNumber))
{
}



XPathExceptionFunctionNotAvailable::XPathExceptionFunctionNotAvailable(
			const XalanDOMString&	theFunctionName,
			const Locator&			theLocator) :
	XalanXPathException(
					theLocator,
					TranscodeFromLocalCodePage("The specified function is not available: ") + theFunctionName)
{
}



XPathExceptionFunctionNotAvailable::~XPathExceptionFunctionNotAvailable()
{
}



const XalanDOMChar	XPathFunctionTable::s_id[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_d,
	0
};

const XalanDOMChar	XPathFunctionTable::s_key[] =
{
	XalanUnicode::charLetter_k,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	0
};

const XalanDOMChar	XPathFunctionTable::s_not[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathFunctionTable::s_sum[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	0
};

const XalanDOMChar	XPathFunctionTable::s_lang[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar	XPathFunctionTable::s_last[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathFunctionTable::s_name[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_true[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_count[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathFunctionTable::s_false[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_floor[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar	XPathFunctionTable::s_round[] =
{
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_d,
	0
};

const XalanDOMChar	XPathFunctionTable::s_concat[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathFunctionTable::s_number[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar	XPathFunctionTable::s_string[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar	XPathFunctionTable::s_boolean[] =
{
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	0
};

const XalanDOMChar	XPathFunctionTable::s_ceiling[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar	XPathFunctionTable::s_current[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathFunctionTable::s_contains[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_s,
	0
};

const XalanDOMChar	XPathFunctionTable::s_document[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathFunctionTable::s_position[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};

const XalanDOMChar	XPathFunctionTable::s_substring[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar	XPathFunctionTable::s_translate[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_localName[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_generateId[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_d,
	0
};

const XalanDOMChar	XPathFunctionTable::s_startsWith[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_s,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_h,
	0
};

const XalanDOMChar	XPathFunctionTable::s_formatNumber[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar	XPathFunctionTable::s_namespaceUri[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	0
};

const XalanDOMChar	XPathFunctionTable::s_stringLength[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_h,
	0
};

const XalanDOMChar	XPathFunctionTable::s_normalizeSpace[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_z,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_substringAfter[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar	XPathFunctionTable::s_systemProperty[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_y,
	0
};

const XalanDOMChar	XPathFunctionTable::s_substringBefore[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_elementAvailable[] =
{
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_functionAvailable[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathFunctionTable::s_unparsedEntityUri[] =
{
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_d,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_y,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	0
};


typedef XPathFunctionTable::SizeType				SizeType;
typedef XPathFunctionTable::FunctionNameTableEntry	FunctionNameTableEntry;

#define XFTBL_SIZE(str)	((sizeof(str) / sizeof(str[0]) - 1))


const FunctionNameTableEntry	XPathFunctionTable::s_functionNames[] =
{
	{
		s_id,
		XFTBL_SIZE(s_id)
	},
	{
		s_key,
		XFTBL_SIZE(s_key)
	},
	{
		s_not,
		XFTBL_SIZE(s_not)
	},
	{
		s_sum,
		XFTBL_SIZE(s_sum)
	},
	{
		s_lang,
		XFTBL_SIZE(s_lang)
	},
	{
		s_last,
		XFTBL_SIZE(s_last)
	},
	{
		s_name,
		XFTBL_SIZE(s_name)
	},
	{
		s_true,
		XFTBL_SIZE(s_true)
	},
	{
		s_count,
		XFTBL_SIZE(s_count)
	},
	{
		s_false,
		XFTBL_SIZE(s_false)
	},
	{
		s_floor,
		XFTBL_SIZE(s_floor)
	},
	{
		s_round,
		XFTBL_SIZE(s_round)
	},
	{
		s_concat,
		XFTBL_SIZE(s_concat)
	},
	{
		s_number,
		XFTBL_SIZE(s_number)
	},
	{
		s_string,
		XFTBL_SIZE(s_string)
	},
	{
		s_boolean,
		XFTBL_SIZE(s_boolean)
	},
	{
		s_ceiling,
		XFTBL_SIZE(s_ceiling)
	},
	{
		s_current,
		XFTBL_SIZE(s_current)
	},
	{
		s_contains,
		XFTBL_SIZE(s_contains)
	},
	{
		s_document,
		XFTBL_SIZE(s_document)
	},
	{
		s_position,
		XFTBL_SIZE(s_position)
	},
	{
		s_substring,
		XFTBL_SIZE(s_substring)
	},
	{
		s_translate,
		XFTBL_SIZE(s_translate)
	},
	{
		s_localName,
		XFTBL_SIZE(s_localName)
	},
	{
		s_generateId,
		XFTBL_SIZE(s_generateId)
	},
	{
		s_startsWith,
		XFTBL_SIZE(s_startsWith)
	},
	{
		s_formatNumber,
		XFTBL_SIZE(s_formatNumber)
	},
	{
		s_namespaceUri,
		XFTBL_SIZE(s_namespaceUri)
	},
	{
		s_stringLength,
		XFTBL_SIZE(s_stringLength)
	},
	{
		s_normalizeSpace,
		XFTBL_SIZE(s_normalizeSpace)
	},
	{
		s_substringAfter,
		XFTBL_SIZE(s_substringAfter)
	},
	{
		s_systemProperty,
		XFTBL_SIZE(s_systemProperty)
	},
	{
		s_substringBefore,
		XFTBL_SIZE(s_substringBefore)
	},
	{
		s_elementAvailable,
		XFTBL_SIZE(s_elementAvailable)
	},
	{
		s_functionAvailable,
		XFTBL_SIZE(s_functionAvailable)
	},
	{
		s_unparsedEntityUri,
		XFTBL_SIZE(s_unparsedEntityUri)
	}
};


const FunctionNameTableEntry* const		XPathFunctionTable::s_lastFunctionName =
	&s_functionNames[sizeof(s_functionNames) / sizeof(s_functionNames[0]) - 1];


const SizeType		XPathFunctionTable::s_functionNamesSize =
	sizeof(s_functionNames) / sizeof(s_functionNames[0]);