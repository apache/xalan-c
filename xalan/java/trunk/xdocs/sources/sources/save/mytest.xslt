<?xml version="1.0" encoding="ISO-8859-1" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!--
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
-->

<!--
* <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
* "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
-->

<xsl:output  method="xml" version="1.0" indent="yes"
  doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
  doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"
  omit-xml-declaration="no"
  encoding="ISO-8859-1"
  standalone="no"
/>

<!-- THE TOP-LEVEL PARAMETERS (sectionid, createdate)

  sectionid   = basename of the source xml file without the (.xml) extension.
  createdate  = date that this stylesheet is used.

  Building the documentation using commandline stylesheet transformation.

  EXSLT -p sectionid='documentfile' -p createdate='June 1, 2011'
    xalan\documentfile.xml  xalan-manual.xslt  ..\html\documentfile.html
-->

<xsl:param name="sectionid"/>
<xsl:param name="createdate"/>


<!-- THE STYLESHEET ROOT - LAUNCH THE CONSTRUCTORS -->

<xsl:template match="/">
<html>
<head>
 <title>Xalan-C++ XSLT User Guide Version 1.11</title>
 <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1"/>
 <meta http-equiv="Content-Style-Type" content="text/css"/>
 <link rel="stylesheet" type="text/css" href="resources/apache-xalan.css"/>
</head>
 <body>

   <!-- CONSTRUCT A BOOK SUBTITLE TITLE FROM SOURCE XML PROPERTIES -->

   <xsl:variable name="xalantitle">
     <xsl:choose>
       <xsl:when test="s1/@title">
         <xsl:value-of select="s1/@title"/>
       </xsl:when>
       <xsl:when test="s2/@title">
         <xsl:value-of select="s2/@title"/>
       </xsl:when>
       <xsl:when test="s3/@title">
         <xsl:value-of select="s3/@title"/>
       </xsl:when>
       <xsl:when test="faqs">
         <xsl:choose>
           <xsl:when test="faqs/@title">
             <xsl:value-of select="faqs/@title"/>
           </xsl:when>
           <xsl:otherwise>
             <xsl:text>Frequently Asked Questions</xsl:text>
           </xsl:otherwise>
         </xsl:choose>
       </xsl:when>
       <xsl:when test="resources">
         <xsl:text>Resources</xsl:text>
       </xsl:when>
       <xsl:when test="changes/@title">
         <xsl:value-of select="changes/@title"/>
       </xsl:when>
       <xsl:otherwise>
         <xsl:text>DEBUG - xalantitle not found</xsl:text>
       </xsl:otherwise>
     </xsl:choose>
   </xsl:variable>

   <!-- THE TITLE SECTION -->

   <xsl:call-template name="doTitle">
     <xsl:with-param name="booktitle">
       <xsl:text>Xalan-C++ XSLT 1.11 User Guide</xsl:text>
     </xsl:with-param>
     <xsl:with-param name="booksection">
       <xsl:value-of select="$xalantitle"/>
     </xsl:with-param>
   </xsl:call-template>

   <!-- THE NAVIGATION SECTION -->

   <xsl:call-template name="doLeftNav">
     <xsl:with-param name="sectparm">
       <xsl:value-of select="$sectionid"/>
     </xsl:with-param>
   </xsl:call-template>

   <!-- THE MAIN CONTENT SECTION -->

   <xsl:call-template name="doContent">
     <xsl:with-param name="mysectionid">
       <xsl:value-of select="$sectionid"/>
     </xsl:with-param>
   </xsl:call-template>

   <!-- THE COPYRIGHT FOOTER -->

   <xsl:call-template name="doFooter">
     <xsl:with-param name="releaseinfo">
       <xsl:text>Web Pages created on - </xsl:text>
       <xsl:value-of select="$createdate"/>
     </xsl:with-param>
   </xsl:call-template>
 </body>
</html>
</xsl:template>


