#include <string>
#include <vector>

#ifdef _XPathWrapper
#define XPathWrapperDecl __declspec(dllexport)
#else
#define XPathWrapperDecl __declspec(dllimport)
#endif

class XPathWrapperImpl;

class XPathWrapperDecl XPathWrapper
{

public:
	XPathWrapper();
	virtual ~XPathWrapper();

	// Given an xml document and an xpath context and expression in the form of (ascii) string objects,
	// this function parses the XML document, evaluates the xpath and returns the result, as a list of 
	// string objects

	std::vector<std::string> evaluate(
		const std::string& xml, 
		const std::string& context, 
		const std::string& path);

private:
	// not implemented
	XPathWrapper(const XPathWrapper&);
	XPathWrapper& operator=(const XPathWrapper&);
	bool operator==(const XPathWrapper&) const;

	XPathWrapperImpl* pImpl;
};


