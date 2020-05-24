# Introduction

## Xalan-C++

Xalan-C++ version is a robust implementation of the W3C Recommendations
for
[XSL Transformations (XSLT)](http://www.w3.org/TR/xslt)
and the
[XML Path Language (XPath)](http://www.w3.org/TR/xpath).
It works with the
[Xerces-C++](http://xerces.apache.org/xerces-c)
XML parser.

The Xalan-C++ project creates and distributes a standard XSLT library
and a simple [`Xalan`](commandline.md) command-line utility for
transforming XML documents.

This project is written in a reduced set of the C++ programming
language.  This makes the library directly usable on numerous
platforms.  A set of 'C' language APIs is also available so that XSLT
and XPath functionality is easily deployed to 'C' language applications.

[Sample programs](samples.md) are available for you to test and
experiment with as you learn the Xalan library.

Some of the [EXSLT extension functions](http://www.exslt.org) for XPath
are available.  We need to write and validate more of the EXSLT
functions.  A library of [XPath extensions](extensionslib.md) is
provided by the Apache Xalan community. Documentation and examples are
available for you to write your own custom
[XPath extensions](extensions.md).

## Licenses

The Xalan-C++ software conforms to The [Apache Software Foundation
License, Version 2.0](licensing.md).

## Getting Involved

You are invited to participate in
[The Apache Software Foundation](http://www.apache.org/foundation/getinvolved.html).
You will need to subscribe if you wish to receive mail from the lists.
Some lists require a subscription in order to post messages.

The *xalan-dev* (`dev@xalan.apache.org`) mailing list is used to
communicate product development issues with the software development
team.  The xalan-dev messages are
[archived here](http://mail-archives.apache.org/mod_mbox/xalan-dev)
and on
[Marc.info](http://marc.info/?l=xalan-dev).

The *xalan-c-users* (`c-users@xalan.apache.org`) mailing list is used
to discuss issues regarding the use of the Xalan-C++ software and
libraries.  The xalan-c-users messages are
[archived here](http://mail-archives.apache.org/mod_mbox/xalan-c-users)
and on
[Marc.info](http://marc.info/?l=xalan-c-users).

The Git repository changes are logged on the *xalan-commits* mailing
list and are
[archived here](http://mail-archives.apache.org/mod_mbox/xalan-commits)
and on
[Marc.info](http://marc.info/?l=xalan-cvs).

You can review the project sources and change history in the Git repository on
[GitHub](https://github.com/apache/xalan-c)
or
[Apache GitBox](https://gitbox.apache.org/repos/asf?p=xalan-c.git).

You can clone a private copy of the repository using either service.

The [JIRA](https://issues.apache.org/jira/projects/XALANC) system is
used for issue tracking.  You must create an account before you can
post messages and files to the JIRA tracking system.
You can view the issues at
[https://issues.apache.org/jira/projects/XALANC](https://issues.apache.org/jira/projects/XALANC).

## XSLT Standards Implemented

Xalan-C++ implements the following specifications from the World Wide
Web Consortium (W3C).

* [XSL Transformations (XSLT) Version 1.0](http://www.w3.org/TR/xslt)
* [XML Path Language (XPath) Version 1.0](http://www.w3.org/TR/xpath)
* [Associating Style Sheets with XML documents 1.0 (Second Edition)](http://www.w3.org/TR/xml-stylesheet)

The next major release of Xalan-C++ will hopefully be compatible with
the following standards:

* [XSL Transformations (XSLT) Version 2.0](http://www.w3.org/TR/xslt20)
* [XSLT 2.0 and XQuery 1.0 Serialization](http://www.w3.org/TR/xslt-xquery-serialization)
* [XML Path Language (XPath) Version 2.0 (Second Edition)](http://www.w3.org/TR/xpath20)
* [XQuery 1.0 and XPath 2.0 Formal Semantics (Second Edition)](http://www.w3.org/TR/xquery-semantics)
* [XQuery 1.0 and XPath 2.0 Data Model (XDM) (Second Edition)](http://www.w3.org/TR/xpath-datamodel)
* [XQuery 1.0 and XPath 2.0 Functions and Operators (Second Edition)](http://www.w3.org/TR/xpath-functions)

This added support will require a major version change from 1 to 2.

## XML Parser Library

Xalan-C++ uses the The Apache Software Foundation Xerces-C project that
implements parsers for XML 1.0 and XML 1.1.  The following standards as
specified by the
[World Wide Web Consortium (W3C)](http://www.w3.org/TR)
are implemented by the
[Xerces-C++](http://xerces.apache.org/xerces-c) XML parser library:

* [Extensible Markup Language (XML) 1.0 (Third Edition) - currently (Fifth Edition)](http://www.w3.org/TR/xml)
* [Extensible Markup Language (XML) 1.1 (First Edition) - currently (Second Edition)](http://www.w3.org/TR/xml11)
* [xml:id Version 1.0](http://www.w3.org/TR/xml-id)
* [XML Base (Second Edition)](http://www.w3.org/TR/xmlbase)
* [XML Information Set (Second Edition)](http://www.w3.org/TR/xml-infoset)
* [Namespaces in XML 1.0 (Third Edition)](http://www.w3.org/TR/xml-names)
* [Namespaces in XML 1.1 (Second Edition)](http://www.w3.org/TR/xml-names11)
* [XML Schema Part 1: Structures Second Edition](http://www.w3.org/TR/xmlschema-1)
* [XML Schema Part 2: Datatypes Second Edition](http://www.w3.org/TR/xmlschema-2)
* [Document Object Model (DOM) Level 1](http://www.w3.org/TR/REC-DOM-Level-1)
* [Element Traversal Specification](http://www.w3.org/TR/ElementTraversal)
* [Document Object Model (DOM) Level 2 Core Specification](http://www.w3.org/TR/DOM-Level-2-Core)
* [Document Object Model (DOM) Level 2 Traversal and Range Specification](http://www.w3.org/TR/DOM-Level-2-Traversal-Range)
* [Document Object Model (DOM) Level 3 Core Specification](http://www.w3.org/TR/DOM-Level-3-Core)
* [Document Object Model (DOM) Level 3 Validation Specification](http://www.w3.org/TR/DOM-Level-3-Val)
* [Document Object Model (DOM) Level 3 Load and Save Specification](http://www.w3.org/TR/DOM-Level-3-LS)
* [XML Inclusions (XInclude) 1.0 (Second Edition)](http://www.w3.org/TR/xinclude)

Xerces-C++ also supports the
[SAX 1.0 and SAX 2.0](http://sax.sourceforge.net/) projects.
SAX is primarily Java based.  The Xerces-C++ project has implemented a
SAX C++ API that looks much like the SAX Java API.

Web Services and Web Security recommendations have been developed by the
[World Wide Web Consortium (W3C)](http://www.w3.org)
and
[OASIS-OPEN](http://www.oasis-open.org)
Advancing open standards for the information society.
Xerces-C++ has support for custom entity resolvers which are inherited
and supported by the Xalan-C++ library.

## Security Issues

The Xalan-C++ library has hooks so that custom entity resolvers can be
deployed.   The entity resolvers are actually interfaces into the
Xerces-C++ XML Parser Library.  Use of trusted entity resolvers greatly
increases the security of XML based applications.

The Web Services Architecture uses specifications and recommendations
that are documented by the following organizations:

* World Wide Web Consortium (W3C)
  [[http://www.w3.org](http://www.w3.org)]
* Advancing open standards for the information society (OASIS-OPEN)
  [[http://www.oasis-open.org](http://www.oasis-open.org)]
* The Sourceforge SAX Project (SAX1, SAX2)
  [[http://sax.sourceforge.net](http://sax.sourceforge.net)]
* Internet Engineering Task Force (IETF)
  [[http://www.ietf.org](http://www.ietf.org)]

There are some [architecture security](secureweb.md) issues that should
be addressed by any XML parser or XSLT transformation application.
Other than validating and cleaning the content of XML elements, the
access to untrusted external entities is a big problem for security
management.

The [`Xalan`](commandline.md) command-line transformation program does
not deploy an OASIS catalog or perform other security measures.  URLs
are resolved using the system directory of the local machine and DNS to
resolve host locations.  The `Xalan` program should be used only with
trusted XSLT stylesheets and XML documents on a trusted computer.

A future release of the `Xalan` command-line transformation program may
include support for an OASIS entity resolver catalog.

Information on creating Secure Web Service Applications is useful for
review. This [security overview document](secureweb.md) goes beyond the
scope of XSLT transformations and asks many pertinent security based
questions.

## Getting Started

See the [download](download.md) and [build](build.md) instructions for
how to build Xalan-C++ from source.  See the [install](install.md) for
how to install prebuilt packages for your operating system directly.

The current Xalan-C++, Version 1.12, is compatible with the stable
releases of [Xerces-C++](http://xerces.apache.org/xerces-c), Version
3.0 and newer.  The current stable release for Xerces-C++ is 3.2.3.

If you are having problems using the product, send your comments to the
*xalan-c-users* (`c-users@xalan.apache.org`) mailing list.

If you think a change needs to be made or have suggestions for future
releases,  send your comments to the *xalan-dev*
(`dev@xalan.apache.org`) mailing list.
