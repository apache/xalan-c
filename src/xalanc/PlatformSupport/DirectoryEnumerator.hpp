/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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



#include <cstring>
#if defined(_MSC_VER)
#include <io.h>
#else
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#endif



#include <functional>
#include <iterator>


#include "xercesc/framework/MemoryManager.hpp"



#include "xalanc/PlatformSupport/XalanFileOutputStream.hpp"
#include "xalanc/PlatformSupport/DOMStringHelper.hpp"
#include "xalanc/PlatformSupport/XalanUnicode.hpp"



namespace XALAN_CPP_NAMESPACE {



using xercesc::MemoryManager;



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
        return const_cast<XalanDOMChar*>(reinterpret_cast<const XalanDOMChar*>(&name[0]));
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
#if defined(__SunOS_5_10) && (__SUNPRO_CC >= 0x570)
            struct stat64 stat_Info;

            const int   retCode = stat64(d_name, &stat_Info);
#else
            struct  stat stat_Info;
        
            const int   retCode = stat(d_name, &stat_Info);
#endif

        return retCode == -1 ? false : S_ISDIR(stat_Info.st_mode);
    }

    bool
    isSelfOrParent() const
    {       
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
    }
};

#endif



struct DirectoryFilterPredicate
{
    bool
    operator()(const FindFileStruct& theFindData) const
    {
        return theFindData.isDirectory();
    }
};



struct FilesOnlyFilterPredicate
{
    bool
    operator()(const FindFileStruct& theFindData) const
    {
        DirectoryFilterPredicate        theDirectoryPredicate;

        return !theDirectoryPredicate(theFindData);
               
    }
};



template<class OutputIteratorType,
         class FilterPredicateType,
         class StringType,
         class StringConversionFunction>
void
EnumerateDirectory(
            MemoryManager&              theMemoryManager,
            const StringType&           theFullSearchSpec,
            OutputIteratorType          theOutputIterator,
            FilterPredicateType         theFilterPredicate,
            StringConversionFunction    theConversionFunction,
#if defined(XALAN_TEMPLATE_FUNCTION_NO_DEFAULT_PARAMETERS)
            bool                        fIncludeSelfAndParent)
#else
            bool                        fIncludeSelfAndParent = false)
