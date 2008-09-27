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

#if !defined(XERCESELEMENTWRAPPERALLOCATOR_INCLUDE_GUARD_12455133)
#define XERCESELEMENTWRAPPERALLOCATOR_INCLUDE_GUARD_12455133



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



#include <xalanc/XercesParserLiaison/XercesElementWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XERCESPARSERLIAISON_EXPORT XercesElementWrapperAllocator
{
public:

    typedef XercesElementWrapper                    ObjectType;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
    typedef ArenaBlock<ObjectType>                  ArenaBlockType;

    typedef ArenaAllocator<ObjectType,
                           ArenaBlockType>          ArenaAllocatorType;
#else
    typedef ArenaAllocator<ObjectType>              ArenaAllocatorType;
#endif

    typedef ArenaAllocatorType::size_type   size_type;

    /**
     * Construct an instance that will allocate blocks of the specified size.
     *
     * @param theBlockSize The block size.
     */
    XercesElementWrapperAllocator(MemoryManager&  theManager, size_type theBlockCount);

    ~XercesElementWrapperAllocator();
    
    /**
     * Create a XercesElementWrapper instance.
     * 
     * @param theXercesElement The Xerces element node
     * @param theNavigator The navigator for this instance.
     *
     * @return pointer to the instance
     */
    ObjectType*
    create(
            const DOMElementType*           theXercesElement,
            const XercesWrapperNavigator&   theNavigator);

    /**
     * Delete all objects from allocator.    
     */ 
    void
    reset();

    /**
     * Get size of an ArenaBlock, that is, the number
     * of objects in each block.
     *
     * @return The size of the block
     */
    size_type
    getBlockCount() const
    {
        return m_allocator.getBlockCount();
    }

    /**
     * Get the number of ArenaBlocks currently allocated.
     *
     * @return The number of blocks.
     */
    size_type
    getBlockSize() const
    {
        return m_allocator.getBlockSize();
    }


private:

    // Not implemented...
    XercesElementWrapperAllocator(const XercesElementWrapperAllocator&);

    XercesElementWrapperAllocator&
    operator=(const XercesElementWrapperAllocator&);

    // Data members...
    ArenaAllocatorType  m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif  // XERCESELEMENTWRAPPERALLOCATOR_INCLUDE_GUARD_12455133
