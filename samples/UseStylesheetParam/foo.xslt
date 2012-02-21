<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:param name="parmA"/>
<xsl:param name="parmB"/>
<xsl:param name="stringA"/>
<xsl:param name="numberA" select="0.0"/>
<xsl:template match="root">
 <OUTPUT>
 <NumberParam><xsl:value-of select="$numberA"/></NumberParam>
 <xsl:copy-of select="."/>
 <ParameterA><xsl:copy-of select="$parmA"/></ParameterA>
 <ParameterB><xsl:copy-of select="$parmB"/></ParameterB>
 <StringA><xsl:value-of select="$stringA"/></StringA>
 </OUTPUT>
</xsl:template>
</xsl:stylesheet>
