<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
				xmlns:external="http://ExternalFunction.xalan-c++.xml.apache.org"
        exclude-result-prefixes="external">
  
  <xsl:template match="/">
    <out>
       <xsl:apply-templates/>
     </out>  
  </xsl:template> 
  
  <xsl:template match="//area">    
    <given>
      The area of each face of the cube is <xsl:value-of select="@value"/>
      <xsl:text> square units </xsl:text> <xsl:value-of select="@units"/>. 
    </given>    
    <result>
      Accordingly, the length of each side is <xsl:value-of select="external:square-root(@value)"/>
      <xsl:text> </xsl:text><xsl:value-of select="@units"/> and the volume of the cube is 
      <xsl:value-of select="external:cube(external:square-root(@value))"/>
      <xsl:text> cubic </xsl:text> <xsl:value-of select="@units"/>.
    </result>
  </xsl:template>
      
  <xsl:template match="now">       
    <when>
      Date and time when we figured all this out: <xsl:value-of select="asctime()"/>.
    </when>
  </xsl:template>
    
</xsl:stylesheet>


