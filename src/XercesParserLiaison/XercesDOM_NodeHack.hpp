#if !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
#define XERCESDOM_NODEHACK_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Attr.hpp>



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

	~XercesDOM_AttrHack();
};



#endif	// !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