<!-- DEBUG TEMPLATE -->
<xsl:template name="DEBUG">
  <xsl:param name="theText"/>
  <xsl:param name="theNode"/>
  <xsl:element name="DEBUG">
    <xsl:element name="DEBUGTEXT">
      <xsl:value-of select="$theText"/>
    </xsl:element>
    <xsl:if test="$theNode">
      <xsl:element name="DEBUGNODES">
        <xsl:copy-of select="$theNode"/>
      </xsl:element>
    </xsl:if>
  </xsl:element>
</xsl:template>


<xsl:template name="doTitle">
  <xsl:param name="booktitle">Xalan-C++ Version 1.11 User Guide</xsl:param>
  <xsl:param name="booksection">Documentation Project</xsl:param>
  <xsl:element name="div">
    <xsl:attribute name="id">
       <xsl:text>title</xsl:text>
    </xsl:attribute>
 <table>
 <tbody>
 <tr>
  <th rowspan="2">
   <a href="http://www.apache.org/">
   <img alt="ASF Logo" title="ASF Logo" 
       src="resources/asf_logo.png" 
       width="220" height="66"/></a>
  </th>
  <th text-align="center" width="75%">
    <a href="index.html"><xsl:value-of select="$booktitle"/></a>
  </th>
 </tr>
 <tr>
  <td valign="middle"><xsl:value-of select="$booksection"/></td>
 </tr>
 </tbody>
 </table>
  </xsl:element>
</xsl:template>


<xsl:template name="doLeftNav">
<xsl:param name="sectparm" select="''"/>
<xsl:element name="div">
  <xsl:attribute name="id">navLeft</xsl:attribute>
  <ul>
  <xsl:for-each select="document('xalan.xml')/book/*">
    <xsl:if test="name(.)='external'">
      <li>
      <xsl:element name="a">
        <xsl:attribute name="href">
          <xsl:value-of select="@href"/>
        </xsl:attribute>
        <xsl:value-of select="@label"/>
      </xsl:element>
      </li>
    </xsl:if>
    <xsl:if test="name(.)='resources'">
      <li>
      <xsl:choose>
        <xsl:when test="$sectparm != 'resources'">
          <a href="resources.html">Resources</a><br/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>Resources</xsl:text><br/>
        </xsl:otherwise>
      </xsl:choose>
      </li>
    </xsl:if>
    <xsl:if test="name(.)='changes'">
      <li>
      <xsl:choose>
        <xsl:when test="$sectparm != 'changes'">
          <a href="changes.html">Changes</a><br/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>Changes</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      </li>
    </xsl:if>
    <xsl:if test="name(.)='separator'">    <!--  </ul>   <hr/>      <ul> -->
      <xsl:text disable-output-escaping="yes">&lt;/ul>&lt;hr/&gt;&lt;ul></xsl:text>
    </xsl:if>
    <xsl:if test="(name(.)='document') or (name(.)='faqs')">
      <li>
      <xsl:choose>
        <xsl:when test="$sectparm != string(@id)">
          <xsl:element name="a">
            <xsl:attribute name="href">
              <xsl:value-of select="concat(@id,'.html')"/>
            </xsl:attribute>
            <xsl:value-of select="@label"/>
          </xsl:element>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="@label"/><br/>
        </xsl:otherwise>
      </xsl:choose>
      </li>
    </xsl:if>
  </xsl:for-each>
  </ul>
</xsl:element>
</xsl:template>


<xsl:template name="doContent">
  <xsl:element name="div">
    <xsl:attribute name="id">content</xsl:attribute>
    <xsl:choose>
      <xsl:when test="faqs">
        <xsl:for-each select="faqs">
          <xsl:call-template name="doFaqDocument"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="resources">
        <xsl:for-each select="resources">
          <xsl:call-template name="doResDocument"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="changes">
        <xsl:for-each select="changes">
          <xsl:call-template name="doChangesDoc"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:element>
</xsl:template>


