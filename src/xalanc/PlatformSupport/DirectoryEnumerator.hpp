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
#if !defined(DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680)
#define DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#if defined(_MSC_VER)
#include <io.h>
#else
#include <dirent.h>
#endif



#include <functional>
#include <iterator>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



#if defined(_MSC_VER)

struct FindFileStruct : public _wfinddata_t
{

	enum eAttributes
	{
		eAttributeArchive = _A_ARCH,
		eAttributeDirectory = _A_SUBDIR,
		eAttributeHidden = _A_HIDDEN,
		eAttributeNormal = _A_NORMAL,
		eReadOnly = _A_RDONLY,
		eSystem = _A_SYSTEM
	};

public:

	/**
	 * Retrieve name of file
	 *
	 * @return file name
	 */
	const XalanDOMChar*
	getName() const
	{
		return name;
	}

	/**
	 * Determine whether file is a directory
	 *
	 * @return true if file is a directory
	 */
	bool
	isDirectory() const
	{
		return attrib & eAttributeDirectory ? true : false;
	}

	bool
	isSelfOrParent() const
	{
		if (isDirectory() == false)
		{
			return false;
		}
		else if (name[0] == '.')
		{
			if (name[1] == '\0')
			{
				return true;
			}
			else if (name[1] == '.' &&
					 name[2] == '\0')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};

#else

struct FindFileStruct : public dirent
{
public:

	/**
	 * Retrieve name of file
	 *
	 * @return file name
	 */
	const char* getName() const
	{
		return d_name;
	}

	/**
	 * Determine whether file is a directory
	 *
	 * @return true if file is a directory
	 */
	bool isDirectory() const
	{
#if defined(AIX) || defined(HPUX) || defined(SOLARIS) || defined(OS390) || defined(OS400) || defined(TRU64)  || defined(CYGWIN)
		return false;
#else		
		if (d_type == DT_DIR || d_type == DT_UNKNOWN)
		{
			return true;
		}
		else
		{
			return false;
		}
#endif		
	}

	bool
	isSelfOrParent() const
	{
#if defined(AIX) || defined(HPUX) || defined(SOLARIS) || defined(OS390) || defined(OS400) || defined(TRU64)
		return false;
#else		
		if (isDirectory() == false)
		{
			return false;
		}
		else if (d_name[0] == '.')
		{
			if (d_name[1] == '\0')
			{
				return true;
			}
			else if (d_name[1] == '.' &&
					 d_name[2] == '\0')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
#endif
	}
};

#endif



#if defined(XALAN_NO_STD_NAMESPACE)
struct DirectoryFilterPredicate : public unary_function<FindFileStruct, bool>
#else
struct DirectoryFilterPredicate : public std::unary_function<FindFileStruct, bool>
#endif
{
	result_type
	operator()(const argument_type&	theFindData) const
	{
		return theFindData.isDirectory();
	}
};



#if defined(XALAN_NO_STD_NAMESPACE)
struct FilesOnlyFilterPredicate : public unary_function<FindFileStruct, bool>
#else
struct FilesOnlyFilterPredicate : public std::unary_function<FindFileStruct, bool>
#endif
{
	result_type
	operator()(const argument_type&	theFindData) const
	{
		DirectoryFilterPredicate		theDirectoryPredicate;

		return !theDirectoryPredicate(theFindData);
			   
	}
};



template<class OutputIteratorType,
		 class FilterPredicateType,
		 class StringType,
		 class StringConversionFunction>
void
EnumerateDirectory(
			const StringType&			theFullSearchSpec,
			OutputIteratorType			theOutputIterator,
			FilterPredicateType			theFilterPredicate,
			StringConversionFunction	theConversionFunction,
#if defined(XALAN_TEMPLATE_FUNCTION_NO_DEFAULT_PARAMETERS)
			bool						fIncludeSelfAndParent)
#else
			bool						fIncludeSelfAndParent = false)
#endif
{
#if defined(_MSC_VER)
	FindFileStruct 		theFindData;

	long	theSearchHandle = _wfindfirst(const_cast<wchar_t*>(theConversionFunction(theFullSearchSpec)),
										  &theFindData);

	if (theSearchHandle != -1)
	{
		try
		{
			do
			{
				if ((fIncludeSelfAndParent == true || theFindData.isSelfOrParent() == false) &&
					theFilterPredicate(theFindData) == true)
				{
					*theOutputIterator = StringType(theFindData.getName());
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

	
#elif defined(LINUX)

	CharVectorType	theTargetVector;

	TranscodeToLocalCodePage(theFullSearchSpec, theTargetVector, false);

	const CharVectorType::size_type		theSize = theTargetVector.size();

	if (theSize > 0)
	{
		if (theTargetVector.back() == '*')
		{
			theTargetVector.pop_back();

			if (theSize == 1)
			{
				theTargetVector.push_back('.');
			}
		}

		theTargetVector.push_back('\0');

		const char* const	theSpec = c_str(theTargetVector);
		assert(theSpec != 0);

		DIR* const	theDirectory = opendir(theSpec);

		if (theDirectory != 0)
		{
			try
			{
				const FindFileStruct*	theEntry =
					(FindFileStruct*)readdir(theDirectory);
	
				while(theEntry != 0)
				{
					if ((fIncludeSelfAndParent == true || theEntry->isSelfOrParent() == false) &&
						theFilterPredicate(*theEntry) == true)
					{
						*theOutputIterator = StringType(theEntry->getName());
					}

					theEntry = (FindFileStruct*)readdir(theDirectory);
				}
			}
			catch(...)
			{
				closedir(theDirectory);

				throw;
			}

			closedir(theDirectory);
		}
	}
#else
	// Do nothing for now...
	// Unsupported platform!!!
#endif
}



template<class OutputIteratorType,
		 class FilterPredicateType,
		 class StringType,
		 class StringConversionFunction>
void
EnumerateDirectory(
			const StringType&			theDirectory,
			const StringType&			theSearchSpec,
			OutputIteratorType			theOutputIterator,
			FilterPredicateType			theFilterPredicate,
			StringConversionFunction	theConversionFunction,
#if defined(XALAN_TEMPLATE_FUNCTION_NO_DEFAULT_PARAMETERS)
			bool						fIncludeSelfAndParent)
#else
			bool						fIncludeSelfAndParent = false)
#endif
{
	StringType	theFullSearchSpec(theDirectory);

	theFullSearchSpec += theSearchSpec;

	EnumerateDirectory(theFullSearchSpec, theOutputIterator, theFilterPredicate, theConversionFunction, fIncludeSelfAndParent);
}



#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
template<class CollectionType, class StringType>
struct DirectoryEnumeratorFunctor
{
	CollectionType
	operator()(const StringType&	theDirectory) const
	{
		CollectionType		theCollection;

		operator()(theDirectory,
			   theCollection);

		return theCollection;
	}

	void
	operator()(
		const StringType&,
		const CollectionType&) const
	{
	}
};
#else
template<class CollectionType,
  	 class StringType = XalanDOMString,
  	 class FilterPredicateType = FilesOnlyFilterPredicate,
  	 class StringConversionFunction = c_wstr_functor>
#if defined(XALAN_NO_STD_NAMESPACE)
struct DirectoryEnumeratorFunctor : public unary_function<StringType, CollectionType>
#else
struct DirectoryEnumeratorFunctor : public std::unary_function<StringType, CollectionType>
#endif
{
#if defined(XALAN_NO_STD_NAMESPACE)
	typedef unary_function<StringType, CollectionType>	BaseClassType;
#else
	typedef std::unary_function<StringType, CollectionType>	BaseClassType;
#endif

	typedef typename BaseClassType::result_type		result_type;
	typedef typename BaseClassType::argument_type	argument_type;

	explicit
	DirectoryEnumeratorFunctor(bool		fIncludeSelfAndParent = false) :
		m_includeSelfAndParent(fIncludeSelfAndParent)
	{
	}
			
	void
	operator()(
			const argument_type&	theFullSearchSpec,
			CollectionType&			theCollection) const
	{
		XALAN_USING_STD(back_inserter)

		EnumerateDirectory(
				theFullSearchSpec,
				XALAN_STD_QUALIFIER back_inserter(theCollection),
				m_filterPredicate,
				m_conversionFunction,
				m_includeSelfAndParent);
	}

	result_type
	operator()(const argument_type&		theFullSearchSpec) const
	{
		result_type		theCollection;

		operator()(
				theFullSearchSpec,
				theCollection);

		return theCollection;
	}

	void
	operator()(
			const argument_type&	theDirectory,
			const argument_type&	theSearchSpec,
			CollectionType&			theCollection) const
	{
		EnumerateDirectory(
				theDirectory,
				theSearchSpec,
				XALAN_STD_QUALIFIER back_inserter(theCollection),
				m_filterPredicate,
				m_conversionFunction,
				m_includeSelfAndParent);
	}

	result_type
	operator()(
			const argument_type&	theDirectory,
			const argument_type&	theSearchSpec) const
	{
		result_type		theCollection;

		operator()(
				theDirectory,
				theSearchSpec,
				theCollection);

		return theCollection;
	}

private:

	FilterPredicateType			m_filterPredicate;

	StringConversionFunction	m_conversionFunction;

	const bool					m_includeSelfAndParent;
};
#endif



XALAN_CPP_NAMESPACE_END



#endif	// DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680
