# Xalan-C++ Overview

## Introduction

Xalan-C++ (named after a rare musical instrument) implements the
[W3C Recommendation 16 November 1999 XSL Transformations (XSLT) Version 1.0](http://www.w3.org/TR/xslt)
and the
[XML Path Language (XPath) Version 1.0](http://www.w3.org/TR/xpath).
XSLT is the first part of the XSL stylesheet language for XML. It includes the XSL
Transformation vocabulary and XPath, a language for addressing parts of XML documents. For links to background materials,
discussion groups, frequently asked questions, and tutorials on XSLT, see
[Getting up to speed with XSLT](#getting-up-to-speed-with-xslt).

Note: XSL also includes a vocabulary for formatting documents, which is
not part of Xalan-C++.  For more information, see
[Extensible Stylesheet Language (XSL) Version 1.0 W3C Recommendation](http://www.w3.org/TR/xsl)
and the [Apache XML FOP (Formatting Objects Project)](http://xmlgraphics.apache.org/fop).

You use the XSLT language to compose XSL stylesheets. An XSL stylesheet
contains instructions for transforming XML documents from one document
type to another document type (XML, HTML, or other). In structural
terms, an XSL stylesheet specifies the transformation of one tree of
nodes (the XML input) into another tree of nodes (the output or
transformation result).

Note: The XSL stylesheet may generate and refer to cascading style
sheets ([CSS](http://www.w3.org/Style/CSS/)) as part of its output.

In the following example, the *foo.xsl* stylesheet is used to transform
*foo.xml* into *foo.out*:

*foo.xml*:

```xml
<?xml version="1.0"?>
<doc>Hello</doc>
```

*foo.xsl*:

```xml
<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:template match="doc">
<out><xsl:value-of select="."/></out>
</xsl:template>
</xsl:stylesheet>
```

*foo.out*:

```xml
<out>Hello</out>
```

## Xalan-C++ Features

* Xalan-C++ fully implements the
  [W3C Recommendation 16 November 1999 XSL Transformations (XSLT) Version 1.0](http://www.w3.org/TR/xslt).
* Xalan-C++ incorporates the
  [XML Path Language (XPath) Version 1.0](http://www.w3.org/TR/xpath).
* Xalan-C++ uses [Xerces-C++](http://xerces.apache.org/xerces-c/index.html)
  to parse XML documents and XSL stylesheets.  The input may appear in
  the form of a file or URL, a stream, or a [DOM](http://www.w3.org/DOM).
  Xalan-C++ performs the transformations specified in the XSL
  stylesheet and produces a file, a stream, or a DOM as you
  specify when you set up the transformation.
* Along with a complete API for performing transformations in your C++
  applications, Xalan-C++ provides a [commandline](commandline.md)
  utility for convenient file-to-file transformations.
* Xalan-C++ supports C++ [extension functions](extensions.md).

## Getting to work with Xalan-C++

For instructions and some suggestions about how to get started using
Xalan-C++, see [Building Xalan-C++](build.md) and
[Installing Xalan-C++](install.md).

## Getting up to speed with XSLT

If you are still working through the details of the XSLT spec (the W3C
1.0 Recommendation), you may want to consult one or more of the
following:

* Crane Softwright's
  [Free preview of Practical Transformation Using XSLT and XPath](http://www.CraneSoftwrights.com/training/)
* Doug Tidwell's [XSLT](http://www.oreilly.com/catalog/xslt/),
  O'Reilly, 2001
* Bob DuCharme's [XSLT Quickly](http://www.manning.com/ducharme/index.html),
  Manning Publications, 2001
* John Robert Gardner and Zarella Rendon's
  [XSLT and XPath: A Guide to Transformations](http://vig.prenhall.com/catalog/academic/product/1,4096,0130404462,00.html),
  Prentice-Hall, 2001
* Elliotte Rusty Harold's
  [Chapter 17 of the XML Bible: XSL Transformations](http://www.ibiblio.org/xml/books/bible2/chapters/ch17.html)
* The Mulberry
  [XSL-List -- Open Forum on XSL](http://www.mulberrytech.com/xsl/xsl-list/)
  (of interest to XSL users at all levels)
* Objects by Design's
  [Transforming XMI to HTML](http://www.objectsbydesign.com/projects/xmi_to_html.html)
  (oriented towards XMI, "an XML-based, stream representation of a UML
  model," but also covers "generic" XML transformations) and their
  related
  [XSLT by Example](http://objectsbydesign.com/projects/xslt/xslt_by_example.html)
* OASIS (the Organization for the Advancement of Structured Information Standards):
  [Extensible Stylesheet Language (XSL)](http://www.oasis-open.org/cover/xsl.html)
  by Robin Cover
* Aaron Skonnard and Martin Gudgin's
  [Essential
   XML Quick Reference](http://www.theserverside.net/tt/books/addisonwesley/EssentialXML/index.tss"):
   A Programmer's Reference to XML, XPath, XSLT, XML Schema, SOAP and
   More. This reference is published by Addison Wesley, 2003, ISBN/0201740958.

When you come across other useful introductory or background materials,
please email the Xalan Development Mailing List, so we can add them to
this list.

## Glossary
<dl>
  <dt><strong>XSLT Namespace</strong></dt>
  <dd>The
      <a href="http://www.w3.org/TR/REC-xml-names/">XML namespace</a>
      for XSLT.  An XML namespace is a collection of element and
      attribute names, identified by a Unique Resource Identifier
      (URI), which often takes the form of a URL, but is really just a
      unique string, not a pointer to a web page. The XSLT namespace
      URI is `http://www.w3.org/1999/XSL/Transform`. In each XSLT
      stylesheet, you must declare this namespace in the stylesheet
      element tag and bind it to a local prefix. Like the XSLT
      specification, we always use xsl as the XSLT namespace prefix in
      our descriptions and examples, although you are free to bind any
      prefix to this namespace.</dd>
 <br>
 <dt><strong>XSL Instruction</strong></dt>
 <dd>Any tag associated with the XSLT namespace.</dd>
 <br>
 <dt><strong>Template</strong></dt>
 <dd>An element, usually with child elements, that specifies a "rule"
     or set of instructions to perform when a  particular kind of
     node is encountered in the source tree.</dd>
 <br>
 <dt><strong>XSL Template Instruction</strong></dt>
 <dd>Any tag that occurs inside an xsl:template element and is associated
     with the XSLT namespace.</dd>
 <br>
 <dt><strong>Source Tree</strong></dt>
 <dd>The XML tree input to the XSL process.</dd>
 <br>
 <dt><strong>Result Tree</strong></dt>
 <dd>The tree that is output by the XSL process.</dd>
 <br>
 <dt>Match Pattern</dt>
 <dd>The part of a template that defines the kind(s) of nodes to which
     the template applies.</dd>
 <br>
</dl>
 <br>

For more definitions of XSLT terminology, see Dave Pawson's
[XSLT Terminology Clarification](http://www.dpawson.co.uk/xsl/xslvocab.html)
and the Glossary in Michael Kay's
[XSLT Programmer's Reference](http://www.wrox.com/Consumer/Store/Details.asp?ISBN=1861003129).
