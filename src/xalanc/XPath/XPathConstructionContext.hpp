/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(XPATHCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680)
#define XPATHCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



//
// An abstract class which provides support for constructing the internal
// representation  of a stylesheet.
//
class XALAN_XPATH_EXPORT XPathConstructionContext
{
public:

	XPathConstructionContext();

	virtual
	~XPathConstructionContext();


	/**
	 * Reset the instance.  Any existing objects
	 * created by the instance will be destroyed.
	 */
	virtual void
	reset() = 0;

	/**
	 * Get a pooled string given the source string.  If
	 * the string already exists in the pool, no copy
	 * will be made.  If not, a copy will be made and
	 * kept for later use.
	 *
	 * @param theString The source string
	 * @return a const reference to a pooled string.
	 */
	virtual const XalanDOMString&
	getPooledString(const XalanDOMString&	theString) = 0;

	/**
	 * Get a pooled string given the source character
	 * array.  If the string already exists in the pool,
	 * no copy will be made.  If not, a copy will be made
	 * and kept for later use.
	 *
	 * @param theString The source character array
	 * @param theLength The length of the character array
	 * @return a const reference to a pooled string.
	 */
	virtual const XalanDOMString&
	getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	/**
	 * Get a cached string for temporary use.
	 *
	 * @return A reference to the string
	 */
	virtual XalanDOMString&
	getCachedString() = 0;

	/**
	 * Return a cached string.
	 *
	 * @param theString The string to release.
	 *
	 * @return true if the string was released successfully.
	 */
	virtual bool
	releaseCachedString(XalanDOMString&		theString) = 0;

	class GetAndReleaseCachedString
	{
	public:

		GetAndReleaseCachedString(XPathConstructionContext&		theConstructionContext) :
			m_constructionContext(&theConstructionContext),
			m_string(&theConstructionContext.getCachedString())
		{
		}

		// Note non-const copy semantics...
		GetAndReleaseCachedString(GetAndReleaseCachedString&	theSource) :
			m_constructionContext(theSource.m_constructionContext),
			m_string(theSource.m_string)
		{
			theSource.m_string = 0;
		}

		~GetAndReleaseCachedString()
		{
			if (m_string != 0)
			{
				m_constructionContext->releaseCachedString(*m_string);
			}
		}

		XalanDOMString&
		get() const
		{
			assert(m_string != 0);

			return *m_string;
		}

		XPathConstructionContext&
		getConstructionContext() const
		{
			return *m_constructionContext;
		}

	private:

		// Not implemented...
		GetAndReleaseCachedString&
		operator=(const GetAndReleaseCachedString&);


		// Data members...
		XPathConstructionContext*	m_constructionContext;

		XalanDOMString*				m_string;
	};

};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680
