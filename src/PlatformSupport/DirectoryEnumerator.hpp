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
#if !defined(DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680)
#define DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#if defined(_MSC_VER)
#include <io.h>
#else
#	if defined(__GNUC__)
#include <dirent.h>
#	else
#		error Unsupport platform!!!
#	endif
#endif

#include <functional>
#include <iterator>
#include <vector>



#include <dom/DOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#if defined(_MSC_VER)

class FindFileStruct : public _wfinddata_t
{
public:

	enum eAttributes
	{
		eAttributeArchive = _A_ARCH,
		eAttributeDirectory = _A_SUBDIR,
		eAttributeHidden = _A_HIDDEN,
		eAttributeNormal = _A_NORMAL,
		eReadOnly = _A_RDONLY,
		eSystem = _A_SYSTEM
	};

	const XMLCh*
	getName() const
	{
		return name;
	}

	bool
	isArchive() const
	{
		return attrib & eAttributeArchive ? true : false;
	}

	bool
	isDirectory() const
	{
		return attrib & eAttributeDirectory ? true : false;
	}

	bool
	isHidden() const
	{
		return attrib & eAttributeHidden ? true : false;
	}

	bool
	isNormal() const
	{
		return attrib == eAttributeNormal ? true : false;
	}

	bool
	isReadOnly() const
	{
		return attrib & eReadOnly ? true : false;
	}

	bool
	isSystem() const
	{
		return attrib & eSystem ? true : false;
	}
};

#else
#	if defined(__GNUC__)

class FindFileStruct
{
	XMLCh m_name[MAXNAMLEN];
	int m_attrib;
public:

	enum eAttributes
	{
		eAttributeArchive = 1,
		eAttributeDirectory = 2,
		eAttributeHidden = 4,
		eAttributeNormal = 8,
		eReadOnly = 16,
		eSystem = 32
	};

	const XMLCh*
	getName() const
	{
		return m_name;
	}

	bool
	isArchive() const
	{
		return m_attrib & eAttributeArchive ? true : false;
	}

	bool
	isDirectory() const
	{
		return m_attrib & eAttributeDirectory ? true : false;
	}

	bool
	isHidden() const
	{
		return m_attrib & eAttributeHidden ? true : false;
	}

	bool
	isNormal() const
	{
		return m_attrib == eAttributeNormal ? true : false;
	}

	bool
	isReadOnly() const
	{
		return m_attrib & eReadOnly ? true : false;
	}

	bool
	isSystem() const
	{
		return m_attrib & eSystem ? true : false;
	}
};


#	else
#		error Unsupported platform!
#	endif
#endif


struct ArchiveFileFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		return theFindData.isArchive();
	}
};



struct DirectoryFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		return theFindData.isDirectory();
	}
};



struct HiddenFileFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		return theFindData.isHidden();
	}
};



struct NormalFileFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		return theFindData.isNormal();
	}
};



struct ReadOnlyFileFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		return theFindData.isReadOnly();
	}
};



struct SystemFileFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		return theFindData.isSystem();
	}
};



struct FilesOnlyFilterPredicate : public std::unary_function<FindFileStruct, bool>
{
	result_type
	operator()(const argument_type&		theFindData) const
	{
		DirectoryFilterPredicate		directoryfilterpredicate;
		ArchiveFileFilterPredicate    archivefilefilterpredicate;
		NormalFileFilterPredicate     normalfilefilterpredicate;
		ReadOnlyFileFilterPredicate   readonlyfilefilterpredicate;
		return !directoryfilterpredicate(theFindData) &&
			   (archivefilefilterpredicate(theFindData) ||
			    normalfilefilterpredicate(theFindData) ||
				readonlyfilefilterpredicate(theFindData));
			   
	}

//	DirectoryFilterPredicate		m_directoryPredicate;
//	ArchiveFileFilterPredicate		m_archivePredicate;
//	NormalFileFilterPredicate		m_normalPredicate;
//	ReadOnlyFileFilterPredicate		m_readOnlyPredicate;
};



template<class OutputIteratorType,
		 class FilterPredicateType,
		 class StringType,
		 class StringConversionFunction>
void
EnumerateDirectory(
			const StringType&			theDirectory,
			OutputIteratorType			theOutputIterator,
			FilterPredicateType			theFilterPredicate,
			StringConversionFunction	theConversionFunction)
{
#if defined(_MSC_VER)
	StringType	theSearchSpec(clone(theDirectory));

	theSearchSpec += "\\*";

	FindFileStruct 		theFindData;

	long	theSearchHandle = _wfindfirst(const_cast<wchar_t*>(theConversionFunction(theSearchSpec)),
										  &theFindData);

	if (theSearchHandle != -1)
	{
		try
		{
			do
			{
				if (theFilterPredicate(theFindData) == true)
				{
					*theOutputIterator = theFindData.getName();
				}
			}
			while(_wfindnext(theSearchHandle,
							 &theFindData) == 0);
		}
		catch(...)
		{
			_findclose(theSearchHandle);

			throw;
		}

		_findclose(theSearchHandle);
	}
#else

#	if defined(__GNUC__)
	// @@ Need to implement this !!
	assert(0);
#	else
#		error Unsupport platform!!!
#	endif

#endif
}



template<class CollectionType,
		 class FilterPredicateType = FilesOnlyFilterPredicate,
		 class StringType = DOMString,
		 class StringConversionFunction = c_wstr_functor>
struct DirectoryEnumeratorFunctor : public std::unary_function<StringType, CollectionType>
{
	result_type
	operator()(const argument_type&		theDirectory) const
	{
		result_type		theCollection;

		operator()(theDirectory,
				   theCollection);

		return theCollection;
	}

	void
	operator()(const argument_type&		theDirectory,
			   CollectionType&			theCollection) const
	{
		EnumerateDirectory(theDirectory,
						   std::back_inserter(theCollection),
						   m_filterPredicate,
						   m_conversionFunction);
	}

private:

	FilterPredicateType			m_filterPredicate;
	StringConversionFunction	m_conversionFunction;
};



#endif	// DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680
