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

#if !defined(XALANMATCHPATTERNDATAALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANMATCHPATTERNDATAALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include "xalanc/XSLT/XSLTDefinitions.hpp"



#include "xalanc/XSLT/XalanMatchPatternData.hpp"



#include "xalanc/PlatformSupport/ArenaAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XalanMatchPatternDataAllocator
{
public:

    typedef XalanMatchPatternData           data_type;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
    typedef ArenaBlock<data_type>           ArenaBlockType;
    typedef ArenaAllocator<data_type,
                           ArenaBlockType>  ArenaAllocatorType;
#else
    typedef ArenaAllocator<data_type>       ArenaAllocatorType;
#endif

    typedef ArenaAllocatorType::size_type   size_type;

    /**
     * Construct an instance that will allocate blocks of the specified size.
     *
     * @param theBlockSize The block size.
     */
    XalanMatchPatternDataAllocator(MemoryManagerType&  theManager, size_type    theBlockCount);

    ~XalanMatchPatternDataAllocator();

    /**
     * Construct an instance
     * 
     * @param constructionContext context for construction of object
     * @param stylesheetTree      stylesheet containing element
     * @param atts                list of attributes for element
     * @param lineNumber                line number in document
     * @param columnNumber          column number in document
     *
     * @param theTemplate The ElemTemplate node that contains the template for this pattern
     * @param thePosition The position in the stylesheet
     * @param theTargetString The target string for match pattern
     * @param TheMatchPattern The match pattern
     * @param thePatternString the pattern string
     * @param thePriority The priority for the match pattern.
     *
     * @return A pointer to the new instance.
     */
    data_type*
    create(
            const ElemTemplate&     theTemplate,
            data_type::size_type    thePosition,
            const XalanDOMString&   theTargetString,
            const XPath&            theMatchPattern,
            const XalanDOMString&   thePatternString,
            data_type::eMatchScore  thePriority);

    /**
     * Determine if an object is owned by the allocator...
     */
    bool
    ownsObject(const data_type*     theObject)
    {
        return m_allocator.ownsObject(theObject);
    }

    /**
     * Delete all objects from the allocator.    
     */ 
    void
    reset()
    {
        m_allocator.reset();
    }

    /**
     * Get the number of ArenaBlocks currently allocated.
     *
     * @return The number of blocks.
     */
    size_type
    getBlockCount() const
    {
        return m_allocator.getBlockCount();
    }

    /**
     * Get size of an ArenaBlock, that is, the number
     * of objects in each block.
     *
     * @return The size of the block
     */
    size_type
    getBlockSize() const
    {
        return m_allocator.getBlockSize();
    }

private:

    // Not implemented...
    XalanMatchPatternDataAllocator(const XalanMatchPatternDataAllocator&);

    XalanMatchPatternDataAllocator&
    operator=(const XalanMatchPatternDataAllocator&);

    // Data members...
    ArenaAllocatorType  m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANMATCHPATTERNDATAALLOCATOR_INCLUDE_GUARD_12455133
