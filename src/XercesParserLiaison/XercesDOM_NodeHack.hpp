#if !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
#define XERCESDOM_NODEHACK_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Attr.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_Text.hpp>



// An evil class to hack the Xerces smart-pointer class.  I'm
// only doing this because I have to...
class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_NodeHack : public DOM_Node
{
public:

	XercesDOM_NodeHack(NodeImpl*	theImpl = 0);

	~XercesDOM_NodeHack();

	NodeImpl*
	getImpl() const
	{
		return fImpl;
	}

	static NodeImpl*
	getImpl(const DOM_Node&		theNode)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XercesDOM_NodeHack&)theNode).getImpl();
#else
		return static_cast<const XercesDOM_NodeHack&>(theNode).getImpl();
#endif
	}
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_AttrHack : public DOM_Attr
{
public:

	XercesDOM_AttrHack(AttrImpl*	theImpl = 0);

	XercesDOM_AttrHack(const DOM_Attr&	theSource);

	~XercesDOM_AttrHack();
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_ElementHack : public DOM_Element
{
public:

	XercesDOM_ElementHack(ElementImpl*	theImpl = 0);

	XercesDOM_ElementHack(const DOM_Element&	theSource);

	~XercesDOM_ElementHack();


	const DOMString
	getNodeNameImpl() const;

	const DOMString
	getNodeValueImpl() const;

	const DOMString
	getNamespaceURIImpl() const;

	const DOMString
	getPrefixImpl() const;

	const DOMString
	getLocalNameImpl() const;

	const DOMString
	getTagNameImpl() const;

	const DOMString
	getAttributeImpl(const DOMString&	name) const;

	const DOMString
	getAttributeNSImpl(
			const DOMString&	namespaceURI,
			const DOMString&	localName) const;

	ElementImpl*
	getImpl() const
	{
		return (ElementImpl*)fImpl;
	}

	static ElementImpl*
	getImpl(const DOM_Element&	theNode)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XercesDOM_ElementHack&)theNode).getImpl();
#else
		return static_cast<const XercesDOM_ElementHack&>(theNode).getImpl();
#endif
	}
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_TextHack : public DOM_Text
{
public:

	XercesDOM_TextHack(TextImpl*	theImpl = 0);

	XercesDOM_TextHack(const DOM_Text&	theSource);

	~XercesDOM_TextHack();


	const DOMString
	getNodeNameImpl() const;

	const DOMString
	getNodeValueImpl() const;

	const DOMString
	getNamespaceURIImpl() const;

	const DOMString
	getPrefixImpl() const;

	const DOMString
	getLocalNameImpl() const;

	const DOMString
	getDataImpl() const;

	TextImpl*
	getImpl() const
	{
		return (TextImpl*)fImpl;
	}

	static TextImpl*
	getImpl(const DOM_Text&	theNode)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XercesDOM_TextHack&)theNode).getImpl();
#else
		return static_cast<const XercesDOM_TextHack&>(theNode).getImpl();
#endif
	}
};



#endif	// !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
