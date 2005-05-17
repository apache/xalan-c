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
#if !defined(XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680)
#define XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanMemoryManagement.hpp>

#include <cstddef>

#include <cassert>

#include <utility>



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)

// An auto_ptr-like class that supports the MemoryManager class.
template<
            class   Type, 
            bool    toCallDestructor = true>
class XalanMemMgrAutoPtr
{
public:

    typedef XALAN_STD_QUALIFIER pair<MemoryManager*, Type*> AutoPtrPairType;

    class MemMgrAutoPtrData : public AutoPtrPairType
    {
    public:
        MemMgrAutoPtrData():
            AutoPtrPairType(0,0)
        {
        }

        MemMgrAutoPtrData(  
                MemoryManager*  memoryManager,
                Type*           dataPointer): 
            AutoPtrPairType(memoryManager, dataPointer)
        {
            invariants();
        }
        
        bool
        isInitilized()const
        {
            return this->first != 0 && this->second != 0;
        }

        void
        deallocate()
        {
            invariants();

            if ( isInitilized() )
            {       
                if ( toCallDestructor ) 
                {
                    this->second->~Type();
                }

                this->first->deallocate(this->second);
            }
        }

        void 
        reset(
                MemoryManager*      memoryManager ,
                Type*               dataPointer)
        {   
            invariants();

            this->first = memoryManager;

            this->second = dataPointer;

            invariants();
        }

    private:

        void
        invariants() const
        {
            assert(
                isInitilized() ||
                (this->first == 0 && this->second == 0));
        }
    };


    XalanMemMgrAutoPtr(
            MemoryManager&  theManager, 
            Type*           ptr) : 
        m_pointerInfo(&theManager, ptr)
    {
    }

    XalanMemMgrAutoPtr() :
        m_pointerInfo()
    {
    }

    XalanMemMgrAutoPtr(const XalanMemMgrAutoPtr<Type, toCallDestructor>&    theSource) :
        m_pointerInfo(((XalanMemMgrAutoPtr<Type>&)theSource).release())
    {
    }

    XalanMemMgrAutoPtr<Type,toCallDestructor>&
    operator=(XalanMemMgrAutoPtr<Type,toCallDestructor>&    theRHS)
    {       
        if (this != &theRHS)
        {
            m_pointerInfo.deallocate();

            m_pointerInfo = theRHS.release();
        }

        return *this;
    }

    ~XalanMemMgrAutoPtr()
    {
        m_pointerInfo.deallocate();
    }

    Type&
    operator*() const
    {
        return *m_pointerInfo.second;
    }

    Type*
    operator->() const
    {
        return m_pointerInfo.second;
    }

    Type*
    get() const
    {
        return m_pointerInfo.second;
    }

    MemoryManager*
    getMemoryManager()
    {
        return m_pointerInfo.first;
    }

    const MemoryManager*
    getMemoryManager() const
    {
        return m_pointerInfo.first;
    }

    MemMgrAutoPtrData
    release()
    {       
        MemMgrAutoPtrData tmp = m_pointerInfo;
    
        m_pointerInfo.reset(0, 0); 
        
        return MemMgrAutoPtrData(tmp);
    }

    Type*
    releasePtr()
    {       
        MemMgrAutoPtrData tmp = release();
    
        return tmp.second;
    }   
    
    void
    reset(
            MemoryManager*  theManager = 0,
            Type*           thePointer = 0)
    {       
        m_pointerInfo.deallocate();

        m_pointerInfo.reset(theManager, thePointer);
    }

private:

    // data member
    MemMgrAutoPtrData   m_pointerInfo;
};




template<class Type>
class XalanMemMgrAutoPtrArray
{
public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
    typedef std::size_t     size_type;
#else
    typedef size_t          size_type;
#endif

    class MemMgrAutoPtrArrayData 
    {
    public:

        MemMgrAutoPtrArrayData():
            m_memoryManager(0),
            m_dataArray(0),
            m_size(0)
        {
        }

        MemMgrAutoPtrArrayData( 
                MemoryManager*  memoryManager,
                Type*           dataPointer,
                size_type       size): 
            m_memoryManager(memoryManager),
            m_dataArray(dataPointer),
            m_size(size)
        {
            invariants();
        }
    
