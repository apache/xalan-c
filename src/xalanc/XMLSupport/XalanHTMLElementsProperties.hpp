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
 */
#if !defined(XALANHTMLELEMENTSPROPERTIES_HEADER_GUARD_1357924680)
#define XALANHTMLELEMENTSPROPERTIES_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XMLSUPPORT_EXPORT XalanHTMLElementsProperties
{
public:

	enum { eMaxAttributes = 6, eMaxAttributeName = 8, eMaxElementName = 10 };

	enum eFlags
	{
		EMPTY = (1 << 1),
		FLOW = (1 << 2),
		BLOCK = (1 << 3),
		BLOCKFORM = (1 << 4),
		BLOCKFORMFIELDSET = (1 << 5),
		CDATA = (1 << 6),
		PCDATA = (1 << 7),
		RAW = (1 << 8),
		INLINE = (1 << 9),
		INLINEA = (1 << 10),
		INLINELABEL = (1 << 11),
		FONTSTYLE = (1 << 12),
		PHRASE = (1 << 13),
		FORMCTRL = (1 << 14),
		SPECIAL = (1 << 15),
		ASPECIAL = (1 << 16),
		HEADMISC = (1 << 17),
		HEAD = (1 << 18),
		LIST = (1 << 19),
		PREFORMATTED = (1 << 20),
		WHITESPACESENSITIVE = (1 << 21),
		HEADELEM = (1 << 22),
		STYLEELEM = (1 << 23),
		SCRIPTELEM = (1 << 24),

		ATTRURL = (1 << 1),
		ATTREMPTY = (1 << 2)
	};

	struct XALAN_XMLSUPPORT_EXPORT InternalAttributeProperties
	{
		typedef unsigned char	FlagsType;

		XalanDOMChar	m_name[eMaxAttributeName + 1];
	
		FlagsType		m_flags;

		/**
		 * Check if particular properties are set for this
		 * instance.  See the eFlag enum for the valid
		 * properties.
		 *
		 * @param theFlags The properties to check.
		 * @return true if the property is set, false if not
		 */
		bool
		is(FlagsType	theFlags) const
		{
			return m_flags & theFlags ? true : false;
		}
	};

	struct XALAN_XMLSUPPORT_EXPORT InternalElementProperties
	{
		typedef InternalAttributeProperties::FlagsType	AttributeFlagsType;
		typedef unsigned int							FlagsType;

		/**
		 * Check if particular attribute properties are set
		 * for this  instance. See the eFlag enum for the valid
		 * properties.
		 *
		 * @param theAttributeName The attribute name.
		 * @param theFlags The properties to check.
		 * @return true if the property is set, false if not
		 */
		bool
		isAttribute(
				const XalanDOMChar*		theAttributeName,
				AttributeFlagsType		theFlags) const;

		/**
		 * Check if particular properties are set for this
		 * instance.  See the eFlag enum for the valid
		 * properties.
		 *
		 * @param theFlags The properties to check.
		 * @return true if the property is set, false if not
		 */
		bool
		is(FlagsType	theFlags) const
		{
			return m_flags & theFlags ? true : false;
		}

		// Data members...
		XalanDOMChar					m_name[eMaxElementName + 1];

		FlagsType						m_flags;

		InternalAttributeProperties		m_attributes[eMaxAttributes + 1];

		/**
		 * Find an instance with the given attribute name.
		 *
		 * @param theAttributeName The attribute name.
		 * @return A reference to an instance.
		 */
		const InternalAttributeProperties&
		findProperties(const XalanDOMChar*	theAttributeName) const;

		static const InternalAttributeProperties	s_dummyProperties;
	};

	/**
	 * This class acts as a proxy for and InternalElementProperties
	 * instance.
	 */
	class XALAN_XMLSUPPORT_EXPORT ElementProperties
	{
	public:

		typedef InternalElementProperties::AttributeFlagsType	AttributeFlagsType;
		typedef InternalElementProperties::FlagsType			FlagsType;

		/**
		 * Constructor
		 * @param theProperties The instance for which this one is a proxy.
		 */
		ElementProperties(const InternalElementProperties*	theProperties = 0) :
			m_properties(theProperties)
		{
		}

		/**
		 * Copy constructor
		 * @param theSource The source instance for the copy.
		 */
		ElementProperties(const ElementProperties&	theSource) :
			m_properties(theSource.m_properties)
		{
		}

		/**
		 * Determine if this is a non-null instance.  You 
		 * must call this before calling any member functions,
		 * if you think the instance may be null.
		 *
		 * @return true if the instance is null, false if not
		 */
		bool
		null() const
		{
			return m_properties == 0 ? true : false;
		}

		/**
		 * Check if particular properties are set for this
		 * instance.  See the eFlag enum for the valid
		 * properties.
		 *
		 * @param theFlags The properties to check.
		 * @return true if the property is set, false if not
		 */
		bool
		is(FlagsType	theFlags) const
		{
			return m_properties->is(theFlags);
		}

		/**
		 * Check if particular attribute properties are set
		 * for this  instance. See the eFlag enum for the valid
		 * properties.
		 *
		 * @param theAttributeName The attribute name.
		 * @param theFlags The properties to check.
		 * @return true if the property is set, false if not
		 */
		bool
		isAttribute(
				const XalanDOMChar*		theAttributeName,
				AttributeFlagsType		theFlags) const
		{
			return m_properties->isAttribute(theAttributeName, theFlags);
		}

	private:

		const InternalElementProperties*	m_properties;
	};

	/**
	 * Find an instance with the given name.
	 *
	 * @param theElementName The element name.
	 * @return A reference to an instance.
	 */
	static ElementProperties
	find(const XalanDOMChar*	theElementName);

private:

	/**
	 * Find an instance with the given name.
	 *
	 * @param theElementName The element name.
	 * @return A reference to an instance.
	 */
	static const InternalElementProperties&
	findProperties(const XalanDOMChar*	theElementName);

	// The order of these is significant!!!

	// The array of properties...
	static const InternalElementProperties			s_elementProperties[];

	// This point to the last of the real propeties in the array.
	static const InternalElementProperties* const	s_lastProperties;

	// This point to the last of the properties in the array, which is
	// a dummy instance we return when an instance that matches the
	// element name cannot be found.
	static const InternalElementProperties* const	s_dummyProperties;


	// These are undefined...
	XalanHTMLElementsProperties();

	~XalanHTMLElementsProperties();

	XalanHTMLElementsProperties&
	operator=(const XalanHTMLElementsProperties&);

};



XALAN_CPP_NAMESPACE_END



#endif	// XALANHTMLELEMENTSPROPERTIES_HEADER_GUARD_1357924680
