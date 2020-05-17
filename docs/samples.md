# Xalan-C++ Samples

## Samples to help you get started

Each of the subdirectories in the Xalan-C++
[*samples*](https://github.com/apache/xalan-c/tree/master/samples)
directory contains the source files for a sample application.

With most of the samples, you can use the following procedure:

1. Go to the samples subdirectory containing the sample (use the CMD
   shell or PowerShell if you are running Windows)
2. Run the sample from the command line (as indicated below)
3. Examine the application source files. You may also want to modify
   the source files. Remember that if you modify a *.cpp* file, you
   must rebuild the executable and place it on the path before you can
   run the modified application.

Note: Each sample application looks for input files in the current
directory, the directory from which you run the application. The
input files are in the samples subdirectory along with the sample
source files.  The location of the sample executables may vary
depending upon the CMake generator used for building.  They should
typically be located within the *builddir/samples/\<sample\>*.  In all
cases, be sure the sample executable is on the `PATH`, and run it
from the samples subdirectory that contains the input files.

Note: The most of the samples are implemented without providing a
pluggable memory manager. [SimpleTransform](#simpletransform)
sample illustrates, in addition to a simple transformation, the usage
of the processor with memory manager.


## ApacheModuleXSLT

Note: This sample must be built with the Apache Web server.

What it does: runs as an Apache module on an Apache Web server;
performs transformations and returns the output to a Web browser. You
configure Apache to respond to a given URL request for an output file
(html or txt file in the configuration below) by applying an XSL
stylesheet file to an XML document file (both with the specified name
in a given location) and returning the transformation output to the
client.

This sample also illustrates use of the
[`XalanTransformer`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html)
class and the C API defined in
[*xalanc/XalanTransformer/XalanCAPI.h*](https://apache.github.io/xalan-c/api/XalanCAPI_8h.html#ab9ec00bf3fbe6a946e20418a53891755).
It returns  transformation output in blocks to a callback function,
which enables the browser to start displaying the result before the
transformation has been completed.

Note: You may need to configure CMake to locate the required Apache
header files.

### Setting up and using ApacheModuleXSLT

To use `ApacheModuleXSLT`, do the following:

1. (UNIX only) Be sure the Xalan and Xerces libraries are on your
   system library path, and copy the Apache module to */usr/lib/apache*.
2. Add `LoadModule` and (UNIX only) `AddModule` entries to the Apache
   configuration file, *httpd.conf*.
   Windows: `LoadModule xslt_module Xalan-C_1_12_0;-<my_Windows_distribution>\bin\ApacheModuleXSLT.dll`
   UNIX: `AddModule mod_xslt.c` and
      `LoadModule xslt_module /usr/lib/apache/mod_xslt.<ref>xx</ref>`
      where `<ref>xx</ref>` is the appropriate library suffix for the
      UNIX platform ("*.so*" or "*.a*").
3. Add a `<Location>` entry to `httpd.conf` that indicates where
   *.xml* and *.xsl* file pairs are to be found, and what target file
   extensions to recognize. We suggest the following:
   ```xml
   <Location /xslt>
     AddHandler mod_xslt .html
     AddHandler mod_xslt .txt
   </Location>
   ```
   This <Location> element instructs the module to respond to requests
   for *xxx.html* and *xxx.txt* files in the in the *xslt* subdirectory
   (under the document root; see next item) by applying the *xxx.xsl*
   stylesheet to *xxx.xml* (both in that directory) and returning the
   transformation result to the browser.<br>
   For example, a request for *foo.html* instructs the module to apply
   *foo.xsl* to *foo.xml* and return the result.<br>
    Note: It is up to the stylesheet to apply the appropriate
    `xsl:output` method to the output. Whether the user specifies
    `html` or `txt` is, of itself, immaterial.
4. Put *.xml* and *.xsl* file pairs in the `<Location>` subdirectory
   (*xslt* in the example) under the document root directory specified
   in *httpd.conf* by the `DocumentRoot` and `<Directory>` settings.
   Alternatively, you can modify these settings to point to
   *samples/ApacheModuleXSLT*, which includes an *xslt* subdirectory
   with *.xml* and *.xsl* file pairs (*foo.xml* and *foo.xsl*,
   *apachemod.xml* and *apachemod.xsl*).
5. Start the Apache server.
6. From a Web browser, call the module with a URL as follows:
   `http://serverName/xslt/xxx.html`
   where `serverName` is the Apache server (such as `www.myServer.com`)
   and `xxx` is the name of an XML/XSL pair of files (such as *foo.xml*
   and *foo.xsl*) in the *xslt* subdirectory under the `DocumentRoot`
   directory.<br>
   For example, `http://www.myServer.com/xslt/apachemod.html`
   instructs `ApacheModuleXSLT` to apply the *apachemod.xsl* stylesheet
   to the *apachemod.xml* XML document (both files in the *xslt*
   directory under the Apache `DocumentRoot` directory) and return the
   transformation result to the browser.

## CompileStylesheet

What it does: Use a compiled stylesheet to perform a series of
transformations.

You can run it from the *CompileStylesheet* subdirectory with
`CompileStylesheet`

See also:
[Performing a series of transformations](usagepatterns.md#performing-a-series-of-transformations).

## DocumentBuilder

What it does: Use a `DocumentBuilder` to programmatically construct an
XML document, apply the *foo.xsl* stylesheet to this document, and
write the ouput to *foo.out*.

You can run it from the *DocumentBuilder* subdirectory with
`DocumentBuilder`.

## ExternalFunction

What it does: implement, install, and illustrate the usage of three
extension functions.  The functions return a square root, a cube, and a
string with the current date and time. The sample stylesheet
(*foo.xsl*) gets the area of a cube and units of measurement from an
XML document (*foo.xml*), computes the length of each side of a cube
and the volume of the cube, and enters the date and time of the
transformation. The output appears in *foo.out*.

Run this sample from the *ExternalFunction* subdirectory with
`ExternalFunction`.

See also: [Extension Functions](faq.md#extension-functions).

## ParsedSourceWrappers

What it does: performs a transformation with input in the form of a
pre-built `XercesDOM` or `XalanSourceTree`.

Run this sample from the *ParsedSourceWrappers* subdirectory with
`ParsedSourceWrappers`

See `transformXercesDOM()` and `transformXalanSourceTree()` as called
by `transform()` in *ParsedSourceWrappers.cpp*.

## SerializeNodeSet

What it does: Serialize the node set returned by the application of an
XPath expression to an XML document.

Run this sample from the *SerializeNodeSet* subdirectory with

```sh
SerializeNodeSet XMLFile ContextNode XPathExpression
```

where *XMLFile* is an XML source file, *ContextNode* is the location
path to the context node, and *XPathExpression* is an XPath expression
to apply to that context node. The *SerializeNodeSet* directory
contains the same *foo.xml* sample source file as the preceding
examples.

## SimpleTransform

What it does: The `SimpleTransform` class uses the *foo.xsl* stylesheet
to transform *foo.xml*, and writes the output to *foo.out*.  The source
for this sample has been modified to demonstrate the usage of the new
pluggable memory management feature.

You can run it from the *SimpleTransform* subdirectory with
`SimpleTransform`.

See also:
[Basic procedures for performing XSL transformations](usagepatterns.md#xalan-c-basic-usage-patterns).

## SimpleXPathAPI

What it does: Use the `XPathEvaluator` interface to evaluate an XPath
expression from the specified context node of an XML file and display
the nodeset returned by the expression.

Note: You can use this sample as an aid when you want to find out what
a given XPath expression returns from a given context node in an XML
file.

Run this sample from the *SimpleXPathAPI* subdirectory with:

```sh
SimpleXPathAPI XMLFile ContextNode XPathExpression
```

where *XMLFile* is an XML source file, *ContextNode* is the location
path to the context node, and *XPathExpression* is an XPath expression
to apply to that context node.

Keep in mind that the string value returned by an XPath expression is
the string value of the first node in the nodeset returned by the
expresssion.

The *XPathWrapper* subdirectory contains an XML file named *xml.foo*
(part of it appears below).

```xml
<?xml version="1.0"?>
<doc>
  <name first="David" last="Marston">Mr. Marson</name>
  <name first="David" last="Bertoni">Mr. Bertoni</name>
  …
  <name first="Paul" last="Dick">Mr. Dick</name>
</doc>
```

You can try command lines like

```sh
SimpleXPathAPI foo.xml /foo:doc foo:name/@last
```

and

```sh
SimpleXPathAPI foo.xml / '//foo:name[position()="4"]/@first'
```

Note: If a `SimpleXPathAPI` argument includes characters (such as `*`)
that the shell interprets incorrectly, enclose the argument in double
quotes.

See also:
[Working with XPath expressions](usagepatterns.md#working-with-xpath-expressions).

## SimpleXPathCAPI

What it does: Use the
[`XPathEvaluator`](https://apache.github.io/xalan-c/api/classxalanc_1_1XPathEvaluator.html)
C interface to evaluate an XPath expression and display the string
value returned by the expression.

Note: Keep in mind that the string value returned by an XPath
expression is the string value of the first node in the nodeset
returned by the expresssion.

Run this sample from the *SimpleXPathCAPI* subdirectory with:

```sh
SimpleXPathCAPI XMLFile XPathExpression
```

where *XMLFile* is an XML source file, and *XPathExpression* is an
XPath expression to apply to the XML source file. The
*SimpleXPathCAPI* subdirectory contains an XML file named *foo.xml*
similar to the *foo.xml* in the preceding example.

You can try command lines like:

```sh
SimpleXPathCAPI foo.xml /doc/name[3]
```

## StreamTransform

What it does: The `StreamTransform` class processes character input
streams containing a stylesheet and an XML document, and writes the
transformation output to a character output stream. This sample
illustrates the process for working with stylesheets and documents
that you assemble in memory.

You can run it from the *SimpleTransform* subdirectory with `StreamTransform`.

## ThreadSafe

What it does: Multiple threads use a single compiled stylesheet
(`StylesheetRoot`) and DOM source tree (`XalanNode`) to perform
transformations concurrently.  The application tracks the progress of
the threads in messages to the console, and each thread writes its own
output file.  Imagine a server application responding to multiple
clients who happen to request the same transformation.

You can run it from the *ThreadSafe* subdirectory with `ThreadSafe`.

See also:
[Performing a series of transformations](usagepatterns.md#performing-a-series-of-transformations).

## TraceListen

What it does: Trace events during a transformation; the transformation
uses *birds.xsl* to transform *birds.xml* and writes the output to
*birds.out*.

You can run it from the *TraceListen* subdirectory with:

```sh
TraceListen traceFlags
```

where `traceFlags` is one or more of the following:

* *-tt* (Trace the templates as they are being called)
* *-tg* (Trace each result tree generation event)
* *-ts* (Trace each selection event)
* *-ttc* (Trace the template children as they are being processed)

These flags are also available in the
[command-line utility (TestXSLT)](commandline.md).

The core of this example is the following fragment:

```c++
// Set up a diagnostic writer to be used by the TraceListener…
XalanStdOutputStream  theStdErr(cerr);
XalanOutputStreamPrintWriter  diagnosticsWriter(theStdErr);
// Make sure that error reporting, which includes any TraceListener
// output does not throw exceptions when transcoding, since that could
// result in an exception being thrown while another exception is active.
// In particular, characters that the TraceListener writes might not be
// representable in the local code page.
theStdErr.setThrowTranscodeException(false);

// Set up the TraceListener…
// traceTemplates, traceTemplateChildren, traceGenerationEvent,
// and TraceSelectionEvent are booleans set by the command line.
TraceListenerDefault theTraceListener(
        diagnosticsWriter,
        traceTemplates,
        traceTemplateChildren,
        traceGenerationEvent,
        traceSelectionEvent);

// Add the TraceListener to the XSLT processor…
theProcessor.setTraceSelects(traceSelectionEvent);
theProcessor.addTraceListener(&theTraceListener);

// Perform the transformation
…
```

## TransformToXercesDOM

What it does: Performs a simple transformation but puts the result in a
Xerces `DOMDocument`

Run this sample from the *TransformToXercesDOM* subdirectory with:

```sh
TransformToXercesDOM XMLFile XSLFile
```

where *XMLFile* is a source XML file, and *XSLFile* is the XSLT input
file.  The program will use *XSLFile* to transform the input file
*XMLFile* using Xerces DOM as the output destination.

See the `FormatterToXercesDOM` usage in the sample code.

## UseStylesheetParam

What it does: Performs a transformation using top-level stylesheet
parameters.  There are three supported types of parameters.  One is a
text string.  A second is a number of type double.  A nodeset or
parsed document can also be used.

You can run it from the *UseStylesheetParam* subdirectory with:

```sh
UseStylesheetParam xmlfile stylesheet outfile [options]
```

where the options are:

* *-s key "'String-Value'"*
* *-n key Number*
* *-d key "Document-URL"*

The files used by the sample program and the top-level parameter
nodesets for this illustration are to be in working directory in which
the sample program runs.

Using the sample program:

```sh
UseStylesheetParam foo.xml foo.xslt foo.out \
    -s stringA "'This is a test string value'" \
    -n numberA  123.012345 \
    -d parmA "parmA.xml" \
    -d parmB "parmB.xml"
```

The `parmA.xml` and `parmB.xml` files are parsed and converted to
nodesets.  The stylesheet *foo.xslt* merges the contents of *foo.xml*
and the parameters into the *foo.out* file.

The source sample is implemented in C++.  Another example is
implemented in 'C' using the XalanCAPI library, *TestCAPIparm.c*.  The
usage interface for both is the same.

See also:
[Setting stylesheet parameters](usagepatterns.md#setting-stylesheet-parameters).

## XalanTransform

What it does: `XalanTransform` uses the
[`XalanTransformer`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html)
class and the associated C++ API to apply an XSL stylesheet file to an
XML document file and write the transformation output to either an
output file or to a stream.

`XalanTransform` takes command-line arguments for the XML document to
be transformed, the XSL stylesheet to apply, and an optional output
file argument. If you omit the third argument, `XalanTransform` writes
the transformation output to a stream that is sent to standard out (the
console).

You can run `XalanTransform` from the *XalanTransform* subdirectory
with:

```sh
XalanTransform foo.xml foo.xsl foo.out
```

Omit the third argument to write the transformation result to the
console.

See also:
[Using the XalanTransformer class](usagepatterns.md#performing-a-series-of-transformations).

## XalanTransformerCallback

What it does: Return transformation output in blocks to a callback
function, which writes the output to a file.  This sample illustrates
the use of a callback function to incrementally process a
transformation result, that is to begin working with the transformation
result before the transformation has been completed. See
[Processing output incrementally](usagepatterns.md#processing-output-incrementally).

You can run it from the *XalanTransformerCallback* subdirectory with:

```sh
XalanTransformerCallback foo.xml foo.xsl [foo.out]
```

Note: If you omit the third argument, the transformation result is
written to the console.
