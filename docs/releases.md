# Xalan-C++ Release History

# 1.12

Released on the 7th June 2020.

## Major changes

The focus of this release is stability and compatibility.  The majority
of changes are bugfixes or portability improvements.

* Xerces-C++ versions 3.0 and newer are supported. Xerces-C++
  Version 3.2.3 is the preferred stable release.
* Addition of a CMake build for all supported platforms.
* Rewrite of StyleBook XML documentation in Markdown, hosted at
  [https://apache.github.io/xalan-c/](https://apache.github.io/xalan-c/).
* Removal of MacOS and Windows project files for old IDE versions.
* Removal of Autoconf build.
* Xalan-C++ now supports Microsoft Visual Studio 2019 and earlier
  via the new CMake build, which replaces the old version-specific
  project and solution files.
* Xalan-C++ now also supports building with C++11, C++14 and C++17.
* Cygwin and MinGW are known to be nonfunctional in this release.
  Restoring full support will be a goal for 1.13.

## Issues resolved

### Bug

* [XALANC-751](https://issues.apache.org/jira/browse/XALANC-751): Segmentation fault in TestXSLT
* [XALANC-773](https://issues.apache.org/jira/browse/XALANC-773): Build failure with Xerces-C++ 3.2.0 and VS2015
* [XALANC-787](https://issues.apache.org/jira/browse/XALANC-787): Drop MacOS and Windows project files
* [XALANC-788](https://issues.apache.org/jira/browse/XALANC-788): Drop Autoconf build
* [XALANC-789](https://issues.apache.org/jira/browse/XALANC-789): Check CMake build correctly versions libraries on all platforms
* [XALANC-790](https://issues.apache.org/jira/browse/XALANC-790): Ensure CMake is using the library rc files for both libraries
* [XALANC-792](https://issues.apache.org/jira/browse/XALANC-792): Replace StyleBook documentation with a supported equivalent
* [XALANC-793](https://issues.apache.org/jira/browse/XALANC-793): CMake build should install pkg-config file
* [XALANC-794](https://issues.apache.org/jira/browse/XALANC-794): XalanTransform does not transform anything
* [XALANC-795](https://issues.apache.org/jira/browse/XALANC-795): CMake CTest should run all samples as well as unit tests
* [XALANC-797](https://issues.apache.org/jira/browse/XALANC-797): Generate XalanVersion.hpp
* [XALANC-798](https://issues.apache.org/jira/browse/XALANC-798): Remove use of version.incl
* [XALANC-799](https://issues.apache.org/jira/browse/XALANC-799): Remove use of winres.h in XalanMsgLib.rc
* [XALANC-800](https://issues.apache.org/jira/browse/XALANC-800): Add CMake feature tests to replace specific platform definitions
* [XALANC-801](https://issues.apache.org/jira/browse/XALANC-801): Enable AppVeyor CI for Windows
* [XALANC-805](https://issues.apache.org/jira/browse/XALANC-805): Apply outstanding critical fixes
* [XALANC-806](https://issues.apache.org/jira/browse/XALANC-806): C++98 and C++11 support
* [XALANC-807](https://issues.apache.org/jira/browse/XALANC-807): CMake build unconditionally adds debug postfix with MSVC

### Improvement

* [XALANC-776](https://issues.apache.org/jira/browse/XALANC-776): Add CMake build system
* [XALANC-777](https://issues.apache.org/jira/browse/XALANC-777): Remove redundant XALAN_SGI_BASED_STL macro
* [XALANC-778](https://issues.apache.org/jira/browse/XALANC-778): Use anonymous namespace for private time compatibility functions
* [XALANC-779](https://issues.apache.org/jira/browse/XALANC-779): Use xercesc namespace unconditionally
* [XALANC-780](https://issues.apache.org/jira/browse/XALANC-780): Use xalanc namespace unconditionally

### Wish

* [XALANC-771](https://issues.apache.org/jira/browse/XALANC-771):  Support for Parallel Builds

# 1.11

This is primarily a bug-fix release of version 1.10.  Support for
number and nodeset types for stylesheet top-level parameters have been
added. Xerces-C++ versions 3.0 and newer are now
supported. Xerces-C++ Version 3.1.1 is the preferred stable release.

## Major changes

* Add support for Xerces-C 3.0 and newer
* Drop support for Microsoft Studio (VC6)
* Microsoft Visual Studio .NET platforms (2003, 2005, 2008, and 2010)
  are supported.  This equates to compilers (VC71, VC8, VC9, and
  VC10).
* Fix support issues for AIX 5.3
* Fix support issues for Solaris 2.8
* Fix support issues for Solaris 2.10
* Fix support issues for GCC-4.3
* Add support for Microsoft .NET 2008 (VC9)
* Add support for Microsoft .NET 2010 (VC10)
* Add number and nodeset types as top-level stylesheet parameters
* Allow separation of compiler CFLAGS and CXXFLAGS for UNIX builds
* Xalan-C documentation rewrite and reorganize
* Parent Apache XML Project has been retired

## Bug fixes

* [XALANC-734](https://issues.apache.org/jira/browse/XALANC-734): Allow runConfigure CFLAGS and CXXFLAGS to inherit environment
* [XALANC-732](https://issues.apache.org/jira/browse/XALANC-732): Fix makefile errors for AIX builds
* [XALANC-730](https://issues.apache.org/jira/browse/XALANC-730): Fixed XalanTransformer memory leaks
* [XALANC-723](https://issues.apache.org/jira/browse/XALANC-723): Add Top-Level parameter support for XalanCAPI and UseStylesheetParams samples
* [XALANC-723](https://issues.apache.org/jira/browse/XALANC-723): The XalanTransformer Top-Level parameters now include nodeset and number types
* [XALANC-721](https://issues.apache.org/jira/browse/XALANC-721): Upgrade support for MS VS 2010 (VC10)
* [XALANC-719](https://issues.apache.org/jira/browse/XALANC-719): Xerces Parser Liaison - errors on elements with no attributes
* [XALANC-716](https://issues.apache.org/jira/browse/XALANC-716): incorrect buffer-size calculation in XalanUTF16Writer.hpp
* [XALANC-715](https://issues.apache.org/jira/browse/XALANC-715): NULL pointer access crash
* [XALANC-713](https://issues.apache.org/jira/browse/XALANC-713): Build Xalan-C++ API Reference Manual using Doxygen 1.7 or newer
* [XALANC-712](https://issues.apache.org/jira/browse/XALANC-712): Rewrite of Xalan-C++ V1.11 User Guide Web Pages
* [XALANC-710](https://issues.apache.org/jira/browse/XALANC-710): MS-Studio 2008 Debug Build sends Xalan-C_1D.lib to wrong directory
* [XALANC-709](https://issues.apache.org/jira/browse/XALANC-709): Doxygen bug in XalanQName.hpp
* [XALANC-708](https://issues.apache.org/jira/browse/XALANC-708): Including XalanCAPI.hpp does not import Xerces typedef XMLch
* [XALANC-705](https://issues.apache.org/jira/browse/XALANC-705): Crash with pure virtual function call
* [XALANC-703](https://issues.apache.org/jira/browse/XALANC-703): Compiler errors while building on AIX 5.3
* [XALANC-700](https://issues.apache.org/jira/browse/XALANC-700): Broken output when serializing UTF16 surrogates
* [XALANC-699](https://issues.apache.org/jira/browse/XALANC-699): Compile using Visual C++ 2010 (VC10)
* [XALANC-698](https://issues.apache.org/jira/browse/XALANC-698): Buffer overflow from XalanMessageLoader::load()
* [XALANC-690](https://issues.apache.org/jira/browse/XALANC-690): The document function does not handle fragment IDs in URLs correctly
* [XALANC-689](https://issues.apache.org/jira/browse/XALANC-689): XalanC transformation of a 600 kb file takes 43 seconds
* [XALANC-684](https://issues.apache.org/jira/browse/XALANC-684): XPath single quate-comma bug
* [XALANC-683](https://issues.apache.org/jira/browse/XALANC-683): xsl:number with level any does not always count all of the preceeding nodes in the document
* [XALANC-681](https://issues.apache.org/jira/browse/XALANC-681): NamedNodeMapAttributeList constructor asserts when the provided NamedNodeMap is empty
* [XALANC-680](https://issues.apache.org/jira/browse/XALANC-680): Error in match pattern with // and abbreviated attribute node test
* [XALANC-679](https://issues.apache.org/jira/browse/XALANC-679): Possible wrong evaluation of abbreviated XPath in template match
* [XALANC-677](https://issues.apache.org/jira/browse/XALANC-677): Debug assertion switching to HTML mode when the outputter is producing a DOM
* [XALANC-675](https://issues.apache.org/jira/browse/XALANC-675): No error is reported for an unknown key
* [XALANC-675](https://issues.apache.org/jira/browse/XALANC-675): GCC-4.3 cleanup
* [XALANC-673](https://issues.apache.org/jira/browse/XALANC-673): Assertion failed with XalanDOMString::substr
* [XALANC-671](https://issues.apache.org/jira/browse/XALANC-671): Incorrect handling of default namespace in xsl:element
* [XALANC-669](https://issues.apache.org/jira/browse/XALANC-669): XalanOtherEncodingWriter reports an incorrect error message
* [XALANC-667](https://issues.apache.org/jira/browse/XALANC-667): setUseValidation doesn't work
* [XALANC-664](https://issues.apache.org/jira/browse/XALANC-664): Testing harness writes strings to the output XML in the local code page.
* [XALANC-661](https://issues.apache.org/jira/browse/XALANC-661): PlatformSupport/DoubleSupport.cpp compile error on AIX 5.3
* [XALANC-660](https://issues.apache.org/jira/browse/XALANC-660): Compilation errors in Tests/Performance/TestHarness.hpp on some platforms
* [XALANC-659](https://issues.apache.org/jira/browse/XALANC-659): Unnecessary casting in ReusableArenaBlock
* [XALANC-658](https://issues.apache.org/jira/browse/XALANC-658): Misplaced assert in ReusableArenaAllocator::destroyObject()
* [XALANC-657](https://issues.apache.org/jira/browse/XALANC-657): Add Windows x64 targets to the project files
* [XALANC-655](https://issues.apache.org/jira/browse/XALANC-655): Boolean value incorrectly converted ot string in attribute value templates
* [XALANC-653](https://issues.apache.org/jira/browse/XALANC-653): Xalan-C not including namespace declaration in output
* [XALANC-650](https://issues.apache.org/jira/browse/XALANC-650): XPathProcessorImpl uses XalanDOMString copy constructor instead of referring to an existing instance
* [XALANC-648](https://issues.apache.org/jira/browse/XALANC-648): XalanUTF8Writer::write(const XalanDOMChar*, XalanDOMString::size_type) does not handle surrogates properly
* [XALANC-647](https://issues.apache.org/jira/browse/XALANC-647): XalanOtherEncodingWriter::writeNumericCharacterReference() is inefficient
* [XALANC-646](https://issues.apache.org/jira/browse/XALANC-646): During transcoding XalanC fails to append '0' to the transcoded string
* [XALANC-645](https://issues.apache.org/jira/browse/XALANC-645): Pluggin of DOMStringPrintWriter into FormatterToXML leads to assert
* [XALANC-641](https://issues.apache.org/jira/browse/XALANC-641): Class used as exception needs accessible copy constructor
* [XALANC-640](https://issues.apache.org/jira/browse/XALANC-640): Path problem in Makefile under cygwin
* [XALANC-638](https://issues.apache.org/jira/browse/XALANC-638): Performance test program does not build on HP-UX
* [XALANC-635](https://issues.apache.org/jira/browse/XALANC-635): Build fails on Solaris 2.8
* [XALANC-634](https://issues.apache.org/jira/browse/XALANC-634): Build fails with VC6
* [XALANC-633](https://issues.apache.org/jira/browse/XALANC-633): The processor should ignore xsl:fallback element in LRE
* [XALANC-632](https://issues.apache.org/jira/browse/XALANC-632): @use-attribute-sets attribute has to have no effect on xsl:copy when the context item is a documen node.
* [XALANC-631](https://issues.apache.org/jira/browse/XALANC-631): The next stylesheet crashes XalanC
* [XALANC-630](https://issues.apache.org/jira/browse/XALANC-630): Incorrect cast in XalanUTF16Writer
* [XALANC-629](https://issues.apache.org/jira/browse/XALANC-629): Performance improvements for text node stripping
* [XALANC-628](https://issues.apache.org/jira/browse/XALANC-628): Inconsistent flusing of serializers
* [XALANC-627](https://issues.apache.org/jira/browse/XALANC-627): Broken cast in ElemTemplateElement::findTemplateToTransformChild()
* [XALANC-626](https://issues.apache.org/jira/browse/XALANC-626): Fix copyrights in Apache XalanC source
* [XALANC-625](https://issues.apache.org/jira/browse/XALANC-625): Spurious (though harmless) assert failure in XPath::findNodeSet() when an expression contains an empty node-set enclosed in parenthesis
* [XALANC-624](https://issues.apache.org/jira/browse/XALANC-624): The key() function does not work when the context node is the root node of a result tree fragment obtained through exsl:node-set()
* [XALANC-623](https://issues.apache.org/jira/browse/XALANC-623): Compatibility issues with Xerces-C 3.0
* [XALANC-621](https://issues.apache.org/jira/browse/XALANC-621): Selection of compiler definitions file fails on Linux PPC in client code
* [XALANC-618](https://issues.apache.org/jira/browse/XALANC-618): uninstallExternalFunction can run past the end of the array
* [XALANC-617](https://issues.apache.org/jira/browse/XALANC-617): Leak in XalanTransformer
* [XALANC-616](https://issues.apache.org/jira/browse/XALANC-616): xsl:strip-space element doesn't work proprely
* [XALANC-615](https://issues.apache.org/jira/browse/XALANC-615): Xalan resolves a namespace prefix when it should report an error
* [XALANC-614](https://issues.apache.org/jira/browse/XALANC-614): MsgCreator Makefile needs link option
* [XALANC-613](https://issues.apache.org/jira/browse/XALANC-613): Cannot pass value to stylesheet parameter if parameter name is namespace-qualified
* [XALANC-609](https://issues.apache.org/jira/browse/XALANC-609): Template with built-in template rules for namespaces gives and assert
* [XALANC-604](https://issues.apache.org/jira/browse/XALANC-604): ConstructWithNoMemoryManager has a bug which prevents proper default initialization of built-in types.
* [XALANC-603](https://issues.apache.org/jira/browse/XALANC-603): Source does not compile properly on HP-UX with newer versions of the HP compiler
* [XALANC-595](https://issues.apache.org/jira/browse/XALANC-595): EXSLT date-time function missing + or - before timezone offset
* [XALANC-588](https://issues.apache.org/jira/browse/XALANC-588): Floating point exceptions in DoubleSupport::initialize() on Solaris 10 (x86)
* [XALANC-570](https://issues.apache.org/jira/browse/XALANC-570): Removal of compiler warnings from STLHelper.hpp
* [XALANC-535](https://issues.apache.org/jira/browse/XALANC-535): If an error/warning message contains not-displayable character for the local encoding , no message is shown at all
* [XALANC-527](https://issues.apache.org/jira/browse/XALANC-527): Compiling on Xerces-3.0
* [XALANC-480](https://issues.apache.org/jira/browse/XALANC-480): XML1.1 and Namespaces for XML1.1 support
* [XALANC-478](https://issues.apache.org/jira/browse/XALANC-478): Add support for XMLEntityResolver
* [XALANC-432](https://issues.apache.org/jira/browse/XALANC-432): Minor spelling problems in XalanMsg_en_US.xlf
* [XALANC-430](https://issues.apache.org/jira/browse/XALANC-430): "StylesheetExecutionContext::getVariable(name)" crahes if variable "name" was not found.
* [XALANC-421](https://issues.apache.org/jira/browse/XALANC-421): omit-xml-declaration ignored
* [XALANC-416](https://issues.apache.org/jira/browse/XALANC-416): Wrong access to out-of-scope xsl:variable not detected in certain circumstances
* [XALANC-415](https://issues.apache.org/jira/browse/XALANC-415): Encoding attribute is not returning properly in output XML header
* [XALANC-413](https://issues.apache.org/jira/browse/XALANC-413): Crash while using transform method on MacOSX
* [XALANC-412](https://issues.apache.org/jira/browse/XALANC-412): XalanEXSLTDateTime not threadsafe, crashing application
* [XALANC-409](https://issues.apache.org/jira/browse/XALANC-409): compilation error on g++ on SunOS 2.8
* [XALANC-408](https://issues.apache.org/jira/browse/XALANC-408): runConfigure help does not include new HP platform
* [XALANC-407](https://issues.apache.org/jira/browse/XALANC-407): XalanC output format is not suitable as a document entity
* [XALANC-376](https://issues.apache.org/jira/browse/XALANC-376): Result tree fragment attributes are created without proper namespace and local name

# 1.10

## Major changes

* Support for XML 1.1 and Namespaces in XML 1.1.
* Added support for IBM XLC 7.0 compiler on AIX.
* Improved and stabilized C pluggable memory management functionality.
* Various improvements in Xalan XML serializers for better performance
  and stability.
* Upgrade from Xerces-C 2.6 to Xerces-C 2.7.

## Bug fixes

* [XALANC-572](https://issues.apache.org/jira/browse/XALANC-572): Compile failure on SuSE80AMD_64b_GCC322
* [XALANC-569](https://issues.apache.org/jira/browse/XALANC-569): Compile-time assert on 32-bit Solaris and HPUX due to IEEE754 issue
* [XALANC-568](https://issues.apache.org/jira/browse/XALANC-568): Pattern "/" does not match the pseudo root document of a node-set obtained from a result-tree fragment via exsl:node-set
* [XALANC-511](https://issues.apache.org/jira/browse/XALANC-511): Crash during XalanTransformer::initialize on AIX
* [XALANC-554](https://issues.apache.org/jira/browse/XALANC-554): Xalan-C does not implement erratum 25
* [XALANC-552](https://issues.apache.org/jira/browse/XALANC-552): Problems with the new serializers with CDATA and unrepresentable or special characters
* [XALANC-558](https://issues.apache.org/jira/browse/XALANC-558): Latest source code does not build with the Intel compiler on Windows
* [XALANC-555](https://issues.apache.org/jira/browse/XALANC-555): XalanEXSLTDateTime.cpp has too many platform-specifc ifdefs
* [XALANC-421](https://issues.apache.org/jira/browse/XALANC-421): omit-xml-declaration ignored
* [XALANC-529](https://issues.apache.org/jira/browse/XALANC-529): Xalan-C sometimes creates empty text nodes in RTFs or sends empty characters() events to FormatterListeners
* [XALANC-530](https://issues.apache.org/jira/browse/XALANC-530): Serializers filter attributes unnecessarily
* [XALANC-245](https://issues.apache.org/jira/browse/XALANC-245): substring-before and substring-after
* [XALANC-115](https://issues.apache.org/jira/browse/XALANC-115): Need to catch use of variables in xsl:key attributes
* [XALANC-540](https://issues.apache.org/jira/browse/XALANC-540): XPath absolute location path does not work when the context node was obtained from a result tree fragment through exsl:node-set()
* [XALANC-544](https://issues.apache.org/jira/browse/XALANC-544): Assert failure, then crash, during "variable undefined" error recovery in certain circumstances.
* [XALANC-543](https://issues.apache.org/jira/browse/XALANC-543): Crash when calling a template that wrongly uses xsl:with-param instead of xsl:param to declare its parameter.
* [XALANC-539](https://issues.apache.org/jira/browse/XALANC-539): XalanMap needs to compact buckets to prevent long search times and ever-increasing memory usage.
* [XALANC-542](https://issues.apache.org/jira/browse/XALANC-542): XalanDocumentFragmentXNodeSetBaseProxy::dereferenced() is not implemented properly
* [XALANC-538](https://issues.apache.org/jira/browse/XALANC-538): Better handling for non-existent node test
* [XALANC-541](https://issues.apache.org/jira/browse/XALANC-541): Fixes for zOS build
* [XALANC-455](https://issues.apache.org/jira/browse/XALANC-455): Problem when releasing InputSource obtained through an EntityResolver
* [XALANC-535](https://issues.apache.org/jira/browse/XALANC-535): If an error/warning message contains not-displayable character for the local encoding , no message is shown at all
* [XALANC-532](https://issues.apache.org/jira/browse/XALANC-532): FormatterToXMLUnicode does not serialize UTF-8
* [XALANC-536](https://issues.apache.org/jira/browse/XALANC-536): ElemNumber uses extra temporary strings
* [XALANC-537](https://issues.apache.org/jira/browse/XALANC-537): XalanC source code quotes incorrectly license protected documents
* [XALANC-526](https://issues.apache.org/jira/browse/XALANC-526): Use of undefined stricmp breaks compilation on Linux
* [XALANC-513](https://issues.apache.org/jira/browse/XALANC-513): ICUFormatNumberFunctor has some MemoryManager issues
* [XALANC-507](https://issues.apache.org/jira/browse/XALANC-507): Safety improvements for XalanVector::insert()
* [XALANC-506](https://issues.apache.org/jira/browse/XALANC-506): Remove references to stlport directory in Makefile.incl.in
* [XALANC-505](https://issues.apache.org/jira/browse/XALANC-505): DirectoryEnumerator.hpp will not build on Solaris 10 with Sun Workshop 10
* [XALANC-502](https://issues.apache.org/jira/browse/XALANC-502): XalanDOMStringCache has initializer list out of order
* [XALANC-501](https://issues.apache.org/jira/browse/XALANC-501): Creation of comments and PIs needs to detect illegal values
* [XALANC-500](https://issues.apache.org/jira/browse/XALANC-500): Remove old "diagnostic" output from StylesheetRoot and XSLTEngineImpl
* [XALANC-497](https://issues.apache.org/jira/browse/XALANC-497): Local variables or parameters in EXSLT-functions are interpreted as global
* [XALANC-496](https://issues.apache.org/jira/browse/XALANC-496): Glitches in header files
* [XALANC-493](https://issues.apache.org/jira/browse/XALANC-493): Problem with XALAN_STATIC_CHECK on HP-UX IA64
* [XALANC-492](https://issues.apache.org/jira/browse/XALANC-492): Various pluggable memory management bugs.
* [XALANC-490](https://issues.apache.org/jira/browse/XALANC-490): XalanFileReporter is inconsistent in its usage of pluggable memory management
* [XALANC-489](https://issues.apache.org/jira/browse/XALANC-489): XalanMemMgrs has a thread-safety issue
* [XALANC-488](https://issues.apache.org/jira/browse/XALANC-488): XSLTInputSource does not support pluggable memory management
* [XALANC-487](https://issues.apache.org/jira/browse/XALANC-487): URISupport does not provide explicit MemoryManager
* [XALANC-485](https://issues.apache.org/jira/browse/XALANC-485): GCC warns about order of members in initializer list
* [XALANC-484](https://issues.apache.org/jira/browse/XALANC-484): Remove compiler warnings about placement delete for HP's aCC compiler
* [XALANC-483](https://issues.apache.org/jira/browse/XALANC-483): XercesParserLiaison does not create Xerces-C DOM instances with namespaces support enabled.
* [XALANC-482](https://issues.apache.org/jira/browse/XALANC-482): Remove compiler warnings
* [XALANC-470](https://issues.apache.org/jira/browse/XALANC-470): runConfigure does not pass link options to Makefile for AIX platform
* [XALANC-465](https://issues.apache.org/jira/browse/XALANC-465): Add alt tag to footer.html page under html/apiDocs tree.
* [XALANC-463](https://issues.apache.org/jira/browse/XALANC-463): Transforming an XML DOM document encapsulated within an XercesDOMWrapperParsedSource object triggers an invalid heap pointer exception if XSL opens other XML document with 'document()' function.
* [XALANC-460](https://issues.apache.org/jira/browse/XALANC-460): Linux/xlC port
* [XALANC-448](https://issues.apache.org/jira/browse/XALANC-448): TestXSLT/process.cpp compilation fails, interface broken in FormatterToText.hpp
* [XALANC-445](https://issues.apache.org/jira/browse/XALANC-445): generate-id() inserts periods into name which limits usage

# 1.9

## Major changes

* Pluggable Memory Management.  This feature allows applications to
  maintain precise control over memory allocations.  This will allow
  users, with knowledge of their application's characteristics, to
  provide a more efficient allocation method than the general purpose
  procedures provided by the system.
* Improved stability on the execution of large stylesheets and XML
  documents.  Because the current implementation relies on function
  recursion, certain stylesheets/input documents may cause the program
  stack to overflow and cause the application to fail.  It also may
  inhibit executing stylesheets that are heavily dependent on
  recursive functions.  This feature will prevent these types of
  failure.
* Provided a global option to pool all text node strings.  This
  feature allows developers to set a flag to determine if a parsed
  source tree will pool the values of all of the text nodes in the XML
  document.  By doing so, this can result in a significant reduction
  in memory usage, particularly if there are a lot of repeated values
  in text nodes.  Please see the class descriptions for
  `XalanSourceTreeDocument`, `XalanDefaultParsedSource`, and
  `XalanTransformer` in the API reference..
* Added support for Microsoft Studio .NET V7.1.  Project files for
  Microsoft Studio .NET V7.1 have been provided under the directory
  xalan\c\Projects\Win32\VC7.1.
* Upgrade from Xerces-C 2.5 to Xerces-C 2.6.

# 1.8

## Major changes

* Upgrade from Xerces-C 2.4 to Xerces-C 2.5.
* Provide build support for Cygwin and HP-UX 11.00 on Itanium.
  Although binary distributions are not provided for the Cygwin
  environment and HP-UX 11.00 on Itanium platform, the build
  configuration files provided in the source distribution can be used
  to build on these platforms.
* Project files for supporting Microsoft Visual Studio .NET.  Although
  a binary distribution for the Microsoft Visual Studio .NET is not
  provided in this release, Visual Studio solution files are provided
  in the Windows source distribution for those who choose to build on
  this platform.
* Additional changes to Xerces Deprecated DOM support.  This is
  another step towards phasing out the support for Xerces Deprecated
  DOM.  Users are encouraged to modify their applications to interface
  with the new Xerces DOM support.
* Implemented optimizations to significantly improve the throughput of
  the ICU-enabled "format-number" function.
* Changes in the XML Serializer. The serializer will no longer put a
  newline after the xml header tag unless indent="yes". See bugzilla
  [24304](http://nagoya.apache.org/bugzilla/show_bug.cgi?id=24304) and
  [28386](http://nagoya.apache.org/bugzilla/show_bug.cgi?id=28386).

# 1.7

## Major changes

* Message Localization Support. This feature enables Xalan-C++ to
  issue diagnostic messages in languages other than the default
  language (English). In order to utilize this feature, the user must
  provide a version of the Xalan-C++ message catalog that has been
  translated in their language of choice.
* Build, packaging, and installation improvements. Changes to the
  Xalan-C++ makefiles now allow for a more flexible build, packaging,
  and install process. Each step in a typical build process can be
  invoked by specifying the appropriate target, for example to build
  the executable, and/or samples, or install the build at
  user-specified location.
* Deprecated DOM Support. The `XercesBridge` classes have been marked
  deprecated and moved into an exclusive directory within the
  `xalanc/XercesParserLiaison` directory called `Deprecated`. If your
  application is dependent on these classes to interface with the
  deprecated Xerces DOM you must alter your include directive with the
  `Deprecated` directory.  For example: `#include
  <xalanc/XercesParserLiaison/XercesDocumentBridge.hpp>` should be
  changed to: `#include
  <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>`.
* Upgrade from Xerces-C 2.3 to Xerces-C 2.4.
* Porting of `ThreadSafe` example to UNIX platform.
* Implementation of EXSLT string extension functions `encode-uri()`
  and `decode-uri()`.
* Implementation of EXSLT dynamic extension function `evaluate()`.
* Implementation of EXSLT date-time extension function `date-time()`.

# 1.6

## Major changes

Directory "sane include" reorganization.  A new sub-directory called
`xalanc` has been added to the `src` directory (for source
distribution packages) and `include` directory (for binary
distribution packages).  New source files which include Xalan headers
will need to be modified to include the `xalanc` prefix in the header
path.  For example:

```
#include <XPath/XObject.hpp>
```

will need to be changed to

```
#include <xalanc/XPath/XObject.hpp>
```

For Windows users, all project files have been modified to reflect
this change.

# 1.5

## Major changes

* For Windows, Xalan-C++ is now packaged as a single DLL.  The project
  files to build separate DLLs are now deprecated and will not be
  maintained.
* Support for Xerces-C++ `DOMNode` classes.
* New sample `TransformToXercesDOM` that illustrates how to perform a
  transform resulting in a Xerces-C++ DOMDocument
* Usage of the Xerces-C++ C++ namespace and creation of Xalan-C++
  namespace.

# 1.4

## Major changes

* The Xalan-C++ distributions no longer include the Xerces-C++ headers
  and libraries.  Accordingly, you must download the appropriate
  Xerces-C++ distribution.
* 64-bit build for HP-11; untested 64-bit support for Solaris and AIX.
* New Linux platform and compiler: Redhat 7.2 with gcc 3.1.
* STLPort is no longer required on any of our supported platforms.
* Alpha support for a number of EXSLT functions.
* `ParsedSourceWrappers`, a sample that illustrates how to perform
  transformations with input in the form of a pre-built `XalanDOM` or
  `XalanSourceTree`.

# 1.3

## Major changes

* A new and simplified command-line utility: the Xalan executable
* New XPath samples exploiting the XPathEvaluator C API and
  illustrating serialization of an XPath result set: `SimpleXPathCAPI`
  and `SerializeNodeSet`
* A new wrapper class that lets you pass in a Xerces DOM as input for
  a transformation
* Bug fixes.
* Changed `XPathException` to `XalanXPathException` because of a clash
  with a new Xerces class of the same name.

# 1.2

## Major changes

* Introduction of a library of extension functions
* Support for pre-parsed source documents and pre-compiled stylesheets
  in `XalanTransformer` and the C API.
* Support for stylesheet params in `XalanTransformer` and the C API
* Support for extension functions in `XalanTransformer`
* Performance enhancements

# 1.1

## Major changes

In addition to adding to the list of platforms on which Xalan-C++
builds, our focus for this release has been on bug fixes, performance
enhancements, and providing a simplified C++ and C API for performing
standard transformations. Major updates include:

* Added HP-UX 11 build.
* Added Solaris build.
* Greatly improved performance.
* Improved conformance to the XSLT specification.
* Simplified the C++ API and provided a C API for performing standard
  transformations with the `XalanTransformer` class and associated
  header files.
* Added sample illustrating use of XalanTransformer and the new C++
  API: `XalanTransform`.
* Added sample illustrating use of XalanTransformer, the new C API,
  and how to run Xalan-C++ and perform transformations on an Apache
  Web server: `ApacheModuleXSLT`.
* Added the `StreamTransform` sample.
* Eliminated dependencies on non-public Xerces headers; accordingly
  can now build with the Xerces distribution.
* Fixed namespace mapping and inheritance problems.
* Fixed failure of `<copy-of …>` to include CRLFs (if any) in the
  result.
* Fixed bug sorting in reverse document order based on position.
* Fixed `<xsl:number>` bug with use of letter-value="traditional" for
  "classical" Greek numbering.
* Fixed use of `<xsl:fallback>` within a top-level extension function.
* Fixed HTML output method bug. The `<HEAD>` element was not
  including a `<META>` tag with the encoding.
* Fixed bug using `key()` in match patterns to find descendants.
* Fixed bug using the `id()` function in complex match patterns.
* Fixed problem outputting multiple single-character CDATA sections.
* Fixed problem outputting whitespace characters.
* Fixed problem transforming from a stream on UNIX platforms.
* Fixed a bug with the `document()` function, which was generating a
  syntax error if called in the following manner:
  `document(string,/)`.
* Fixed named templates bug accepting global parameter when called
  with a parameter of the same name that the named template does not
  recognize.

# 1.0

## Major changes

* Full support for namespace handling
* Full implementation of the `format-number()` function and support for
  the decimal-format element
* Integration with the ICU for number formatting, sorting, and output
  encoding
* Support for the `exclude-result-prefixes` attribute
* Support for the output `encoding` attribute

# 0.40.0

## Major changes

* Permitting OS file names (as well as URLs) as command-line arguments
  with TestXSLT
* Stricter compliance for HTML and XML output
* Improved handling of relative URI's
* Improved handling of Import and Include
* Better namespace handling (although there are still problems here)
* Support for `Document()` function
* Support for XPath predicates
* ProblemListener with a different output stream and call
  `XalanTransformer::setProblemListener()` before calling
  `XalanTransformer::transform()`.