#endif
{
#if defined(_MSC_VER)
    FindFileStruct      theFindData;
    
    #ifdef _WIN64
        typedef intptr_t    theHandleType;
    #else
        typedef long    theHandleType;
    #endif

#pragma warning(push)
#pragma warning(disable: 4244)
    theHandleType   theSearchHandle =
        _wfindfirst(
            reinterpret_cast<wchar_t*>(const_cast<XalanDOMChar*>(theConversionFunction(theFullSearchSpec))),
            &theFindData);
#pragma warning(pop)

    if (theSearchHandle != -1)
    {
  
        try
        {
            do
            {
                if ((fIncludeSelfAndParent == true || theFindData.isSelfOrParent() == false) &&
                    theFilterPredicate(theFindData) == true)
                {
                    *theOutputIterator = StringType(theFindData.getName(), theMemoryManager);
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

    CharVectorType  theTargetVector(theMemoryManager);

    TranscodeToLocalCodePage(theFullSearchSpec, theTargetVector, false);

    const CharVectorType::size_type     theSize = theTargetVector.size();
    int  indexSuffix=0, indexName=0;
    bool target_Dir = false; 

    if (theSize > 0)
    {
        if (theTargetVector.back() == '*')
        {
            target_Dir = true;
            theTargetVector.pop_back();

            if (theSize == 1)
            {
                theTargetVector.push_back('.');
            }
        
        }
        else
        {
            target_Dir = false;

            while(theTargetVector.back() != '*')
            {
                theTargetVector.pop_back();
                indexSuffix++;
            }

            theTargetVector.pop_back();
            while(theTargetVector.back() != '/')
            { 
                theTargetVector.pop_back();
                indexName++;
            }
        }      

        theTargetVector.push_back('\0');

        const char* const   theSpec = c_str(theTargetVector);
        assert(theSpec != 0);
        
        XalanDOMString      theName(theMemoryManager);
        XalanDOMString      theSuffix(theMemoryManager);
        if ( !target_Dir )
        {
            using std::strlen;

            int lenSpec = strlen(theSpec); 
            theFullSearchSpec.substr(theName, lenSpec, indexName); 
            theFullSearchSpec.substr(theSuffix, lenSpec+indexName+1, indexSuffix);
        }

        DIR* const  theDirectory = opendir(theSpec);

        if (theDirectory != 0)
        {
            chdir(theSpec);
            try
            {
                const FindFileStruct*   theEntry =
                    (FindFileStruct*)readdir(theDirectory);
    
                while(theEntry != 0)
                {
                    if ((fIncludeSelfAndParent == true || theEntry->isSelfOrParent() == false))
                    {
                        if (theFilterPredicate(*theEntry) == true)
                        {
                            if( target_Dir )
                            {
                                *theOutputIterator = StringType(theEntry->getName(), theMemoryManager);
                            }
                            else
                            {
                                XalanDOMString  Getname(theEntry->getName(), theMemoryManager);
                                int Check_1 = Getname.compare(theName);
                                XalanDOMString  GetnameSuffix(theMemoryManager);
                                Getname.substr(GetnameSuffix, Getname.size() -indexSuffix, indexSuffix);            
                                int Check_2 = GetnameSuffix.compare(theSuffix);
                                if ( Check_1 == 1 && (!Check_2) )
                                {
                                *theOutputIterator = StringType(theEntry->getName(), theMemoryManager);
                                }
                            }
                        }
                    }
                    theEntry = (FindFileStruct*)readdir(theDirectory);
                } //while
            }//try

            catch(...)
            {
                closedir(theDirectory);

                throw;
            }
            if( target_Dir )
                           chdir("..");
                         else
                           chdir("../..");
            closedir(theDirectory);
        }
    }

#endif
}



template<class OutputIteratorType,
         class FilterPredicateType,
         class StringType,
         class StringConversionFunction>
void
EnumerateDirectory(
            MemoryManager&              theMemoryManager,
            const StringType&           theDirectory,
            const StringType&           theSearchSpec,
            OutputIteratorType          theOutputIterator,
            FilterPredicateType         theFilterPredicate,
            StringConversionFunction    theConversionFunction,
#if defined(XALAN_TEMPLATE_FUNCTION_NO_DEFAULT_PARAMETERS)
            bool                        fIncludeSelfAndParent)
#else
            bool                        fIncludeSelfAndParent = false)
#endif
{
    StringType  theFullSearchSpec(theDirectory, theMemoryManager);

    theFullSearchSpec += theSearchSpec;

    EnumerateDirectory(
        theMemoryManager,
        theFullSearchSpec,
        theOutputIterator,
        theFilterPredicate,
        theConversionFunction,
        fIncludeSelfAndParent);
}



#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
template<class CollectionType, class StringType>
struct DirectoryEnumeratorFunctor
{
    CollectionType
    operator()(const StringType&    theDirectory) const
    {
        CollectionType      theCollection;

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
struct DirectoryEnumeratorFunctor
{
    explicit
    DirectoryEnumeratorFunctor(
                MemoryManager&  theMemoryManager,
                bool            fIncludeSelfAndParent = false) :
        m_includeSelfAndParent(fIncludeSelfAndParent),
        m_memoryManager(theMemoryManager)
    {
    }
            
    void
    operator()(
            const StringType&    theFullSearchSpec,
            CollectionType&      theCollection) const
    {
        using std::back_inserter;

        EnumerateDirectory(
            m_memoryManager,
            theFullSearchSpec,
            std::back_inserter(theCollection),
            m_filterPredicate,
            m_conversionFunction,
            m_includeSelfAndParent);
    }

    CollectionType
    operator()(const StringType&     theFullSearchSpec) const
    {
        CollectionType     theCollection;

        operator()(
                theFullSearchSpec,
                theCollection);

        return theCollection;
    }

    void
    operator()(
            const StringType&    theDirectory,
            const StringType&    theSearchSpec,
            CollectionType&      theCollection) const
    {
        EnumerateDirectory(
            m_memoryManager,
            theDirectory,
            theSearchSpec,
            std::back_inserter(theCollection),
            m_filterPredicate,
            m_conversionFunction,
            m_includeSelfAndParent);
    }

    CollectionType
    operator()(
            const StringType&    theDirectory,
            const StringType&    theSearchSpec) const
    {
        CollectionType     theCollection;

        operator()(
                theDirectory,
                theSearchSpec,
                theCollection);

        return theCollection;
    }

private:

    FilterPredicateType         m_filterPredicate;

    StringConversionFunction    m_conversionFunction;

    const bool                  m_includeSelfAndParent;

    MemoryManager&              m_memoryManager;
};
#endif



}



#endif  // DIRECTORY_ENUMERATOR_HEADER_GUARD_1357924680
