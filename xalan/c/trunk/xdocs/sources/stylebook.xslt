<?xml version="1.0" encoding="UTF-8" ?>
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
  encoding="UTF-8"
  standalone="no"
/>

<!-- THE TOP-LEVEL PARAMETERS 

  logoimage   = resource image for active logo (resources/asf_logo.png)
  logolink    = logo active hyperlink          (http://xalan.apache.org)
  logowidth   = width of trademark logo in pixels
  logoheight  = height of tracemark logo in pixels

  bookxml     = path to the book.xml for navigation and book title
  booklink    = path to the index.html for the book

  sectionid   = basename of the source xml file without the (.xml) extension.
  createdate  = date that this stylesheet is used.

  xmlsources  = directory where project sources reside

 ===============

  Building a documentation web page using a commandline XSLTt transformation program

  EXSLT -p sectionid='getstarted' -p createdate='Aug 7, 2011'
    -p logoimage='resources/xalan-logo.png'
    -p logolink='http://xalan.apache.org'
    -p logowidth='144'
    -p logoheight='75'
    -p bookxml='xalan.xml'
    -p booklink='index.html'
    xalan\documentfile.xml  xalan-manual.xslt  ..\html\documentfile.html
-->

<xsl:param name="logoimage">resources/asf_logo.png</xsl:param>
<xsl:param name="logolink">http://www.apache.org</xsl:param>
<xsl:param name="logowidth">144</xsl:param>
<xsl:param name="logoheight">75</xsl:param>

<xsl:param name="bookxml">xalan.xml</xsl:param>
<xsl:param name="booklink">index.html</xsl:param>

<xsl:param name="sectionid">index</xsl:param>
<xsl:param name="createdate"/>

<xsl:param name="xmlsources">xalan</xsl:param>


<!-- THE ROOT TEMPLATE TO BUILD ASF STYLEBOOK WEB PAGE -->

<xsl:template match="/">

<!-- CONSTRUCT A BOOK TITLE FROM THE REFERENCED BOOK XML -->

  <xsl:variable name="booktitle">
    <xsl:value-of select='document($bookxml)/book/@title'/>
  </xsl:variable>


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

<xsl:element name='html'>
  <xsl:element name='head'>
    <xsl:element name='title'>
      <xsl:value-of select="concat('ASF: ',$xalantitle)"/>
    </xsl:element><!--/title-->
    <meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
    <meta http-equiv="Content-Style-Type" content="text/css"/>
    <link rel="stylesheet" type="text/css" href="resources/apache-xalan.css"/>
  </xsl:element><!--/head-->

<!-- PUT IN THE APACHE SOFTWARE FOUNDATION LICENSING STATEMENT -->
 <xsl:comment>
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
 </xsl:comment>

 <body>
   <!-- THE TITLE SECTION -->

   <xsl:call-template name="doTitle">
     <xsl:with-param name="headtitle">
       <xsl:value-of select="$booktitle"/>
     </xsl:with-param>
     <xsl:with-param name="headtitlelink">
       <xsl:value-of select="$booklink"/>
     </xsl:with-param>
     <xsl:with-param name="headsubtitle">
       <xsl:value-of select="$xalantitle"/>
     </xsl:with-param>
     <xsl:with-param name="headlogoimg">
       <xsl:value-of select="$logoimage"/>
     </xsl:with-param>
     <xsl:with-param name="headlogolink">
       <xsl:value-of select="$logolink"/>
     </xsl:with-param>
     <xsl:with-param name="headlogoheight">
       <xsl:value-of select="$logoheight"/>
     </xsl:with-param>
     <xsl:with-param name="headlogowidth">
       <xsl:value-of select="$logowidth"/>
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
       <xsl:text>Web Page created on - </xsl:text>
       <xsl:value-of select="$createdate"/>
     </xsl:with-param>
   </xsl:call-template>
 </body>
</xsl:element><!--html-->
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
  <xsl:param name="headtitle">XML StyleBook Example</xsl:param>
  <xsl:param name="headtitlelink">index.html</xsl:param>
  <xsl:param name="headsubtitle">Documentation Test Example</xsl:param>
  <xsl:param name="headlogoimg">resources/asf_logo.png</xsl:param>
  <xsl:param name="headlogolink">http://www.apache.org</xsl:param>
  <xsl:param name="headlogoheight"><xsl:value-of select="$logoheight"/></xsl:param>
  <xsl:param name="headlogowidth"><xsl:value-of select="$logowidth"/></xsl:param>
  <xsl:element name="div">
    <xsl:attribute name="id">
       <xsl:text>title</xsl:text>
    </xsl:attribute>
    <table class="HdrTitle">
    <tbody>
    <tr>
     <th rowspan="2">
      <xsl:element name='a'>
        <xsl:attribute name='href'><xsl:value-of select='$headlogolink'/></xsl:attribute>
        <xsl:element name='img'>
          <xsl:attribute name='alt'><xsl:text>Trademark Logo</xsl:text></xsl:attribute>
          <xsl:attribute name='src'><xsl:value-of select="$headlogoimg"/></xsl:attribute>
          <xsl:attribute name='width'><xsl:value-of select="$headlogowidth"/></xsl:attribute>
          <xsl:attribute name='height'><xsl:value-of select="$headlogoheight"/></xsl:attribute>
        </xsl:element><!-- img -->
      </xsl:element><!-- a -->
     </th>
     <th text-align="center" width="75%">
       <xsl:element name='a'>
         <xsl:attribute name='href'>
           <xsl:value-of select="$headtitlelink"/>
         </xsl:attribute>
         <xsl:value-of select="$headtitle"/>
       </xsl:element><!-- a -->
     </th>
    </tr>
    <tr>
     <td valign="middle"><xsl:value-of select="$headsubtitle"/></td>
    </tr>
    </tbody>
    </table>

<!--  </xsl:element> -->

<!-- Useful Emulation of Project Header Buttons -->

<!-- <table class="HdrButtons"> -->

    <table class="HdrButtons" align="center" border="1">
     <tbody>
      <tr>
       <td><a href="http://www.apache.org">Apache Foundation</a></td>
       <td><a href="http://xalan.apache.org">Xalan Project</a></td>
       <td><a href="http://xerces.apache.org">Xerces Project</a></td>
       <td><a href="http://www.w3.org/TR">Web Consortium</a></td>
       <td><a href="http://www.oasis-open.org/standards">Oasis Open</a></td>
      </tr>
     </tbody>
    </table>
  </xsl:element>
<!-- -->

</xsl:template>


<xsl:template name="doLeftNav">
<xsl:param name="sectparm" select="''"/>
<xsl:element name="div">
  <xsl:attribute name="id">navLeft</xsl:attribute>
  <ul>
  <xsl:for-each select="document($bookxml)/book/*">
    <xsl:if test="name(.)!='hidden'">
    <xsl:choose>
      <xsl:when test="name(.)='external'">
        <li>
        <xsl:element name="a">
          <xsl:attribute name="href">
            <xsl:value-of select="@href"/>
          </xsl:attribute>
          <xsl:value-of select="@label"/>
        </xsl:element>
        </li>
      </xsl:when>
      <xsl:when test="name(.)='resources'">
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
      </xsl:when>
      <xsl:when test="name(.)='changes'">
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
      </xsl:when>
      <xsl:when test="name(.)='separator'">    <!--  </ul>   <hr/>      <ul> -->
        <xsl:text disable-output-escaping="yes">&lt;/ul&gt;&lt;hr /&gt;&lt;ul&gt;</xsl:text>
      </xsl:when>
      <xsl:when test="(name(.)='document') or (name(.)='faqs')">
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
      </xsl:when>


      <xsl:when test="name(.)='group'">
        <xsl:text disable-output-escaping="yes">&lt;/ul&gt;</xsl:text>
        <xsl:element name="p">
          <xsl:attribute name="class">navGroup</xsl:attribute>
          <em><xsl:value-of select="@label"/></em>
        </xsl:element>
        <xsl:text disable-output-escaping="yes">&lt;ul&gt;</xsl:text>

        <xsl:for-each select="./*"> 
          <xsl:choose>

            <xsl:when test="name(.)='external'">
              <li>
              <xsl:element name="a">
                <xsl:attribute name="href">
                  <xsl:value-of select="@href"/>
                </xsl:attribute>
                <xsl:value-of select="@label"/>
              </xsl:element>
              </li>
            </xsl:when>
            <xsl:when test="name(.)='resources'">
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
            </xsl:when>
            <xsl:when test="name(.)='changes'">
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
            </xsl:when>
            <xsl:when test="(name(.)='document') or (name(.)='faqs')">
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
            </xsl:when>


            <xsl:otherwise>
<!--
 ! Unrecognized child of Navigation <group> element -->
-->
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
<!--
 ! Unrecognized child of Navigation <book> element -->
-->
      </xsl:otherwise>
    </xsl:choose>
    </xsl:if> <!-- not hidden -->
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
    <xsl:text>Copyright &#169; 1999-2012 The Apache Software Foundation</xsl:text><br/>
    <xsl:text>Apache, Xalan, and the Feather logo are trademarks of The Apache Software Foundation</xsl:text>
    <xsl:element name="div">
      <xsl:attribute name="class">small</xsl:attribute>
      <xsl:value-of select="$releaseinfo"/>
    </xsl:element>
  </xsl:element>
</xsl:template>


<!-- 
  getPrevFaqCount returns the number of <faq> elements found in the previous <group>s 
  Used to create sequential ID numbers across <faq> groups for titles and anchors.
-->

<xsl:template name="getPrevFaqCount">
  <xsl:param name="mygroup">1</xsl:param>
  <xsl:variable name="faqcount">
    <xsl:choose>
      <xsl:when test="$mygroup &lt;= 1">0</xsl:when>
      <xsl:otherwise>
        <xsl:variable name="subcount">
          <xsl:call-template name="getPrevFaqCount">
            <xsl:with-param name="mygroup" select="$mygroup - 1"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="$subcount + count(./group[position()= ($mygroup - 1)]/faq)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:value-of select="$faqcount"/>
</xsl:template>

<!-- Recursive - Generate the grouped faq Questions section -->

<xsl:template name="groupFaqQuestions">
  <xsl:param name="mygroup">1</xsl:param>
  <xsl:if test="./group[position()=$mygroup]">
    <h3><xsl:value-of select="./group[position()=$mygroup]/@title"/></h3>
    <xsl:variable name="prevfaqs">
      <xsl:call-template name="getPrevFaqCount">
        <xsl:with-param name="mygroup"><xsl:value-of select="$mygroup"/></xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:for-each select="./group[position()=$mygroup]">

<!-- list must have at least one faq -->
      <xsl:if test="./faq[position()=1]">
        <xsl:element name="ol">
          <xsl:attribute name="start">
            <xsl:value-of select="$prevfaqs + 1"/>
          </xsl:attribute>
          <xsl:call-template name="doFaqTitles">
            <xsl:with-param name="faqNbr"><xsl:value-of select="$prevfaqs + 1"/></xsl:with-param>
          </xsl:call-template>
        </xsl:element>
      </xsl:if>
    </xsl:for-each>
    <xsl:call-template name="groupFaqQuestions">
      <xsl:with-param name="mygroup"><xsl:value-of select="$mygroup + 1"/></xsl:with-param>
    </xsl:call-template>
  </xsl:if>
</xsl:template>

<!-- Recursive - Generate the grouped faq Answers section -->

<xsl:template name="groupFaqAnswers">
  <xsl:param name="mygroup">1</xsl:param>
  <xsl:if test="./group[position()=$mygroup]">
    <xsl:variable name="prevfaqs">
      <xsl:call-template name="getPrevFaqCount">
        <xsl:with-param name="mygroup"><xsl:value-of select="$mygroup"/></xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:for-each select="./group[position()=$mygroup]">
      <xsl:if test="./faq[position()=1]">
        <xsl:call-template name="doFaqContent">
          <xsl:with-param name="faqNbr" select="$prevfaqs + 1"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:for-each>
    <xsl:call-template name="groupFaqAnswers">
      <xsl:with-param name="mygroup" select="$mygroup + 1"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template>


<!-- Updated to accommodate faq groups for XALAN-J -->

<xsl:template name="doFaqDocument">
  <h2 class="faqSubHead">Questions</h2>
  <xsl:choose>
    <xsl:when test="./group[position()=1]">
      <xsl:call-template name="groupFaqQuestions"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:if test="./faq[position()=1]">
        <ol start="1">
           <xsl:call-template name="doFaqTitles"/>
        </ol>
      </xsl:if>
    </xsl:otherwise>
  </xsl:choose>
  <h2 class="faqSubHead">Answers</h2>
  <xsl:choose>
    <xsl:when test="./group[position()=1]">
      <xsl:call-template name="groupFaqAnswers"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="doFaqContent"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="doFaqTitles">
  <xsl:param name="faqNbr">1</xsl:param>
  <xsl:param name="myNbr">1</xsl:param>
  <xsl:if test="faq[position()=$myNbr]">
    <!-- output the title reference -->
    <li>
    <xsl:element name="a">
      <xsl:attribute name="href">
        <xsl:value-of select="concat('#faq-', $faqNbr)"/>
      </xsl:attribute>
      <xsl:value-of select="faq[position()=$myNbr]/@title"/>
    </xsl:element>
    </li>
    <!-- get the next faq if any -->
    <xsl:call-template name="doFaqTitles">
      <xsl:with-param name="faqNbr" select="number($faqNbr + 1)"/>
      <xsl:with-param name="myNbr" select="number($myNbr + 1)"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template> 


<xsl:template name="doFaqContent">
  <xsl:param name="faqNbr">1</xsl:param>
  <xsl:param name="myNbr">1</xsl:param>

  <xsl:if test="faq[position()=$myNbr]">            <!-- output content of this element only -->

<!--  <h3><xsl:value-of select="faq[position()=$myNbr]/@title"/></h3>     -->

    <xsl:element name="h3">
      <xsl:attribute name="id">
        <xsl:value-of select="concat('faq-', $faqNbr)"/>
      </xsl:attribute>
      <xsl:attribute name="class">
        <xsl:text>faqTitle</xsl:text>
      </xsl:attribute>
      <xsl:value-of select="concat($faqNbr, '. ', faq[position()=$myNbr]/@title)"/>
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
      <xsl:with-param name="faqNbr" select="number($faqNbr + 1)"/>
      <xsl:with-param name="myNbr" select="number($myNbr + 1)"/>
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
    <xsl:attribute name="class">content</xsl:attribute>
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
    <xsl:attribute name="class">content</xsl:attribute>
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

<!--
<xsl:template match="note">
  <blockquote class="note"><xsl:apply-templates/></blockquote>
</xsl:template>
-->

<xsl:template match="note">
    <xsl:element name="table">
      <xsl:attribute name="class">note</xsl:attribute>
      <tr>
        <xsl:element name="td">
          <xsl:attribute name="class">noteImg</xsl:attribute>
          <xsl:element name="img">
            <xsl:attribute name="src">resources/note.gif</xsl:attribute>
            <xsl:attribute name="alt">note</xsl:attribute>
          </xsl:element>
        </xsl:element>
        <xsl:element name="td">
          <xsl:attribute name="class">noteTxt</xsl:attribute>
          <xsl:apply-templates/>
        </xsl:element>
      </tr>
    </xsl:element>
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
        select="document(concat($xmlsources,'/resources.xml'))//resource[@id=$idval]"/>
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
        select="document(concat($xmlsources,'/resources.xml'))//human-resource[@id=$idval]"/>
  <xsl:element name="a">
    <xsl:attribute name="href">
      <xsl:value-of select="concat('mailto:',$resref/@mailto)"/>
    </xsl:attribute>
    <xsl:value-of select="$resref/@name"/>
  </xsl:element>
</xsl:template>


</xsl:stylesheet>