        bool
        isInitilized()const
        {
            return m_memoryManager != 0 && m_dataArray != 0  && m_size != 0;
        }

        void
        deallocate()
        {
            invariants();

            if ( isInitilized() )
            {           
                assert ( m_dataArray != 0 );

                for ( size_type i = 0; i < m_size ; ++i )
                {
                    m_dataArray[i].~Type();
                }

                m_memoryManager->deallocate(m_dataArray);
            }
        }

        void 
        reset(  
                MemoryManager*  theMemoryManager,
                Type*           thePointer,
                size_type       size)
        {   
            invariants();

            m_memoryManager = theMemoryManager;

            m_dataArray = thePointer;

            m_size = size;

            invariants();
        }   

        MemoryManager*  m_memoryManager;

        Type*           m_dataArray;

        size_type       m_size;

    private:

        void
        invariants()const
        {
            assert(
                isInitilized() ||
                (m_memoryManager == 0 && m_dataArray == 0 && m_size == 0));
        }
    };

    XalanMemMgrAutoPtrArray(
            MemoryManager&  theManager, 
            Type*           ptr,
            size_type       size) : 
        m_pointerInfo(
            &theManager,
            ptr,
            size)
    {
    }   

    XalanMemMgrAutoPtrArray() :
        m_pointerInfo()
    {
    }

    XalanMemMgrAutoPtrArray(const XalanMemMgrAutoPtrArray<Type>&    theSource) :
        m_pointerInfo(((XalanMemMgrAutoPtr<Type>&)theSource).release())
    {
    }

    XalanMemMgrAutoPtrArray<Type>&
    operator=(XalanMemMgrAutoPtrArray<Type>&    theRHS)
    {       
        if (this != &theRHS)
        {
            m_pointerInfo.deallocate();

            m_pointerInfo = theRHS.release();
        }

        return *this;
    }

    ~XalanMemMgrAutoPtrArray()
    {
        m_pointerInfo.deallocate();
    }

    Type&
    operator*() const
    {
        return *m_pointerInfo.m_dataArray;
    }

    Type*
    operator->() const
    {
        return m_pointerInfo.m_dataArray;
    }

    Type*
    get() const
    {
        return m_pointerInfo.m_dataArray;
    }

    size_type
    getSize()const
    {
        return m_pointerInfo.m_size;
    }

    MemoryManager*
    getMemoryManager()
    {
        return m_pointerInfo.m_memoryManager;
    }

    const MemoryManager*
    getMemoryManager() const
    {
        return m_pointerInfo.m_memoryManager;
    }

    XalanMemMgrAutoPtrArray<Type>&
    operator++ ()
    {
        ++m_pointerInfo.m_size;

        return *this;
    }

    /* Since this class is not reference-counted, I don't see how this
       could work, since the destruction of the temporary will free
       the controlled pointer.
    XalanMemMgrAutoPtrArray<Type>
    operator++ (int)
    {
        XalanMemMgrAutoPtrArray<Type> temp = *this;
        ++*this;

        return temp;
    }
    */

    MemMgrAutoPtrArrayData
    release()
    {
        MemMgrAutoPtrArrayData tmp = m_pointerInfo;

        m_pointerInfo.reset(0, 0, 0); 

        return MemMgrAutoPtrArrayData(tmp);
    }

    Type*
    releasePtr()
    {       
        MemMgrAutoPtrArrayData  tmp = release();

        return tmp.m_dataArray;
    }

    void
    reset( 
            MemoryManager*  theManager = 0,
            Type*           thePointer = 0 ,
            size_type       size = 0)
    {       
        m_pointerInfo.deallocate();

        m_pointerInfo.reset(theManager, thePointer, size);
    }
    
    Type&
    operator[](size_type    index) const
    {
        return m_pointerInfo.m_dataArray[index];
    }

private:

    // data member
    MemMgrAutoPtrArrayData m_pointerInfo;
};




XALAN_CPP_NAMESPACE_END



#endif  // if !defined(XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680)
