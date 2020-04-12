# Xalan-C++ basic usage patterns
  
## Introduction

To perform a transformation, use one of the `XalanTransformer`
`transform()` methods.  The transformation requires an XML source
document and an XSL stylesheet. Both of these objects may be
represented by instances of `XSLTInputSource`. You can construct an
`XSLTInputSource` with a string (the system ID for a file or URI), an
input stream, or a DOM.

If you are using an XSL stylesheet to perform a series of
transformations, you can improve performance by calling `transform()` 
with a compiled stylesheet, an instance of `XalanCompiledStylesheet`. 
If you are transforming an XML source more than once, you should call
`transform()` with a parsed XML source, an instance of
`XalanParsedSource`. See
[Performing a series of transformations](#performing-a-series-of-transformations)

If you XML source document contains a stylesheet Processing Instruction
(PI), you do not need to include a stylesheet object when you call
`transform()`.

The transformation output is represented by an `XSLTResultTarget`, which
you can set up to refer to an output stream, the system ID for a file
or URI, or a Formatter for one of the various styles of DOM output.

For detailed API documentation, see the Xalan-C++ API (doxygen).  For
an overview of the command-line utility, see
[Command-Line Utility](commandline.md).

## Basic usage pattern with the XalanTransformer C++ API

Using `XalanTransformer` and the C++ API, you can perform one or more
transformations as described in the following steps.

Note: For a working sample that illustrates these steps, see the
[XalanTransform](samples.md#xalantransform) sample.
      
### 1. Include the required header files

Always start with `xalanc/Include/PlatformDefinitions.hpp`, the
Xalan-C++ base header file.  Also include
`xercesc/util/PlatformUtils.hpp`,
`xalanc/XalanTransformer/XalanTransformer.hpp`, and any other header
files your particular application requires.

```c++
#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
…
```

### 2. Define namespaces

```c++
using xercesc::XMLPlatformUtils;
using xalanc::XalanTransformer;
```

Whilst you can use the standard C++ namespace syntax directly, the
Xerces-C++ and Xalan-C++ namespaces are linked to the version number.
For example, the Xalan namespace is currently `xalanc_1_12`.  The
macros will automatically take care of this when code is re-compiled
against a new version of the libraries.  Using the namespaces directly
will require each namespace related statement be updated by hand.

### 3. Initialize Xerces and Xalan

Use the static initializers to initialize the Xalan-C++ and Xerces-C++
platforms. You must initialize Xerces-C++ once per process. You may
initialize and terminate Xalan-C++ multiple times, but this is not
recommended: it is inefficient and is not thread safe.

```c++
XMLPlatformUtils::Initialize();
XalanTransformer::initialize();
```

### 4. Create a XalanTransformer

```c++
XalanTransformer theXalanTransformer;
```

### 5. Perform each transformation

You can explicitly instantiate `XSLTInputSource` objects for the XML
source document and XSL stylesheet, and an `XSLTResultTarget` object
for the output, and then call `XalanTransformer` `transform()` with
those objects as parameters.  For example:

```c++
XSLTInputSource xmlIn("foo.xml");
XSLTInputSource xslIn("foo.xsl");
XSLTResultTarget xmlOut("foo-out.xml");
int theResult =
    theXalanTransformer.transform(xmlIn,xslIn,xmlOut)
```

Alternatively, you can call `transform()` with the strings (system
identifiers), streams, and/or DOMs that the compiler needs to
implicitly construct the `XSLTInputSource` and `XSLTResultTarget`
objects. For example:

```c++
const char* xmlIn = "foo.xml";
const char* xslIn = "foo.xsl";
const char* xmlOut = "foo-out.xml";
int theResult =
    theXalanTransformer.transform(xmlIn,xslIn,xmlOut)
```

Keep in mind that `XSLTInputSource` and `XSLTResultTarget` provide a
variety of single-argument constructors that you can use in this manner:

* XSLTInputSource(const char* systemID)
* XSLTInputSource(const XMLCh* systemID) (Unicode characters)
* XSLTInputSource(istream* stream)
* XSLTInputSource(XalanNode* node)
* XSLTResultTarget(char* fileName)
* XSLTResultTarget(XalanDOMString& fileName)
* XSLTResultTarget(ostream* stream)
* XSLTResultTarget(ostream& stream)  
* XSLTResultTarget(Writer* characterStream)
* XSLTResultTarget(XalanDocument* document)
* XSLTResultTarget(XalanDocumentFragment* documentFragment)
* XSLTResultTarget(XalanElement* element)
* XSLTResultTarget(FormatterListener& flistener)

Note: Each `transform()` method returns an integer code, 0 for success.
If an error occurs, you can use the `getLastError()` method to return a
pointer to the error message.

### 6. Shut down Xalan

When you shut down Xalan, you may also want to shut down Xerces and ICU
support (if enabled). Keep the following considerations in mind:

* Once you have shut down Xerces, you can no longer make Xalan or
  Xerces calls in the current process.
* Shut down ICU support if you have enabled it, and if the application
  is exiting or no longer requires the ICU.  The
  `XMLPlatformUtils::Terminate()` call does nothing if ICU support has
  not been enabled.
* Ensure that there are no Xalan-C++ or Xerces-C++ objects extant at
  the point of termination.  Any deletion of objects after termination
  could cause errors.

Use the static terminators:

```c++
XalanTransformer::terminate();
XMLPlatformUtils::Terminate();
XalanTransformer::ICUCleanUp();
```

## Using a stylesheet processing instruction

If you want to use the stylesheet referred to by a stylesheet
processing instruction in the XML document, simply call `transform()`
without the second `XSLTInputSource` argument.  For example:

```c++
// foo.xml contains a stylesheet PI
const char* xmlIn = "foo.xml";
const char* xmlOut = "foo-out.xml";
int theResult =
    theXalanTransformer.transform(xmlIn,xmlOut)
```

## Setting Stylesheet Parameters

An XSL stylesheet can include parameters that are set at run time
before a transformation takes place.  Traditionally, a top-level
parameter value is of text string type.  The Xalan library now
supports three types of top-level parameters that can be set.  One is a
text string parameter.  Another is a number parameter of floating point
type double.  The third is a nodeset (`XalanNode *`) parameter, usually
implemented as a parsed document.

Any XObject that is created outside of the transformation can be
associated with a top-level parameter.  The `XalanTransformer` has an
XObject factory whereby top-level parameters can be owned by the
`XalanTransformer` object.

To set a stylesheet parameter, use the `XalanTransformer`
`setStylesheetParam()` method.  The `setStylesheetParam()` method takes
two arguments: the parameter name and the value.  The value can be a
string type, a number double type, an (`XalanNode *`) pointer to a
nodeset or parsed document, or any XObjectPtr returned from an
XObject factory.

Top level parameters are sticky.  Once set to an instance of an
`XalanTransformer` object, they can be used for multiple
transformations.  The `XalanTransformer` `reset()` method prepares a
transformer for a new transformation.  Use the
`clearStylesheetParams()` method to release the top-level stylesheet
parameters.

The `Xalan` command line utility currently supports only a text string
value for a top-level stylesheet parameter.  The single quotes are
required to identify a text string value.

```sh
Xalan -p param1 "'string expression'"foo.xml foo.xsl
```

If the string expression includes spaces or other characters that the
shell intercepts, first enclose the string in single quotes so
Xalan-C++ interprets it as a string expression, and then enclose the
resulting string in double quotes so the shell interprets it as a
single argument.

The [UseStylesheetParam](samples.md#usestylesheetparam) sample
application supports all three types of top-level stylesheet
parameters.

The 'C' language interface `XalanCAPI` also supports the three types of
top-level parameters.  The sample program `TestCAPIparms.c` shows how
to use top-level parameters with 'C' language programs.

Note: The `Xalan` command line utility should be revised to accommodate
the number and nodeset types of top-level stylesheet parameters. Only
text string values are currently supported.

Top-level stylesheet parameters of nodeset type (`XalanNode *`) are
useful for the merging of multiple XML documents.

## Processing output incrementally

`XalanTransformer` provides a `transform()` method that sends the
output in blocks to a callback function, which enables you to begin
processing the output while the transformation is still in process:

```c++
int
transform(const XSLTInputSource& xmlIn,
          const XSLTInputSource& xslIn,
          void* theOutputHandle,
          XalanOutputHandlerType theOutputHandler,
          XalanFlushHanderType theFlushHandler = 0);
```

For an example, see
[XalanTransformerCallback](samples.md#xalantransformercallback).

### Performing a series of transformations

Before Xalan performs a standard transformation, it must parse the XML
document and compile the XSL stylesheet into binary representations. If
you plan to use the same XML document or stylesheet in a series of
transformations, you can improve performance by parsing the XML
document or compiling the stylesheet once and using the binary
representation when you call `transform()`.

`XalanTransformer` includes methods for creating compiled stylesheets
and  parsed XML documents: the `compileStylesheet()` method returns a
`XalanCompiledStylesheet`; the `parseSource()` method returns a pointer
`XalanParsedSource`.

Note: In the case of failure, both methods return 0.

Example using a `XalanCompiledStylesheet` to perform multiple
transformations:

```c++
XalanCompiledStylesheet* compiledStylesheet = 0;
compiledStylesheet = theXalanTransformer.compileStylesheet("foo.xsl");
assert(compiledStylesheet!=0);
theXalanTransformer.transform("foo1.xml", *compiledStylesheet, "foo1.out.");
theXalanTransformer.transform("foo2.xml", *compiledStylesheet, "foo2.out");
…
```

For a working sample, see the
[CompileStylesheet](samples.md#compilestylesheet) sample.

Example using a `XalanParsedSource` for multiple transformations:

```c++
XalanParsedSource* parsedXML = 0;
parsedXML = theXalanTransformer.parseSource("foo.xml");
assert(parsedXML!=0);
theXalanTransformer.transform(*parsedXML, "foo1.xsl", "foo-xsl1.out");
theXalanTransformer.transform(*parsedXML, "foo2.xsl", "foo-xsl2.out");
…
```

For a sample that uses both a parsed XML source and a compiled
stylesheet, see [ThreadSafe](samples.md#threadsafe).

## Working with DOM input and output

You can set up an `XSLTResultTarget` to produce a DOM when you perform
a transformation. You can also use a DOM as input for a transformation.

The following code fragment illustrates the procedures for working with
DOM output:

```c++
// Use the Xerces DOM parser to create a DOMDocument.

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/XalanTransformer.hpp>

using xercesc::DOMDocument;
using xercesc::DOMImplementation;

using xalanc::FormatterToXML;
using xalanc::XalanTransformer;


    // If you want to produce DOM output, create an empty Xerces Document
    // to contain the transformation output.
    
    DOMDocument * theDOM = 
        DOMImplementation::getImplementation()->createDocument();    

    // Now create a FormatterListener which can be used by the transformer
    // to send each output node to the new Xerces document

    FormatterToXercesDOM theFormatter(theDOM);

    // Now do the transform as normal
    XalanTransformer theXalanTransformer
    int theResult = theXalanTransformer.transform(
        "foo.xml", 
        "foo.xsl", 
        theFormatter);
…

    // After you perform the transformation, the DOMDocument contains
    // the output.

```

Note: You can also follow the same process but use a
`FormatterToDeprecatedXercesDOM` if you require a `DOM_Document`
output.  However this is discouraged, as support for the deprecated DOM
may be removed in future releases of Xalan-C++.

If you want to use a Xerces DOM object as input for a transformation
without wrapping the DOM in a `XercesParserLiaison`, see
[passing in a Xerces DOM](#passing-in-a-xerces-dom-to-a-transformation).

### Limitations

Performance is much better when Xalan-C++ uses native source tree
handling rather than interacting with the Xerces `DOMParser`.

If you are using the deprecated DOM, the Xerces `DOMParser` by default,
creates a `DOM_XMLDecNode` in the DOM tree to represent the XML
declaration. The Xalan bridge for the Xerces DOM does not support this
non-standard node type. Accordingly, you must call
`DOMParser::setToCreateXMLDeclTypeNode(false)` *before* you parse the
XML file.  If not, the behavior is undefined, and your application may
crash.

### Passing in a Xerces DOM to a transformation

You may want to use a Xerces DOM that was created without using the
`XalanTransformer` class.  As the following code snippet illustrates,
`XercesDOMWrapperParsedSource` to pass in a Xerces DOM as the source
for an XSL transformation.

```c++
#include <xercesc/parsers/DOMParser.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>

void parseWithXerces(XalanTransformer &xalan, 
                     const XSLTInputSource &xmlInput,
                     const XalanCompiledStylesheet* styleSheet,
                     const XSLTResultTarget &output,
                     XMLFileReporter &logFile)
{
    XercesDOMParser theParser;
  
    // Turn on validation and namespace support.
    theParser.setDoValidation(true);
    theParser.setDoNamespaces(true);

    // Parse the document
  
    theParser.parse(xmlInput);
    DOMDocument *theDOM = theParser.getDocument();
    theDOM->normalize();

    XercesDOMSupport theDOMSupport;
    XercesParserLiaison theParserLiaison;
  
    // Use the DOM to create a XercesDOMWrapperParsedSource, 
    // which you can pass to the transform method.
    try
    {
        const XercesDOMWrapperParsedSource parsedSource(
                                   theDOM,
                                   theParserLiaison,
                                   theDOMSupport,
                                   XalanDOMString(xmlInput.getSystemId()));

        xalan.transform(parsedSource, stylesheet, output);

    }
    catch (…)
    {
      …
    }
}  
```

## Working with XPath expressions

XSL stylesheets use XPath expressions to select nodes, specify
conditions, and generate text for the result tree.  XPath provides an
API that you can call directly.  For example, you may want to select
nodes programmatically and do your own processing without a stylesheet.
Xalan-C++ provides an `XPathEvaluator` interface to simplify the
process of executing XPath expressions.

For an example that executes XPath expressions against XML source
files, see [SimpleXPathAPI](samples.md#simplexpathapi) (takes
advantage of the `XPathEvaluator` interface) and `XPathWrapper`.

## Using TraceListener

`TraceListener` is a debugging abstract base class implemented by
`TraceListenerDefault`. You can use `TraceListener` to trace any
combination of the following:

* Calls to templates
* Calls to template children
* Selection events
* Result tree generation events

To construct a `TraceListener` with `TraceListenerDefault`, you need a
`PrintWriter` and a boolean for each of these four tracing options. You
can then use the `XSLTEngineImpl` `setTraceSelects()` and
`addTraceListener()` methods to add the `TraceListener` to an
`XSLTProcessor`.
 
See the [TraceListen](samples.md#tracelisten) sample
application. The `TraceListen` sample uses `TraceListenerDefault` to
write events to the screen.

## Using the ICU

You can use the
[International Components for Unicode (ICU)](http://icu-project.org) to
extend support for encoding, number formatting, and sorting.

* *Encoding*.  Xerces-C++ and Xalan-C++ use UTF-16 encoding to work
  with Unicode data.  If you integrate the ICU with Xerces-C++, both 
  Xerces-C++ and Xalan-C++ use ICU support for input and output
  transcoding.
* *`format-number()`*.  This XSLT function includes two or three
  arguments (the third is optional): number, format pattern, and
  decimal-format name.  Xalan-C++ ignores the format pattern and
  optional decimal-format name. If you install ICU support for
  format-number(), this function is fully supported with all its
  arguments.
* *`xsl:sort`*.  If you install ICU support for `xml:sort`, Xalan-C++
  implements Unicode-style collation.

If you choose to build Xalan with ICU, you will need to rebuild Xerces
with ICU as well.

### Enabling ICU support for encoding

If you want to enable ICU support for encoding, you must enable ICU
support in your Xerces-C++ build. Xerces-C++ uses ICU for input
encoding. See the
[Xerces-C++ build instructions](http://xerces.apache.org/xerces-c/build-3.html).
With ICU support enabled in Xerces-C++, Xalan-C++ automatically uses
ICU support for output encoding (the `xsl:output` encoding attribute).

### Enabling ICU support for number formatting and sorting 

If you only want to use the ICU to support number formatting and
sorting, you do not need to enable ICU support in Xalan-C++, but you
must do the following in the application where you want to enable ICU
support:

* Define the environment variable `ICUROOT`.
* Substitute ICU support for `format-number()`, `xsl:number`, and/or
  `xsl:sort`.
* Rebuild the Xalan-C++ library to include the `ICUBridge`.

*ICUBridge*

All Xalan-C++ references to ICU are centralized in the ICUBridge
module, which supplies the infrastructure for enabling ICU support for
number formatting and sorting.

```c++
#include <xalanc/ICUBridge/ICUBridge.hpp>
#include <xalanc/ICUBridge/FunctionICUFormatNumber.hpp>
#include <xalanc/ICUBridge/ICUXalanNumberFormatFactory.hpp>
#include <xalanc/ICUBridge/ICUBridgeCollationCompareFunctor.hpp>
```

Set the CMake option `transcoder=icu` when configuring to enable
`ICUBridge`.

*Number formatting*

To enable ICU support for the XSLT `format-number()` function, do the
following:

```c++
// Install ICU support for the format-number() function.
FunctionICUFormatNumber::FunctionICUFormatNumberInstaller theInstaller;
```

*Sorting* 

To enable ICU support for `xsl:sort`, do the following:

```c++
// Set up a StylesheetExecutionContextDefaultobject
// (named theExecutionContext in the following fragment),
// and install the ICUCollationCompareFunctor.
ICUBridgeCollationCompareFunctortheICUFunctor;
theExecutionContext.installCollationCompareFunctor(&theICUFunctor);
```

## Basic XalanTransformer usage pattern with the C API

We also include a simple C interface for using the `XalanTransformer`
class. The [ApacheModuleXSLT](samples.md#apachemodulexslt) sample 
illustrates the use of this C API.

Basic strategy:

### 1. Include the XalanTransformer C API header

```c++
#include <XalanTransformer/XalanCAPI.h>
```

### 2. Initialize Xalan and Xerces

```c++
XalanInitialize();
```

### 3. Create a Xalan transformer

```c++
XalanHandle xalan = NULL;
xalan = CreateXalanTransformer();
```

### 4. Perform each transformation

For example:

```c++
const char * xmlfilename = "foo.xml";
const char* xslfilename = "foo.xsl";
const char* outfilename = "foo.out";
int theResult = 0;
theResult = XalanTransformToFile(xmlfilename,
                                 xslfilename,
                                 outfilename,
                                 xalan);
```

Note: If the XML input file contains a stylesheet processing
instruction that you want to use, use an empty `XSLTInputSource` for
the stylesheet argument.

### 5. Shut down Xalan

```c++
XalanTerminate();
```

## Variations with the XalanTransformer C API

The Xalan C API supports approximately the same set of options as the
C++ API.  In particular, you can

* Use stylesheet processing instructions (PI) to indicate the URI of
  the stylesheet. Supply `NULL` for the stylesheet argument.
* Set stylesheet parameters. Use the `SetStylesheetParam()` function.
* Compile stylesheets. Use the `CompileStylesheet()` method to compile
  a stylesheet, and the `TransformToFileCSS()` or
  `TransformToDataCSS()` function to use the compiled stylesheet in a
  transformation.
* Parse XML sources. Use the `XalanParseSource()` or
  `XalanParseSouceUseXalan()` method.
* Place the transformation output in a character array. Use the
  `TransformToData()` or `TransformToDataCSS()` function. After you
  perform the transformation, use the `XalanFreeData()` function to
  free memory allocated for the output data.
* Send the output to a callback function to process blocks of output
  data as they arrive.

For a sample that sends output in blocks to a callback function, see
the [ApacheModuleXSLT](samples.md#apachemodulexslt) sample.
