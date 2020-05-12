# XML Security Overview

This document goes well beyond XSLT. Use it as a general reference.

There are numerous security issues and problems that are endemic to the
XML architecture.  I will try to identify some of the most common
issues and threats  and describe some mitigation strategies.

The biggest threat issue is a matter of trust.
How well do you trust your sources of XML data?
What are the tools that can help increase the trust?

Most Web Service communications uses HTTP over standard TCP ports.
The HTTP protocol on standard TCP ports has free access through
business firewalls.  How well do your proxy servers handle the Web
Service security issues  required for your applications?

How well are your resource identifiers protected?
How well do your applications cope with resource identifier spoofing?
Can your resource identifiers be trusted by outside clients?
Can you trust the credentials of your clients?

Will the SOAP interface for your Web Service send error messages to an
untrusted Web Service address?

Is your WSDL interface description file readily available for download,
thus enabling persons with malicious intent to create targeted attacks
on your Web Services?

Can you trust the client credentials that use your Web Service
application?

There are numerous security issues that are not directly involved in
the markup of XML or its processing.  These issues relate to
infrastructure.

Can you trust your DNS (Domain Name Service) and reduce its
vulnerability to hijacking?

Are your web servers hardened against known application vulnerabilities?

Are your applications hardened against cross site scripting and SQL
injection?

Can your client applications trust the scripts that are transmitted as
web pages?

Can your web server trust the scripts that are submitted?

Is application data sanitized before being consumed by your
applications?

## XML Parser Threats

This list will help you find the XML threat vectors that need to be
addressed.  Some vectors cannot be easily resolved.

* Resolving External Entities
* Implicit Trust of Internal DTD
* Resource Identifier Spoofing
* Malformed UTF-8 and UTF-16
* Secure the trust of external DTD descriptions
* Secure the trust of external Schema definitions
* Secure the trust of entity import and include constructs
* Configuration of Entity Resolver Catalogs

Resolving External Entities

The XML1.0 and XML1.1 standards specify a `DOCTYPE` format.  The
processing may uncover significant entity resolver deficiencies.

```xml
<!DOCTYPE name PUBLIC "public-id" "system-id" [internal-DTD]>
```
```xml
<!DOCTYPE name SYSTEM "system-id" [internal-DTD]>
```

XML Parsers MUST process the `[internal-DTD]` if it exists.

XML Parsers MAY process the external `"system-id"` if it can be found.

XML Parsers MAY process the external `"public-id"` if it can be found.

XML Parsers MAY prefer either the `"public-id"` or `"system-id"` if
both are specified.

XML Parsers MAY ignore both the `"public-id"` and `"system-id"` if
present.

Declaring a parameter entity notation `"%entity;"`  in the
`[internal-DTD]` and expanding the content within the `[internal-DTD]`
will force the XML parser to import the content referenced by the
`"%entity;"` notation.

Declaring a general entity notation `"&entity;"` in the
`[internal-DTD]` and expanding the content within the body of the XML
document will force the XML parser to import the content referenced by
the `"&entity"` notation.

The default method of resolving external entities is by resolving
entity name strings relative to DNS named hosts and/or path names
relative to the local computer system.  When receiving XML documents
from an outside source, these entity reference locations may be
unreachable, unreliable, or untrusted.

Web Service SOAP XML documents MUST NOT have `DOCTYPE` definitions.
SOAP processors should not process `DOCTYPE` definitions.
The conformance is implementation dependent.