<xsl:template name="doFooter">
  <xsl:param name="releaseinfo"/>
  <xsl:element name="div">
    <xsl:attribute name="id">footer</xsl:attribute>
    <!-- copyright byline information -->
    <!-- &#169; is the (c) copyright symbol -->
    <xsl:text>Copyright &#169; 1999-2011 The Apache Software Foundation</xsl:text><br/>
    <xsl:text>Apache, Xalan, and the Feather logo are trademarks of The Apache Software Foundation</xsl:text>
    <xsl:element name="div">
      <xsl:attribute name="class">small</xsl:attribute>
      <xsl:value-of select="$releaseinfo"/>
    </xsl:element>
  </xsl:element>
</xsl:template>


<xsl:template name="doFaqDocument">
  <h2 class="faqSubHead">Questions</h2>
  <ol start="1">
  <xsl:call-template name="doFaqTitles">
    <xsl:with-param name="faqNbr">1</xsl:with-param> 
  </xsl:call-template>
  </ol>
  <h2 class="faqSubHead">Answers</h2>
  <xsl:call-template name="doFaqContent">
    <xsl:with-param name="faqNbr">1</xsl:with-param>
  </xsl:call-template>
</xsl:template>


<xsl:template name="doFaqTitles">
  <xsl:param name="faqNbr">1</xsl:param>
  <xsl:variable name="myNbr" select="$faqNbr"/>
  <xsl:if test="faq[position()=$myNbr]">
    <!-- output the title reference -->
    <li>
    <xsl:element name="a">
      <xsl:attribute name="href">
        <xsl:value-of select="concat('#faq-', $myNbr)"/>
      </xsl:attribute>
      <xsl:value-of select="faq[position()=$myNbr]/@title"/>
    </xsl:element>
    </li>
    <!-- get the next faq if any -->
    <xsl:call-template name="doFaqTitles">
      <xsl:with-param name="faqNbr" select="number($myNbr + 1)"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template> 


<xsl:template name="doFaqContent">
  <xsl:param name="faqNbr">1</xsl:param>
  <xsl:variable name="myNbr" select="$faqNbr"/>

  <xsl:if test="faq[position()=$myNbr]">            <!-- output content of this element only -->

<!--  <h3><xsl:value-of select="faq[position()=$myNbr]/@title"/></h3>     -->

    <xsl:element name="h3">
      <xsl:attribute name="id">
        <xsl:value-of select="concat('faq-', $myNbr)"/>
      </xsl:attribute>
      <xsl:attribute name="class">
        <xsl:text>faqTitle</xsl:text>
      </xsl:attribute>
      <xsl:value-of select="concat($myNbr, '. ', faq[position()=$myNbr]/@title)"/>
    </xsl:element>

<!--  The FAQ Text Part                            -->
    <xsl:for-each select="faq[position()=$myNbr]/*">      <!-- children of 'faq' -->
      <xsl:choose>
        <xsl:when test="name(.)='q'">                  <!-- The Question Text -->
          <div class="faqQuestion">
            <xsl:apply-templates/>               <!-- children of 'q' -->
          </div>
        </xsl:when>
        <xsl:when test="name(.)='a'">                  <!-- The Answer Text -->
          <br/>
          <hr/>
          <div class="faqAnswer">
            <xsl:apply-templates/>               <!-- children of 'a' -->
          </div>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>

    <xsl:call-template name="goToTop"/>

<!--  Recurse for next faq item                    -->
    <xsl:call-template name="doFaqContent">
      <xsl:with-param name="faqNbr" select="number($myNbr + 1)"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template> 


<xsl:template name="goToTop">
  <p align="right" size="2">
    <a href="#content">(top)</a>
  </p>
</xsl:template>


<xsl:template name="doResDocument">
  <xsl:for-each select="resource|human-resource">
    <xsl:choose>
      <xsl:when test="name(.)='resource'">
