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

#if !defined(XALAN_MATCHPATTERNDATA_HEADER_GUARD)
#define XALAN_MATCHPATTERNDATA_HEADER_GUARD



// Base include file.  Must be first.
#include "xalanc/XSLT/XSLTDefinitions.hpp"



#include <cstddef>



#include "xalanc/XalanDOM/XalanDOMString.hpp"



#include "xalanc/XPath/XPath.hpp"



XALAN_CPP_NAMESPACE_BEGIN


class ElemTemplate;



/**
 * This class contains information concerning a match pattern in
 * a stylesheet.
 */
class XALAN_XSLT_EXPORT XalanMatchPatternData
{   

public:

    typedef XPath::eMatchScore  eMatchScore;

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t		size_type;
#else
	typedef size_t			size_type;
#endif


    /**
     * Construct a XalanMatchPatternData from a pattern and template.
     *
     * @param theTemplate The ElemTemplate node that contains the template for this pattern
     * @param thePosition The position in the stylesheet
     * @param theTargetString The target string for match pattern
     * @param TheMatchPattern The match pattern
     * @param thePatternString the pattern string
     * @param thePriority The priority for the match pattern.
     */
    XalanMatchPatternData(
            MemoryManagerType&      theManager,
            const ElemTemplate&     theTemplate,
            size_type               thePosition,
            const XalanDOMString&   theTargetString,
            const XPath&            theMatchPattern,
            const XalanDOMString&   thePatternString,
            eMatchScore             thePriority) :
        m_template(&theTemplate),
        m_position(thePosition),
        m_targetString(theTargetString, theManager),
        m_matchPattern(&theMatchPattern),
        m_pattern(&thePatternString),
        m_priority(thePriority)
    {
    }

    ~XalanMatchPatternData()
    {
    }

    /**
     * Retrieve string for target.
     * 
     * @return target string
     */
    const XalanDOMString&
    getTargetString() const
    {
        return m_targetString;
    }

    /**
     * Retrieve the match pattern associated with pattern.
     * 
     * @return XPath for pattern
     */
    const XPath*
    getExpression() const
    {
        return m_matchPattern;
    }

    /**
     * Retrieve position of pattern in stylesheet.
     * 
     * @return The position in the stylesheet
     */
    size_type
    getPosition() const
    {
        return m_position;
    }

    /**
     * Retrieve pattern string.
     * 
     * @return string that contains element pattern
     */
    const XalanDOMString*
    getPattern() const
    {
        return m_pattern;
    }

    /**
     * Retrieve node that contains the template for this pattern.
     * 
     * @return template node
     */
    const ElemTemplate*
    getTemplate() const
    {
        return m_template;
    }

    eMatchScore
    getDefaultPriority() const
    {
        return m_priority;
    }

    double
    getPriorityOrDefault() const;

private:
    // not implemented
    XalanMatchPatternData();
    XalanMatchPatternData( const XalanMatchPatternData&);

    const ElemTemplate*     m_template;

    size_type               m_position;

    XalanDOMString          m_targetString;

    const XPath*            m_matchPattern;

    const XalanDOMString*   m_pattern;

    eMatchScore             m_priority;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_MATCHPATTERNDATA_HEADER_GUARD
