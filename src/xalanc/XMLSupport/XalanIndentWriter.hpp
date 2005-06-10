/*
 * Copyright 1999-2005 The Apache Software Foundation.
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
#if !defined(XALANIDENTWRITER_HEADER_GUARD_1357924680)
#define XALANIDENTWRITER_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>

#include <xalanc/Include/XalanVector.hpp>

#include <xalanc/PlatformSupport/Writer.hpp>


XALAN_CPP_NAMESPACE_BEGIN


template<class WhiteSpaceWriter,
         class NewLineWriter>
class XalanIndentWriter
{
public:
    typedef typename XalanDOMString::size_type       size_type;
    typedef XalanVector<bool>               BoolStackType;
    typedef typename WhiteSpaceWriter::writer_type   writer_type;

    XalanIndentWriter(
            writer_type&    writer,
            size_type       ident) :
        m_whiteSpaceWriter(writer),
        m_newLineWriter(writer),
        m_indent(ident),
        m_currentIndent(0),
        m_startNewLine(false),
        m_ispreserve(false),
        m_isprevtext(false),
        m_preserves(writer.getMemoryManager(), 5)
    {
        assert( m_indent != -1);
    }

    ~XalanIndentWriter()
    {
    }
        
        
    void
    setIndent(size_type value)
    {
        m_indent = value;
    }
    
    size_type
    getIndent()const
    {
        return m_indent;
    }
    
    void
    indent()
    {
        if( shouldIndent() )
        {
            if(m_startNewLine == true)
            {
                m_newLineWriter();
            }

            m_whiteSpaceWriter(m_currentIndent);

        }
    }
    
    void
    increaseIndent()
    {
        m_currentIndent += m_indent;
    }
    
    void
    decreaseIndent()
    {
        assert( m_currentIndent >= m_indent);

        m_currentIndent -= m_indent;
    }       
    
    void
    setStartNewLine(bool value)
    {
        m_startNewLine = value;
    }

    /**
     * Output a line break.
     */
    void
    outputLineSep()
    {
        m_newLineWriter();
    }

    void
    setPrevText(bool value)
    {
        m_isprevtext = value;
    }

    void
    setPreserve(bool value)
    {
        m_ispreserve = value;
    }

    void
    pop_preserve()
    {
        if ( m_preserves.empty() )
        {
            m_ispreserve = false;
        }
        else
        {
            m_ispreserve = m_preserves.back();

            m_preserves.pop_back();
        }
    }

    void
    push_preserve()
    {
        m_preserves.push_back(m_ispreserve);
    }

private:
    bool
    shouldIndent() const
    {
       return (!m_ispreserve && !m_isprevtext);
       // return true;
    }

private:

     //data members
     
    WhiteSpaceWriter m_whiteSpaceWriter;

    NewLineWriter    m_newLineWriter;

    /**
     * Amount to indent.
     */
     size_type      m_indent;

    /**
     * Flag to keep track of the indent amount.
     */
     size_type      m_currentIndent;

     /**
     * Flag to signal that a newline should be added.
     */
     bool        m_startNewLine;

     /**
     * State flag to tell if preservation of whitespace 
     * is important.
     */
    bool        m_ispreserve; 

     /**
     * State flag that tells if the previous node processed 
     * was text, so we can tell if we should preserve whitespace.
     */
    bool        m_isprevtext; 

    /**
     * Stack to keep track of whether or not we need to 
     * preserve whitespace.
     */
    BoolStackType   m_preserves;
};

XALAN_CPP_NAMESPACE_END



#endif  // XALANIDENTWRITER_HEADER_GUARD_1357924680
