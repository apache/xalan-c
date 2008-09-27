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

#if !defined(XALANSOURCETREEELEMENTAALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANSOURCETREEELEMENTAALLOCATOR_INCLUDE_GUARD_12455133



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElementA.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementAAllocator
{
public:

    typedef XalanSourceTreeElementA                 ObjectType;

    typedef ObjectType::IndexType                   IndexType;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
    typedef ArenaBlock<ObjectType>                  ArenaBlockType;

    typedef ArenaAllocator<ObjectType,
                           ArenaBlockType>          ArenaAllocatorType;
#else
    typedef ArenaAllocator<ObjectType>              ArenaAllocatorType;
#endif

    typedef ArenaAllocatorType::size_type           size_type;

    /**
     * Construct an instance that will allocate blocks of the specified size.
     *
     * @param theBlockSize The block size.
     */
    XalanSourceTreeElementAAllocator(
                MemoryManager&  theManager,
                size_type       theBlockCount);

    ~XalanSourceTreeElementAAllocator();

    /**
     * Create an instance.
     * 
     * @param theTagName The tag name of the element
     * @param theOwnerDocument The document that owns the instance
     * @param theAttributes An array of pointers to the attribute instances for the element
     * @param theAttributeCount The number of attributes.
     * @param theParentNode The parent node, if any.
     * @param thePreviousSibling The previous sibling, if any.
     * @param theNextSibling The next sibling, if any.
     * @param theIndex The document-order index of the node.
     *
     * @return pointer to the instance
     */
    ObjectType*
    create(
            const XalanDOMString&       theTagName,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanSourceTreeAttr**       theAttributes,
            XalanSize_t                 theAttributeCount,
            XalanNode*                  theParentNode = 0,
            XalanNode*                  thePreviousSibling = 0,
            XalanNode*                  theNextSibling = 0,
            IndexType                   theIndex = 0);

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
    XalanSourceTreeElementAAllocator(const XalanSourceTreeElementAAllocator&);

    XalanSourceTreeElementAAllocator&
    operator=(const XalanSourceTreeElementAAllocator&);

    // Data members...
    ArenaAllocatorType  m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANSOURCETREEELEMENTAALLOCATOR_INCLUDE_GUARD_12455133