[http://www.w3.org/TR/soap](http://www.w3.org/TR/soap)

## Trusted External Entities

The *OASIS XML Catalogs* specification, if implemented by an
application, can specify a set of external entities that can be
trusted by mapping known identifiers to local or trusted resources.
A secure application should not trust entity identifiers whose
resources cannot be localized and secured.

[http://www.oasis-open.org/committees/entity](http://www.oasis-open.org/committees/entity)

A similar method can be designed specifically for each application.

A trusted application may need to pre-screen any entity definitions in XML
before passing the information into the core of the application.

A trusted application should install some type of entity resolving catalog
or database that can be trusted.

## Processing Instruction (PI) Threats

Processing instructions are a mechanism to send specific information
into an application.  A common processing instruction is a stylesheet
declaration.  This information is part of an XML document and comes
usually after the XML header and before the root element.

A stylesheet declaration may cause an application to look for an
untrusted XSLT stylesheet to use for transformation of the following
root element.  A standard exists for associating style sheets with XML
documents.

[http://www.w3.org/TR/xml-stylesheet](http://www.w3.org/TR/xml-stylesheet)

Examples in the *xml-stylesheet* recommendation describe how to use the
processing instruction to associate CSS stylesheets for XHTML.
Applications that use XSLT transformations will interpret the
xml-stylesheet processing instruction as the location of a XSLT
transformation stylesheet.

As more processing instructions become standardized and in common use,
their threat of misuse increases.

## SOAP Simple Object Access Protocol

The SOAP specification explicitly forbids the transport of `DOCTYPE`
definitions and PI processing instructions.

The SOAP specifies a transport envelope that encapsulates an XML
message for transport.  SOAP can also handle various transmission
status indicators implying confirmation of delivery, error messages,
and queue status messages.  SOAP transports can be loosely coupled and
intermittent.  SOAP is used extensively in the design and deployment of
Web Service architectures.  A companion Web Service specification is
WSDL, the Web Service Definition Language.

The SOAP protocol as widely deployed by Microsoft and other vendors is
based on specifications that predate the adoption by the
[World Wide Web Consortium (W3C)](http://www.w3.org).
SOAP is not based on Microsoft technology.  It is an open standard
drafted by UserLand, Ariba, Commerce One, Compaq, Developmentor, HP,
IBM, IONA, Lotus, Microsoft, and SAP.
[SOAP 1.1](http://www.w3.org/TR/2000/NOTE-SOAP-20000508)
was presented to the W3C in May 2000 as an official Internet standard.

The original [SOAP 1.1](http://www.w3.org/TR/soap11) standard is
associated with this URI namespace prefix:
`http://schemas.xmlsoap.org/soap/`

There are significant changes in naming conventions since SOAP 1.1
was adopted by W3C as a recommended standard.
The current iteration is [SOAP 1.2](http://www.w3.org/TR/soap12)
and is associated with this URI namespace prefix:
`http://www.w3.org/2003/05`

The basic security threat to the SOAP architecture is the ability to
spoof Web Service addresses and telling a SOAP server to respond to a
rogue Web Service address when a `mustUnderstand` attribute is
processed and an error indication is raised.

Other intelligence that can be obtained might be the location of a
public accessible WSDL definition of the messages being transported by
SOAP, thus allowing additional malware attacks to be automatically
generated.

## WSDL Web Service Description Language

WSDL is known as the Web Service Description Language.  The WSDL XML
document is a an interface description that can be transformed into
various programming languages.  Such transformed interface
descriptions are recognized as Java Interfaces and C++ Virtual Classes.

The original [WSDL 1.1](http://www.w3.org/TR/wsdl) standard is
associated with this URI namespace prefix:
`http://schemas.xmlsoap.org/wsdl/`

The current [WSDL 2.0](http://www.w3.org/TR/wsdl20) standard is
maintained by W3C in their namespace with prefix:
`http://www.w3.org/`

The WSDL can provide a template for generating a compliant Web Service
systems for multiple and hetrogeneous platforms.

A WSDL document that can benefit developers can also be used by malware
and hackers to tailor specific threats against targeted Web Services.

The SOA (Service Oriented Architecure), SAAS (Software As A Service),
PAAS (Platform As A Service) are families of Web Services used as
interfaces into what is generally known as Cloud Computing.

## URI Uniform Resource Identifiers

The URI does not need to specify the location of a resource.  It merely
provides a resource name. A catalog, database, or other mechanism is
used to map URIs to resource locations.

The security issue here is that most URIs are used with a DNS (Domain
Name Service) to find a host and path to a resource.  The URI is then
treated as a URL (Uniform Resource Locator).

The mitigation of these threats requires diligence of the application
architects to ensure an appropriate level of trust for the URIs and
URLs used in their applications.

The transmission media is inherently untrusted.  Often SOAP bindings
and HTTP transports are used.  Web Service addressing is readily
spoofed.

## URL Uniform Resource Locators

See [URI Uniform Resource Identifiers](#uri-uniform-resource-identifiers)

## Malformed UTF-8 and UTF-16 Strings

Public Key Infrastructure (X.509) certificates are leased from a
certificate authority or are self-signed.  The distinguished names and
parts thereof are usually rendered in unicode.

The value of zero is not a valid Unicode character.  It is possible to
create non-zero UTF-8 and UTF-16 sequences that equate to zero, which
is not allowed.  Some rogue hackers have successfully obtained
wild-card PKI (X.509) certificates by prepending a UTF-8(zero) in a
distinguished name when applying for a certificate.  Such a certificate
could be used to successfully sign anything.

Applications should not blindly accept UTF-8 and UTF-16 strings without
verifying the proper encoding for those strings.  Contents that equate
to bad Unicode character values should be denied.

## Canonical XML Issues

Canonical XML is a tranformation of an XML document into a canonical
form useful for signing.  This is used in some Web Service security
implementations.

There are several areas where Canonical XML will create XML documents
that have severe application problems.

The number values are rendered in Base-10 as decimal fractions.  The
computations performed by computers are usually in Base-2 floating
point arithmetic.  You therefore have truncation or roundoff issues
when converting between decimal fractions and Base-2 fractions.

The canonical process may collapse whitespace and transform
multi-character line endings to single-character line endings.  When
whitespace is significant, the canonical issues for signing can cause
problems.

It is possible to create XHTML documents that will not work with some
browsers.  The empty `<a/>` anchor element is not allowed by many
browsers, therefore `<a></a>` is required.  A standard XML canonical
process may collapse elements with no content into empty elements.
The empty paragraph `<p/>` is disallowed.  The `<p></p>` is supported.

The World Wide Web Consortium (W3C) has additional detailed discussion
of [canonicalization issues](http://www.w3.org/TR/C14N-issues/)

## XHTML Output Mode - Workaround

The Xalan-C/C++ library currently has no XHTML output mode.  Since
XHTML is to be well-formed XML, the desire is to use the XML output
method.

XHTML is based on HTML version 4.

Empty elements declared by HTML-4 should have a space before the
trailing `/>` markup (i.e. `<br />` and `<hr />`).  XML output mode
does not normally have this space when using the
`<xsl:element name="br" />` in your stylesheet.  Most modern browsers
are ok with no space, but viewing the browser source shows a warning
condition.

Non-empty elements declared by HTML-4 should not be rendered as empty
XML elements.  If there is no content, the elements should be rendered
with both a start-tag and end-tag (i.e. `<a name="xxx"></a>`) instead
of an XML empty-element.  XSLT processors usually create an
empty-element (i.e. `<a name="xxx"/>`) when the element being defined
has no content other than attributes.

For XSLT processors creating XML documents for XHTML, you can create
what looks like an element with no content by including the `&#8204;`
character (a zero-width non-joining character often known as `&zwnj;`)
as the element text content.  This also allows transitional browsers
the ability to find the end tag.

```
  DTD    <!ENTITY zwnj    "&#8204;">

  <a name="marker">&zwnj;</a>
````

Transitional XHTML is not usually well-formed XML.  It becomes a mix of
HTML version 4 and XML markup.  Strict XHTML is required to be
well-formed XML.
