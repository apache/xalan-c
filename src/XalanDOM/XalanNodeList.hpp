#if !defined(XALANNODELIST)
#define XALANNODELIST



#include "XalanDOMDefinitions.hpp"



class XalanNode;



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the DOM NodeList interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_DOM_EXPORT XalanNodeList
{
public:

	XalanNodeList();

	virtual
	~XalanNodeList();

	/** @name Get functions. */
	//@{
	/**
	 * Returns the <code>index</code>th item in the collection. 
	 *
	 * If <code>index</code> is greater than or equal to the number of nodes in 
	 * the list, this returns <code>null</code>.
	 *
	 * @param index Index into the collection.
	 * @return The node at the <code>index</code>th position in the 
	 *	 <code>NodeList</code>, or <code>null</code> if that is not a valid 
	 *	 index.
	 */
	virtual XalanNode*
	item(unsigned int	index) const = 0;

	/**
	 * Returns the number of nodes in the list. 
	 *
	 * The range of valid child node indices is 0 to <code>length-1</code> inclusive. 
	 */
	virtual unsigned int
	getLength() const = 0;

protected:

	XalanNodeList(const XalanNodeList&	theSource);

	XalanNodeList&
	operator=(const XalanNodeList&	theSource);

	bool
	operator==(const XalanNodeList& 	theRHS) const;

private:
};



#endif
