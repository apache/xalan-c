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

#if !defined(XALANSOURCETREEELEMENTNANSALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANSOURCETREEELEMENTNANSALLOCATOR_INCLUDE_GUARD_12455133



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElementNANS.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementNANSAllocator
{
public:

    typedef XalanSourceTreeElementNANS              ObjectType;

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
    XalanSourceTreeElementNANSAllocator(
            MemoryManager&  theManager,
            size_type       theBlockCount);

    ~XalanSourceTreeElementNANSAllocator();

    /**
     * Create an instance.
     * 
     * @param theTagName The tag name of the element
     * @param theLocalName The local name of the attribute
     * @param theNamespaceURI The namespace URI of the attribute
     * @param thePrefix The namespace prefix of the attribute
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
            const XalanDOMString&       theLocalName,
            const XalanDOMString&       theNamespaceURI,
            const XalanDOMString&       thePrefix,
            XalanSourceTreeDocument*    theOwnerDocument,
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
    XalanSourceTreeElementNANSAllocator(const XalanSourceTreeElementNANSAllocator&);

    XalanSourceTreeElementNANSAllocator&
    operator=(const XalanSourceTreeElementNANSAllocator&);

    // Data members...
    ArenaAllocatorType  m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANSOURCETREEELEMENTANSALLOCATOR_INCLUDE_GUARD_12455133
