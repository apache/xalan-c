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
	m_FunctionNameIndex()
{
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
		m_FunctionCollection.reserve(eDefaultTableSize);

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("last")),
						FunctionLast());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("position")),
						FunctionPosition());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("count")),
						FunctionCount());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("id")),
						FunctionID());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("local-name")),
						FunctionLocalName());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("namespace-uri")),
						FunctionNamespaceURI());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("name")),
						FunctionName());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("string")),
						FunctionString());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("concat")),
						FunctionConcat());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("starts-with")),
						FunctionStartsWith());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("contains")),
						FunctionContains());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("substring-before")),
						FunctionSubstringBefore());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("substring-after")),
						FunctionSubstringAfter());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("substring")),
						FunctionSubstring());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("string-length")),
						FunctionStringLength());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("normalize-space")),
						FunctionNormalizeSpace());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("translate")),
						FunctionTranslate());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("boolean")),
						FunctionBoolean());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("not")),
						FunctionNot());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("true")),
						FunctionTrue());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("false")),
						FunctionFalse());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("lang")),
						FunctionLang());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("number")),
						FunctionNumber());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("sum")),
						FunctionSum());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("floor")),
						FunctionFloor());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("ceiling")),
						FunctionCeiling());

		InstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("round")),
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
