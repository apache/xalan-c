<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:output method="html"/>
<xsl:param name="testfile" select="'No_file'"/>

<xsl:variable name="Results" select="document($testfile)"/>
<xsl:variable name="drive" select="document($testfile)/resultsfile/testfile/RunResults/@BaseDrive"/>

<xsl:template match="/">
<html>
 <head>
  <base href="{$drive}:"/>
 </head>

  <xsl:apply-templates select="$Results/resultsfile/testfile/RunResults"/>

  <TABLE frame="box" border="1" rules="groups" width="95%" cellspacing="2" cellpadding="5">
  <CAPTION align="center"><b><xsl:text>Failed Cases:</xsl:text></b></CAPTION>
    <!-- fake row to establish widths -->
    <TR><TD width="20%"></TD><TD width="80%"></TD></TR>
    <xsl:apply-templates select="$Results/resultsfile/testfile/Test_Dir"/>
  </TABLE>

  <TABLE frame="box" border="1" rules="groups" width="95%" cellspacing="2" cellpadding="5">
  <CAPTION align="center"><b>The following testcases were missing gold files:</b></CAPTION>
    <!-- fake row to establish widths -->
    <TR><TD width="20%"></TD><TD width="80%"></TD></TR>
    <xsl:apply-templates select="$Results/resultsfile/testfile/Test_Dir" mode="ambg"/>
  </TABLE>

</html>
</xsl:template>

<xsl:template match="RunResults">
	<TABLE frame="box" border="1" rules="groups" width="95%" cellspacing="2" cellpadding="5"> 
	<CAPTION align="center" fontsize="15"><b>C++ Test Results</b></CAPTION>
	  <tr>
		<!-- td rowspan="1" colspan="1"></td -->
		<th align="center">RunID</th>
		<th align="center">Xerces</th>
		<th align="center">Pass</th>
		<th align="center">Fail</th>
		<th align="center">Missing Gold</th>
  	  </tr>
	<tr>
	<td align="center"><b><xsl:value-of select="@UniqRunid"/></b></td>
	<td align="center"><b><xsl:value-of select="@Xerces-Version"/></b></td>
	<td align="center"><b><xsl:value-of select="@Passed"/></b></td>
	<td align="center" bgcolor="red"><b><xsl:value-of select="@Failed"/></b></td>
	<td align="center"><b><xsl:value-of select="@No_Gold_Files"/></b></td>
	</tr>
	</TABLE>
</xsl:template>

<xsl:template match="Test_Dir">
	<xsl:for-each select="Testcase">
		<xsl:if test="@result='FAIL'">
			<tr>
			<td bgcolor="red"><xsl:value-of select="@desc"/></td>
			<td><xsl:value-of select="@reason"/></td>
			</tr>
			<tr>
			<td align="center">Actual:</td>
			<td><xsl:value-of select="act"/></td>
			</tr>
			<tr>
			<td align="center">Expected:</td>
			<td><xsl:value-of select="exp"/></td>
			</tr>
			<tr>
			<td align="center">links</td>
			<td><a href="{xml}" target="new">xml,  </a><a href="{xsl}" target="new">xsl,  </a>
				<a href="{result}" target="new">result,  </a><a href="{gold}" target="new">gold</a></td>
			</tr>
		</xsl:if>
	</xsl:for-each>
</xsl:template>


<xsl:template match="Test_Dir" mode="ambg">
	<xsl:for-each select="Testcase">
		<xsl:if test="@result='AMBG'">
		  <tr>
			<td><xsl:value-of select="@desc"/></td>
		  </tr>
		</xsl:if>
	</xsl:for-each>
</xsl:template>

</xsl:stylesheet>