<!--    <h4 class="resourcetitle"><strong><xsl:value-of select="@title"/></strong><h4> -->
        <xsl:element name="h4">
          <xsl:attribute name="class">resourcetitle</xsl:attribute>
          <strong><xsl:value-of select="@title"/></strong>
        </xsl:element>
        <p>id = [<xsl:value-of select="@id"/>]<br/>
        <xsl:element name="a">
          <xsl:attribute name="href">
            <xsl:value-of select="@location"/>
          </xsl:attribute>
          <xsl:value-of select="@location"/>
        </xsl:element><br/></p>
      </xsl:when>
      <xsl:when test="name(.)='human-resource'">
<!--    <h4 class="resourcetitle"><strong><xsl:value-of select="@name"/></strong><h4> -->
        <xsl:element name="h4">
          <xsl:attribute name="class">resourcetitle</xsl:attribute>
          <strong><xsl:value-of select="@name"/></strong>
        </xsl:element>
        <p>id = [<xsl:value-of select="@id"/>]<br/>
        <xsl:element name="a">
          <xsl:attribute name="href">
            <xsl:value-of select="concat('mailto:',@mailto)"/>
          </xsl:attribute>
          <xsl:value-of select="concat('mailto:',@mailto)"/>
        </xsl:element><br/></p>
      </xsl:when>
    </xsl:choose>
  </xsl:for-each>
</xsl:template>

<xsl:template name="doChangesDoc">
  <!-- context is <changes> element -->
  <xsl:for-each select="release">
    <h3 class="chgRelease">
      <xsl:value-of select="concat('Release ',@version,' (',@date,')')"/>
    </h3>
    <xsl:if test="features">
      <h4 class="chgFeatures"><xsl:text>Features</xsl:text></h4>
      <xsl:for-each select="features">
        <ul>
          <xsl:for-each select="feat">
            <li><xsl:apply-templates/></li>
          </xsl:for-each>
        </ul>
        <hr/>
      </xsl:for-each>
    </xsl:if>
    <xsl:if test="fixes">
      <h4 class="chgFixes"><xsl:text>Bug Fixes</xsl:text></h4>
      <xsl:for-each select="fixes">
        <ul>
          <xsl:for-each select="fix">
            <li><xsl:apply-templates/></li>
          </xsl:for-each>
        </ul>
        <hr/>
      </xsl:for-each>
    </xsl:if>
    <xsl:call-template name="goToTop"/>
  </xsl:for-each>
</xsl:template>

<!-- THE MATCHING TEMPLATE RULES -->

<xsl:template match="s1">
  <h2><xsl:value-of select="@title"/></h2>
  <xsl:apply-templates/>
  <xsl:call-template name="goToTop"/>
</xsl:template>

<xsl:template match="s2">
  <xsl:call-template name="goToTop"/>
  <h3><xsl:value-of select="@title"/></h3>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="s3">
  <xsl:call-template name="goToTop"/>
  <h4><xsl:value-of select="@title"/></h4>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="s4">
  <h5><xsl:value-of select="@title"/></h5>
  <xsl:apply-templates/>
</xsl:template>

<!--<!ENTITY % blocks "p|note|ul|ol|gloss|table|source|anchor">-->

<xsl:template match="p">
  <p><xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="ul">
  <ul><xsl:apply-templates/></ul>
</xsl:template>

<xsl:template match="ol">
  <ol><xsl:apply-templates/></ol>
</xsl:template>

<xsl:template match="li">
  <li><xsl:apply-templates/></li>
</xsl:template>

<xsl:template match="table">
  <table border="1"><xsl:apply-templates/></table>
</xsl:template>

<xsl:template match="tr">
  <tr><xsl:apply-templates/></tr>
</xsl:template>

<xsl:template match="tn">
 <td class="empty"/><!-- empty table element -->
</xsl:template>

<xsl:template match="th">
  <xsl:element name="th">
    <xsl:if test="@rowspan">
      <xsl:attribute name="rowspan">
        <xsl:value-of select="@rowspan"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@colspan">
      <xsl:attribute name="colspan">
        <xsl:value-of select="@colspan"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<xsl:template match="td">
  <xsl:element name="td">
    <xsl:if test="@rowspan">
      <xsl:attribute name="rowspan">
        <xsl:value-of select="@rowspan"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@colspan">
      <xsl:attribute name="colspan">
        <xsl:value-of select="@colspan"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<xsl:template match="note">
  <blockquote class="note"><xsl:apply-templates/></blockquote>
