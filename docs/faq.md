# Frequently asked questions

## Where do I go to learn about XSLT

*Where do I go to learn about XSLT?*

The definitive sources are the W3C XSLT and XPath recommendations:
[W3C Recommendation 16 November 1999 XSL Transformations (XSLT) Version 1.0](http://www.w3.org/TR/xslt)
and
[XML Path Language (XPath) Version 1.0](http://www.w3.org/TR/xpath).

For a brief listing of tutorials, discussion forums, and other
materials, see
[Getting up to speed with XSLT](overview.md#getting-up-to-speed-with-xslt).


## Asking questions about Xalan-C++

*Where can I ask a question?*

For specific questions on Xalan-C++, see list archives:
[xalan-c-users](http://marc.info/?l=xalan-c-users) and
[xalan-dev](http://marc.info/?l=xalan-dev).
You must subscribe to these Apache mailing lists before posting your
questions.

The Apache Software Foundation has information on how you can subscribe
to the
[mailing lists](http://www.apache.org/foundation/mailinglists.html).

You can post messages to the lists by sending mail to:

* `c-users@xalan.apache.org` (user's list)
* `dev@xalan.apache.org` (developer's list)

Again, please review the archives before posting a new question.


## What is Xerces-C++?

*What is Xerces-C++ and why do I need it?*

Xerces-C++ is a validating XML parser written in a portable subset of
C++.  Xerces-C++ makes it easy to give your application the ability to
read and write XML data.  Like Xalan-C++, Xerces-C++ is available from
the Apache XML site:
[http://xerces.apache.org](http://xerces.apache.org).

## Which version of Xerces-C++ should I be using?

*Which version of Xerces-C++ should I be using?*

The Xalan-C++ release notes includes information about the Xerces-C++
release with which the Xalan-C++ release has been coordinated and
tested. See the [release history](releases.md).

## Should I be using the Xerces DOM or Xalan DOM?

*Should I be using the Xerces DOM or Xalan DOM?*

The Xalan DOM implementation is highly optimised for transformations.
However, whilst you can build documents in the Xalan DOM, subsequent
modification will not work.  The Xalan DOM is designed to be either an
input or an output from a transformation, not as a general DOM
implementation.

So in cases where you want to simply transform documents using Xalan,
using the internal DOM implementation is the best approach.

In cases where you want to modify the DOM document on the fly, you
should use the Xerces DOM as the base document.  You can wrap the
Xerces DOM in a wrapper (see
[Passing in a Xerces DOM](usagepatterns.md#passing-in-a-xerces-dom-to-a-transformation))
to then use as an input to a Xalan transformation.  Alternatively you
can output the result of a transformation to a Xerces DOM document (see
[Working with DOM input and output](usagepatterns.md#working-with-dom-input-and-output)).  In either
case, the Xerces document can be freely modified.  However, after you
modify the document, you need to re-build the wrapper so that any
changes are replicated in the Xalan wrappers.

## Problems with samples in Windows

*I have encountered problem executing the Xalan-C++ sample applications
after rebuilding them under Win32 Environment (Windows NT 4.0, SP3).
When I tried to execute the sample, I receive the error message
`Debug Assertion Failed! …  Expression: _BLOCK_TYPE_IS_VALID(pHead->nBlockUse)`.*

You may be mixing debug and release versions of executables and
libraries.  In other words, if you are compiling the sample for debug,
then you should link with the debug version of the Xalan-C++ and
Xerces-C++ libraries and run with the debug version of the dynamic link
libraries.

You must also make sure your application is linking with the "Debug
Multithreaded DLL" run-time library or the "Multithreaded DLL" run-time
library.  To check this setting do the following in Visual C++:

1. Select "Settings" from the "Project" menu.
2. Click the "C/C++" tab.
3. In the Category drop-down list, select "Code Generation".
4. In the "Use run-time library" drop-down list, select "Multithreaded
   DLL" for the "Win32 Release" configuration, or select "Debug
   Multithreaded DLL" for the "Win32 Debug" configuration.

Once you have changed this setting, you must rebuild your project.

Note: This FAQ entry is largely historical.  While mixing Release and
Debug builds and different runtimes is still inadvisable, the CMake
build system should link the correct versions of the libraries and
prevent this happening by accident.

## Building on Windows

*What do I need to rebuild Xalan-C++ on Windows?*

For more details, see
[Downloading Xalan-C++](download.md) and
[Building Xalan-C++](build.md).

## Building on UNIX

*What do I need to rebuild Xalan-C++ on UNIX?*

For more details, see
[Downloading Xalan-C++](download.md) and
[Building Xalan-C++](build.md).

## What is ICU

*What is ICU and why do I need it?*

The
[International Components for Unicode (ICU)](http://icu-project.org/)
is a C and C++ library that provides robust and full-featured Unicode
support on a wide variety of platforms.  Xalan-C++ uses the ICU to
extend support for encoding, number formatting, and sorting.

Xalan should work with any release of ICU from the past decade.

For more details see
[Using the International Components for Unicode (ICU)](usagepatterns.md#using-the-icu).

## A tar checksum error on Solaris

*I am getting a tar checksum error on Solaris. What's the problem?*

The Solaris tar utility you are using does not properly handle files
with long pathnames.  You must use GNU tar (gtar), which handles
arbitrarily long pathnames and is freely available on every platform on
which Xalan-C++ is supported.  If you don't already have GNU tar
installed on your system, you can obtain it from the Free Software
Foundation
[http://www.gnu.org/software/tar/tar.html](http://www.gnu.org/software/tar/tar.html).
For additional background information on this problem, see the online
manual
[GNU tar and POSIX tar](http://www.gnu.org/manual/tar/html_chapter/tar_8.html#SEC112)
for the utility.

## Xalan-C++ in Apache

*Is it possible to run Xalan-C++ from an Apache server?*

A simple Apache module called
[ApacheModuleXSLT](samples.md#apachemodulexslt)
is provided as a sample.  It demonstrates how to integrate Xalan-C++
with Apache.

## Is Xalan-C++ thread-safe?

*Is Xalan-C++ thread-safe?*

Instances of
[`XalanTransformer`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html)
are not thread-safe; each thread should use its own instance.

In order to support very efficient use in multi-threaded applications,
Xalan-C++ is designed to avoid synchronization as much as possible.
Each thread of execution is required to have its own set of "support"
objects that contain the state of the transformation.  Accordingly, no
synchronization is required when multiple threads are executing.

Parsed ("compiled") stylesheets (see
[Performing a series of transformations](usagepatterns.md#performing-a-series-of-transformations))
and parsed source documents may be freely shared by multiple threads of
execution without worrying about providing synchronized access to them.
The *only* exception to this rule: You use
[`XercesParserLiaison`](https://apache.github.io/xalan-c/api/classxalanc_1_1XercesParserLiaison.html)
to parse a document after calling
[`XercesParserLiaison::setBuildBridgeNodes(false)`](https://apache.github.io/xalan-c/api/classxalanc_1_1XercesParserLiaison.html#a01a4d6c3abaeb09738d555814ef7194c)
or
[`XercesParserLiaison::setThreadSafe(false)`](https://apache.github.io/xalan-c/api/classxalanc_1_1XercesParserLiaison.html#af8dc3b5cfae5dd9a1cbdf446e369b9c9).
In this case, the document *cannot* be shared by multiple threads of
execution.  For reasons of performance, we do not recommend the use of
`XercesParserLiaison`, so this should not be an issue for most
applications.

All other objects in Xalan-C++ are *not* thread-safe. Each thread must
have its own instance of each object.

See the [ThreadSafe](samples.md#threadsafe) sample program for more
information.

## What can I do to speed up transformations?

*What can I do to speed up transformations?*

To maximize performance, here are some suggestions for you to keep in
mind as you set up your applications:

* Use a compiled stylesheet when you expect to use the stylesheet more
  than once.
* Set up your stylesheets to function efficiently.
  * Don't use `//` (descendant axes) patterns near the root of a large
    document.
  * Use `xsl:key` elements and the `key()` function as an efficient way
    to retrieve node sets.
  * Where possible, use pattern matching rather than `xsl:if` or
    `xsl:when` statements.
  * `xsl:for-each` is fast because it does not require pattern
    matching.
  * Keep in mind that `xsl:sort` prevents incremental processing.
  * When you create variables,
    `<xsl:variable name="fooElem" select="foo"/>` is usually faster
    than
    `<xsl:variable name="fooElem"><xsl:value-of-select="foo"/></xsl:variable>`.
   * Be careful using the `last()` function.
   * The use of index predicates within match patterns can be
     expensive.

## Stylesheet validation

*Can I validate an XSL stylesheet?*

An XSL stylesheet is an XML document, so it can have a `DOCTYPE` and be
subject to validation, but you probably will have to write a custom DTD
for the purpose.

The XSLT Recommendation includes a
[DTD Fragment for XSL Stylesheets](http://www.w3.org/TR/xslt#dtd)
with some indications of what you need to do to create a complete DTD
for a given stylesheet.  Keep in mind that stylesheets can include
literal result elements and produce output that is not valid XML.

You can use the `xsl:stylesheet` doctype defined in `xsl-html40s.dtd`
for stylesheets that generate HTML.

## What does the XalanDOMException HIERARCHY_REQUEST_ERR mean?

*What does the
[`XalanDOMException`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanDOMException.html)
[`HIERARCHY_REQUEST_ERR`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanDOMException.html#a7ace065fde4b86526306d2c9ca3ab074a1333e25db8de187d2caee6513cc9dcc4)
mean?*

It means that an attempt was made to add a node to a DOM that would
create an invalid structure.  For example, text nodes are not allowed
as children of the document node.

This is a common error when attempting to transform to DOM.  Source
documents and stylesheets that might produce valid serialized XML
might not produce a valid DOM.  The usual suspect is text nodes being
generated before the document element is generated.

If you think you have seen this error because of a bug in Xalan-C++'s
source tree implementation, please post a bug report on
[Jira](https://issues.apache.org/jira/browse/XALANC), and
attach a minimal source document and stylesheet that produce the
problem to the bug report.

## Submitting Patches

*Who do I submit patches to?*

Your contributions are much appreciated! You can e-mail your patches to
the Xalan Development Mailing List or raise an issue on the
[Jira issue tracking system](https://issues.apache.org/jira/browse/XALANC).

The Xalan projects use Jira as the issue tracking system.  Any
significant bug or feature request is posted to this system.  You must
subscribe to the system in order to submit patches and raise issues.

* Subscribe to Jira at: [https://issues.apache.org/jira](https://issues.apache.org/jira)
* Browse the issues at: [https://issues.apache.org/jira/browse/XALANC](https://issues.apache.org/jira/browse/XALANC)

Issues posted to the project on Jira at `XALANC` are automatically
posted to the Xalan Development Mailing List.

Our mailing lists are moderated.  You should subscribe to the mailing
list in order to post a message, otherwise message delivery requires
manual intervention or may be dropped.

## Transformation Output Methods

*How do I output a transformation to a DOM, a file, an in-memory
buffer, or as input to another transformation?*

Since the C++ language can automatically construct an
[`XSLTResultTarget`](https://apache.github.io/xalan-c/api/classxalanc_1_1XSLTResultTarget.html)
from any of its constructor's argument types, you usually don't need to
create one explicitly.

The output parameter of
[`XalanTransformer::transform()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a3162d8129224a1a6ed67d8752f3447b4)
is an
[`XSLTResultTarget`](https://apache.github.io/xalan-c/api/classxalanc_1_1XSLTResultTarget.html)
which has many constructors.

Output to a file:

* The easiest way is to use a null-terminated string containing the
  file name to create an `XSLTResultTarget`.  Or, use an instance of
  `std::ofstream`.  The command line executables, and many of the
  sample applications use file names, so take a look through the
  source code for more information.

Output to an in-memory buffer:

* Use an instance of `std::ostrstream` or `std::ostringstream` to
  create an XSLTResultTarget.  See the StreamTransform sample for
  more information.

Input to another transformation:

* Any of the previous output targets could be used as the input to
  another transformation, but the
  [`FormatterToSourceTree`](https://apache.github.io/xalan-c/api/classxalanc_1_1FormatterToSourceTree.html)
  is probably the best for efficiency reasons.  See the source code for
  the `TestXSLT` command line program for more information.

## Problems Using Sun's Forte/Workshop Compiler with code containing std::istrstream

*Why won't XSLTInputSource work with `std::istrstream` on Sun Solaris
using Forte/Sun Workshop compiler?*

There is a bug in Sun's C++ standard library implementation for the
Forte/Workshop compiler.  The short answer is that you need to get a
patch.

The issue is resolved if you use the SunStudio platform for your code
development.  The Solaris SunStudio is now available from Oracle.

## Modifying an instance of XalanDocument

*My transformation outputs to a
[`XalanDocument`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanDocument.html)
(actually
[`XalanSourceTreeDocument`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanSourceTreeDocument.html)
underneath) but W3C DOM functions like
`DOMElement::setAttribute` don't work! Am I going crazy or what?*

No, you aren't going crazy.  Xalan's default source tree is read-only
for efficiency. If you need a DOM that supports modifications, use the
Xerces DOM instead.  See the
[TransformToXercesDOM](samples.md#transformtoxercesdom) sample for more
information.

## Changing Where Error Output is Sent

*XalanTransformer outputs errors to the console. How do I, for example,
output error to a file?*

By default,
[`XalanTransformer`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html)
creates a
[`XalanTransformerProblemListener`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformerProblemListener.html)
(a subclass of
[`ProblemListener`](https://apache.github.io/xalan-c/api/classxalanc_1_1ProblemListener.html))
that writes output to `std::cerr`. To change this you can:

* Redirect `std::cerr` from the command line.
* Call
  [`XalanTranformer::setWarningStream`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a1c7826710c68a6311329ef7be65253e6)
  with a different `std::ostream` before calling
  [`XalanTransformer::transform`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a3162d8129224a1a6ed67d8752f3447b4).
* Instantiate your own
  [`XalanTransformerProblemListener`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformerProblemListener.html)
  with a different output stream and call
  [`XalanTransformer::setProblemListener()`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a3dda165f568b89e02bcbf2f56508ec7a)
  before calling
  [`XalanTransformer::transform`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#a3162d8129224a1a6ed67d8752f3447b4).
* Subclass some
  [`ProblemListener`](https://apache.github.io/xalan-c/api/classxalanc_1_1ProblemListener.html)
  type and do custom handling of errors (you still then need to tell
  [`XalanTransformer`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html)
  instances to use your `ProblemListener`).

In most cases you probably want to do one of the first two.

## Programmatic Error Information

*How do I programmatically get the file name, line number, and column
number for an error in an XML file?*

Create a custom `ErrorHandler` (a Xerces-C++ class) and call
[`XalanTransformer::setErrorHandler`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanTransformer.html#af3eff73a57f2998a2b440c2c4f105371)
before parsing any sources.

## String Transcoding

*How do I make a `char*` out of
[`XalanDOMString`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanDOMString.html)
(or vice-versa)?*

See the static method
[`XalanDOMString::transcode`](https://apache.github.io/xalan-c/api/classxalanc_1_1XalanDOMString.html#a0b7ab1c0f2ea4615962437b8837e11d7),
or the functions
[`TranscodeToLocalCodePage`](https://apache.github.io/xalan-c/api/namespacexalanc.html#ada7c55659cbbc92271ac1fa128afa07b).
However, you should be very careful when transcoding Unicode characters
to the local code page, because not all Unicode characters can be
represented.

## Error Code/Exception Summary

*Is there a table of error codes somewhere? How about a summary of what
methods throw which exceptions?*

There isn't, but we're working on it.

## Extension Functions

*The Xalan extension functions (`xalan:node-set()`, etc.) don't work for
me. Help!*

Did you declare the namespace on the `xsl:stylesheet` or
`xsl:transform` element?  It should look like this:

```xml
   <xsl:stylesheet version="1.0" xmlns:xalan="http://xml.apache.org/xalan"> …rest of stylesheet
```

If you did and you still have problems, you might want to ask the
mailing list.

## Using format-number and ICU

*Why does Xalan emit a warning when using the XSLT function
`format-number()`?*

Did you build with ICU support? See
[Using the International Components for Unicode (ICU)](usagepatterns.md#using-the-icu).

## Perl wrapper for Xalan-C++?

*Is there a Perl wrapper for Xalan-C++?*

There is no Apache Perl wrapper, however Edwin Pratomo has written a
wrapper for Xalan-C++ version 1.4 that can be found on CPAN.

## Missing *LocalMsgIndex.hpp* file

*Why can't I find the *LocalMsgIndex.hpp* file?*

The *LocalMsgIndex.hpp* file is not shipped with the source
distributions because this file is generated during the build process,
customized for the locale and message set you are using.

The *LocalMsgIndex.hpp* file is a member of the
*include/xalanc/PlatformSupport* directory.
