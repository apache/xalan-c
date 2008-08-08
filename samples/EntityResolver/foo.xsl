<?xml version="1.0"?> 
<!DOCTYPE xsl:stylesheet [
  <!ENTITY apply-templates SYSTEM "apply-templates.xml">
]>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:variable name="doc" select="document('document.xml')" />
  <xsl:template match="doc">
    <doc>
        <child>&apply-templates;</child>
        <child><xsl:value-of select="$doc" /></child>
    </doc>
  </xsl:template>

</xsl:stylesheet>


