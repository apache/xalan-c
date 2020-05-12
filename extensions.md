# Xalan-C++ extension functions

## Introduction

At times, you may want to call your own custom C functions from a
stylesheet.  For these situations, Xalan-C++ supports the creation and
use of extension functions.  Xalan-C++ also provides a
[library of extension functions](extensionslib.md)
for your use.

You can think of extension functions as extending the core library of
functions that XPath provides. Like the XPath functions, an extension
function returns an XObject, which may contain a value of any of the
five XSLT data types: `node-set`, `result-tree-fragment`, `string`,
`boolean` or `number`.

You can send arguments to an extension function in the form of XPath
expressions, literals (for `string`, `boolean`, and `number`), the
values returned by other functions, and XSL variables or parameters set
to any of the preceding.

For an example that implements, installs, and uses three extension
functions, see the [External Functions](samples.md#externalfunctions)
sample.

Note: Xalan-C++ does not support extension elements.

## Implementing an extension function

Like the standard XPath functions, the functions you create derive from
the Function base class. Set up your extension function class as
follows:

1. The body of a function is the `execute()` method. Use the
   appropriate `XObjectFactory` method -- `createNumber()`
   in the example below -- to create an `XObject` corresponding to the
   XSLT data type the function returns.
2. Implement a `clone()` method to enable Xalan to create and maintain
    a copy of the extension function.
3. (Optional) As Xalan does for the XPath functions, you may want to
   prevent the compiler from generating an assignment or equality
   operator for this function.

These features all appear in the following example.

```c++
// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>

#include <cmath>
#include <ctime>

#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>

using namespace xalanc;

// This class defines a function that will return the square root
// of its argument.
class FunctionSquareRoot : public Function
{
public:

  /**
   * Execute an XPath function object.  The function must return a valid
   * XObject.
   *
   * @param executionContext executing context
   * @param context          current context node
   * @param opPos            current op position
   * @param args             vector of pointers to XObject arguments
   * @return                 pointer to the result XObject
   */
  virtual XObjectPtr
  execute(
      XPathExecutionContext& executionContext,
      XalanNode* /* context */,
      const XObjectPtr arg,
     	const Locator*	/* locator */) const
  {
    if (args.size() != 1)
    {
      executionContext.error("The square-root() function takes one argument!",
                              context);
    }
    assert(args[0] != 0);
    // Use the XObjectFactory createNumber() method to create an XObject
    // corresponding to the XSLT number data type.
    return executionContext.getXObjectFactory().createNumber(
                                                        sqrt(args[0]->num()));
  }

  /**
   * Implement clone() so Xalan can copy the square-root function into
   * its own function table.
   *
   * @return pointer to the new object
   */
  virtual FunctionSquareRoot*
  clone() const
  {
    return new FunctionSquareRoot(*this);
  }

private:
  // The assignment and equality operators are not implemented…
  FunctionSquareRoot&
  operator=(const FunctionSquareRoot&);
  bool
  operator==(const FunctionSquareRoot&) const;
}
```

## Installing an extension function

[`XalanTransformer`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html)
provides methods for installing and uninstalling external functions:

* [`installExternalFunction()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a7d932d74897e12629afd62bba2735456)
  makes the function available in the current instance of `XalanTransformer`. Use
  [`uninstallExternalFunction()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a8dbb4538357ab2909925dd16a449dbac)
  to remove the function.
* [`installExternalFunctionGlobal()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a7d4f49b03739feaa01cdbffd5668e736)
  makes the function available globally.
  Use
  [`uninstallExternalFunctionGlobal()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#ab5c8f39bcf0bf84b6bbe0607bbe4afde)
  to remove the function. The global install and uninstall operations
  are not thread-safe.  However, all global functions should be
  thread-safe, because multiple threads could call a particular
  function instance at the same time.

These methods include arguments for the namespace, the function name,
and the function implementation.

When you install an extension function, the function inhabits the
namespace you designate. For information about XML namespaces, see
[Namespaces in XML](http://www.w3.org/TR/REC-xml-names/).

The following code fragment installs locally the square root function
defined above, and binds it to the extension-function name
`square-root` in the namespace
`http://MyExternalFunction.mycompany.org` so it can be accessed from
stylesheets. Keep in mind that the function name does not need to be
the same as the name of the function class, and that a function name
may be used more than once provided that each function with that name
is installed in a different namespace.

```c++
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
// You have created a header file for FunctionSquareRoot.
#include <MyFunctions/FunctionSquareRoot.hpp>
// The namespace…
const XalanDOMString	
    theNamespace("http://MyExternalFunction.mycompany.org");

theXalanTransformer.installExternalFunction(theNamespace,
                                            XalanDOMString("square-root"),
                                            FunctionSquareRoot());
```

For an example that installs three functions, see the
[External Functions](samples.md#externalfunctions) sample.

## Using an extension function

To use the extension function in a stylesheet, you must do the following:

1. Declare the extension function namespace.
   <br>
   `xmlns:prefix=URI`
   <br>
   The `prefix` identifies the namespace, and `URI` matches the
   namespace specified when the function is installed.
   <br>
   By default, namespace declarations are included in the
   transformation output. To exclude namespaces from the output,
   use
   <br>
   `exclude-result-prefixes="prefix-1 prefix-2 …"`
   <br>
   in the stylesheet element or
   <br>
   `xsl:exclude-result-prefixes="prefix-1 prefix-2 …"`
   <br>
   in a literal result element or extension element.
2. Call the extension function in the same manner you would call an
   XPath function. The function name you use in the stylesheet is a
   Qualified Name (QName) made up of the prefix you declared in step 1
   and the function name you specified when you installed the function.
   <br>
   You can use XPath expressions, literals (for `string`, `boolean`,
   and `number`), and values returned by other functions to specify
   function arguments.

Suppose, for example, you are working with XML documents containing
area elements like `<area value="397"/>`, where the value attribute
identifies the area of a square.

The following stylesheet declares the square-root function namespace
(the prefix is up to you), instructs the processor not to copy the
namespace declaration to the result tree, and uses the square-root
function to return the square root of `//area/@value:`

```xml
<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0"
	xmlns:external="http://ExternalFunction.xalan-c.xml.apache.org"
  exclude-result-prefixes="external">

    <xsl:template match="//area">
    <out>
      The area of the square is
      <xsl:value-of select="@value"/> square units.
      The length of each side is
      <xsl:value-of select="external:square-root(@value)"/> units
    </out>
    </xsl:template>
</xsl:stylesheet>
```

This stylesheet converts `<area value="397"/>` into the following
output:

```xml
<out>
  The area of the square is
  397 square units.
  The length of each side is
  19.9249 units.
</out>
```

For a slightly more complex variation on this example, see the
[External Functions](samples.md#externalfunctions) sample

### Passing Nodes to a function

Please keep in mind that *all* `LocationPath` expressions return a
`node-set`, even if the expression only returns a single attribute or a
`text` node (`node-set`s with one member). You can use the XSLT
`string()` function to convert a `node-set` value to `string`, and the
`number()` function to convert a `node-set` value to `number` (a
double).

If you pass a `node-set` to an extension function, be sure to set up
the function to process a node-set.

Suppose, for example, you have a `ProcessNodes` function class that
uses

```c++
const NodeRefListBase& theNodeList = args[0]->nodeset();
```

in the `execute()` method to get a reference to the `node-set`.

Assuming you install the function as `ProcessNodes` and use the
`node-ext` prefix in a stylesheet to refer to the `ProcessNodes`
function namespace, any of the following function calls are
syntactically possible:

```xml
<!--Process the current node-->
<xsl:variable name="success" select="node-ext:ProcessNodes(.)"/>

<!--Process all nodes in current context-->
<xsl:variable name="success" select="node-ext:ProcessNodes(*)"/>

<!-- Process all nodes -->
<xsl:variable name="success" select="node-ext:ProcessNodes(/*)"/>

<!--Process the foo/baz nodes in current context -->
<xsl:variable name="success" select="node-ext:ProcessNodes(foo/baz)"/>

<!--Process the/foo/baz and /bar/saz nodes -->
<xsl:variable name="success" select="node-ext:ProcessNodes(/foo/baz | /bar/saz)"/>
```

The `NodeRefListBase` is in fact a list of references into the XML
document, so keep in mind that
[`getNextSibling()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanNode.html#a9b4a7333371ffee9a6fd851203f4c98a),
for example, gets you the next sibling in the document, which may not
be the next `Node` in the `node-set`.
