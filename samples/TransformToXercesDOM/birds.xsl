<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" indent="yes"/>

<xsl:template match="Class">
<BirdInfo>
	<xsl:apply-templates select="*"/>
</BirdInfo>
</xsl:template>

<xsl:template match="Order">
  <xsl:element name="{@Name}">
	<xsl:apply-templates select="*"/>
  </xsl:element>
</xsl:template>

<xsl:template match="Family">
  <xsl:element name="{@Name}">
	<xsl:apply-templates select="*"/>
  </xsl:element>
</xsl:template>

<xsl:template match="Species">
	<xsl:copy-of select="."/>
</xsl:template>

</xsl:stylesheet>
