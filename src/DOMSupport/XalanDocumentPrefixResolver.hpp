/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(XALANDOCUMENTPREFIXRESOLVER_HEADER_GUARD_1357924680)
#define XALANDOCUMENTPREFIXRESOLVER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <DOMSupport/DOMSupportDefinitions.hpp>



#include <map>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/PrefixResolver.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>



#include <include/STLHelper.hpp>



#include <DOMSupport/TreeWalker.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanAttr;
class XalanDocument;



class XALAN_DOMSUPPORT_EXPORT XalanDocumentPrefixResolver : public PrefixResolver
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<const XalanNode*> 					AttributeVectorType;

	typedef map<const XalanDOMString*,
				AttributeVectorType,
				pointer_less<XalanDOMString> >			NamespacesMapType;
#else
	typedef std::vector<const XalanNode*> 				AttributeVectorType;

	typedef std::map<const XalanDOMString*,
					 AttributeVectorType,
					 pointer_less<XalanDOMString> >		NamespacesMapType;
#endif

	/**
	 * Constructor.
	 *
	 * @param theDocument The XalanDocument instance to use for namespace bindings.
	 * @param theURI The base URI for the instance.  This is rarely needed, but can be provided if the URI for the XalanDocument instance is known.
	 */
	XalanDocumentPrefixResolver(
			const XalanDocument*	theDocument,
			const XalanDOMString&	theURI = XalanDOMString());

	virtual
	~XalanDocumentPrefixResolver();


	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual const XalanDOMString&
	getURI() const;

	/**
	 * A utility class to walk the XalanDocument instance and look for attribute nodes that declare namespaces.
	 */
	class NamespaceNodesTreeWalker : public TreeWalker
	{
	public:

		NamespaceNodesTreeWalker(NamespacesMapType&		theMap);

		virtual
		~NamespaceNodesTreeWalker();

	protected:

		virtual bool
		startNode(const XalanNode*	node);

		virtual bool
		startNode(XalanNode*	node);

		virtual bool
		endNode(const XalanNode*	node);

		virtual bool
		endNode(XalanNode*	node);

	private:

		NamespacesMapType&	m_map;
	};


protected:

	/**
	 * This function is called when more than one binding exists for a given prefix.
	 * The default behavior uses the first binding, but a deriving class can change
	 * that behavior by overriding.
	 *
	 * @param theVector The vector associated with the namespace prefix. 
	 * @return A pointer to a const XalanDOMString containing the desired binding, or NULL.
	 */
	virtual const XalanDOMString*
	duplicateBinding(const AttributeVectorType&		theVector) const;

private:

	NamespacesMapType		m_namespaces;

	const XalanDOMString	m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANDOCUMENTPREFIXRESOLVER_HEADER_GUARD_1357924680