</xsl:template>

<xsl:template match="source">
  <blockquote class="source"><pre><xsl:value-of select="."/></pre></blockquote>
</xsl:template>

<xsl:template match="hidden">
<!-- content is hidden -->
</xsl:template>

<!--<!ENTITY % markup "em|ref|code|br">-->

<xsl:template match="br">
  <br/>
</xsl:template>

<xsl:template match="em">
  <b><xsl:apply-templates/></b>
</xsl:template>

<xsl:template match="code">
  <code><xsl:apply-templates/></code>
</xsl:template>

<xsl:template match="ref">
  <b><i><xsl:apply-templates/></i></b>
</xsl:template>

<!-- Glossary Items <label> <item> -->

<xsl:template match="label">
  <p class="label"><xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="item">
  <p class="item"><xsl:apply-templates/></p>
</xsl:template>

<!--<!ENTITY % links "link|anchor|jump|img|resource-ref|human-resource-ref">-->

<xsl:template match="anchor">
  <xsl:element name="a">
    <xsl:attribute name="name">
       <xsl:value-of select="@name"/>
    </xsl:attribute>
    <xsl:text>&#8204;</xsl:text> <!-- (zero-width non-join) character -->
  </xsl:element>
</xsl:template>

<xsl:template match="img">
  <xsl:copy-of select="."/>
</xsl:template>

<!-- Glossary elements "gloss (label|item)* -->

<xsl:template match="gloss">
  <xsl:element name="div">
    <xsl:attribute name="class">glossary</xsl:attribute>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<xsl:template match="label">
  <p class="label"><em>
  <xsl:apply-templates/>
  </em></p>
</xsl:template>

<xsl:template match="item">
  <xsl:element name="blockquote">
    <xsl:attribute name="class">item</xsl:attribute>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<!-- ********
<!ELEMENT link (#PCDATA|img)*>
<!ATTLIST link idref NMTOKEN #IMPLIED
    anchor CDATA  #IMPLIED>

<!ELEMENT jump (#PCDATA|img)*>
<!ATTLIST jump href CDATA #REQUIRED>
*************
-->

<xsl:template match="link">
  <xsl:element name="a">
    <xsl:attribute name="href">
      <xsl:if test="@idref">
        <xsl:value-of select="concat(@idref, '.html')"/>
      </xsl:if>
      <xsl:if test="@anchor">
        <xsl:value-of select="concat('#', @anchor)"/>
      </xsl:if>
    </xsl:attribute>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<xsl:template match="jump">
  <xsl:element name="a">
    <xsl:attribute name="href">
      <xsl:value-of select="@href"/>
    </xsl:attribute>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<!-- cross-reference resource elements -->

<!--
 Resolve Resource References through "resources.xml" document
-->

<xsl:template match="resource-ref">
  <xsl:variable name="idval" select="@idref"/>
  <xsl:variable name="resref" 
        select="document('xalan/resources.xml')//resource[@id=$idval]"/>
  <xsl:element name="a">
    <xsl:attribute name="href">
      <xsl:value-of select="$resref/@location"/>
    </xsl:attribute>
    <xsl:value-of select="$resref/@title"/>
  </xsl:element>
</xsl:template>

<!--
 Resolve Mailbox References through "resources.xml" document
-->

<xsl:template match="human-resource-ref">
  <xsl:variable name="idval" select="@idref"/>
  <xsl:variable name="resref" 
        select="document('xalan/resources.xml')//human-resource[@id=$idval]"/>
  <xsl:element name="a">
    <xsl:attribute name="href">
      <xsl:value-of select="concat('mailto:',$resref/@mailto)"/>
    </xsl:attribute>
    <xsl:value-of select="$resref/@name"/>
  </xsl:element>
</xsl:template>


</xsl:stylesheet>
