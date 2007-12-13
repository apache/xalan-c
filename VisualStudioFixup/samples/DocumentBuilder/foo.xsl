<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template match="foo">
    <out><xsl:apply-templates select="*"/></out>
  </xsl:template>

  <xsl:template match="foobar">
    <xsl:copy-of select="."/>
  </xsl:template>

</xsl:stylesheet>
