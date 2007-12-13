<?xml version="1.0"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes"/>
    
  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="s1">
    <html>
      <head><title><xsl:value-of select="@title"/></title></head>
      <body  bgcolor="#ffffff" text="#000000">
        <xsl:apply-templates select="s2"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="s2">
    <table width="100%" border="0" cellspacing="0" cellpadding="4">
      <tr>
        <td bgcolor="#006699">
          <font color="#ffffff" size="+1">
            <b><xsl:value-of select="@title"/></b>
          </font>
        </td>
      </tr>
    </table>
    <xsl:apply-templates/>
    <br/>
  </xsl:template>

  <xsl:template match="p">
    <p><xsl:apply-templates/></p>
  </xsl:template>

</xsl:stylesheet>