<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
				xmlns:external="http://ExternalFunction.xalan-c++.xml.apache.org">
  <xsl:template match="doc">
    <out><xsl:value-of select="external:cube(external:square-root(9))"/></out>
	<xsl:text>
</xsl:text>
    <out><xsl:value-of select="asctime()"/></out>
  </xsl:template>
</xsl:stylesheet>
