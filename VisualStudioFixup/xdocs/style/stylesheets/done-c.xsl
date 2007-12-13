<?xml version="1.0" encoding="ISO-8859-1" ?>

<!DOCTYPE xsl:stylesheet>

<!-- XSL Style sheet, DTD omitted -->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml"/>
  
  <xsl:template match="Commits">
  <xsl:comment>This XML fragment contains a list of source code and sample updates to place in an &lt;s3&gt; section of readme.xml</xsl:comment>
    <xsl:if test="count(Commit[@category='core'])>0">
      <p>Source code updates:</p>
      <ul>
      <xsl:for-each select="Commit[@category='core']">
        <li><xsl:apply-templates select="Who|DateCommitted|Modified|Added|Removed|Log"/></li>
      </xsl:for-each>
      </ul>
    </xsl:if>
    <xsl:if test="count(Commit[@category='core'])=0">
      <note>This release includes no source code updates.</note>
    </xsl:if>
    <xsl:if test="count(Commit[@category='samples'])>0">
      <p>Samples updates:</p>
      <ul>
      <xsl:for-each select="Commit[@category='samples']">
        <li><xsl:apply-templates select="Who|DateCommitted|Modified|Added|Removed|Log"/></li>
      </xsl:for-each>
      </ul>
    </xsl:if>
    <xsl:if test="count(Commit[@category='samples'])=0">
      <note>This release includes no updates of the samples.</note>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="Who">
    <ref>Committed by </ref><xsl:value-of select="."/>
  </xsl:template>
  <xsl:template match="DateCommitted">
    <ref> on </ref><xsl:value-of select="."/><br/>
  </xsl:template>    
  <xsl:template match="Modified">    
    <ref>Modified: </ref><xsl:value-of select="."/><br/>
  </xsl:template>    
  <xsl:template match="Added">    
    <ref>Added: </ref><xsl:value-of select="."/><br/>
  </xsl:template>    
  <xsl:template match="Removed">    
    <ref>Removed: </ref><xsl:value-of select="."/><br/>
  </xsl:template>    
    <xsl:template match="Log">    
    <ref>Committer's log entry: </ref><xsl:value-of select="."/><br/><br/>
  </xsl:template>
  
</xsl:stylesheet>