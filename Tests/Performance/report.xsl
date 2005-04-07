<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"> 
	<xsl:output method="html"/>
	<xsl:template match="/">
		<html>
			<title>Result Report</title>
			<body>
				<xsl:apply-templates/>
			</body>		
		</html>
	</xsl:template>
	
	<xsl:template match="resultsfile">
		<table border="1">
			<tr>
				<td>
					<b>Filename:</b>
				</td>
				<td>
					<xsl:value-of select="@filename"/>
				</td>
			</tr>
			<tr>
				<td>
					<b>Description:</b>
				</td>
				<td>
					<xsl:value-of select="testfile/@desc"/>
				</td>
			</tr>
			<tr>
				<td>
					<b>Time:</b>
				</td>
				<td>
					<xsl:value-of select="testfile/@time"/>
				</td>
			</tr>
		</table>
		<P/>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="testfile">
		<table border='1'>
			<tr>
				<td><b>Stylesheet</b></td>
				<td><b>Verify</b></td>
				<td><b>Result</b></td>
				<td><b>Time to compile</b></td>
				<td><b>Iterations</b></td>
				<td><b>Total transform time</b></td>
				<td><b>Min. parse input</b></td>
				<td><b>Avg. parse input</b></td>
				<td><b>Max. parse input</b></td>
				<td><b>Min. transform</b></td>
				<td><b>Avg. transform</b></td>
				<td><b>Max. transform</b></td>
			</tr>

			<xsl:apply-templates/>
		</table>
	</xsl:template>

	<xsl:template match="testcase">
		<tr>
			<td><xsl:value-of select="@stylesheet"/></td>
			<td><xsl:value-of select="@verify"/></td>
			<td><xsl:value-of select="@result"/></td>
			<td><xsl:value-of select="@compile-xsl"/></td>
			<td><xsl:value-of select="@num-iterations"/></td>
			<td><xsl:value-of select="@elapsed-time"/></td>
			<td><xsl:value-of select="@min-parse-input"/></td>
			<td><xsl:value-of select="@avg-parse-input"/></td>
			<td><xsl:value-of select="@max-parse-input"/></td>
			<td><xsl:value-of select="@min-transform"/></td>
			<td><xsl:value-of select="@avg-transform"/></td>
			<td><xsl:value-of select="@max-transform"/></td>
		</tr>
	</xsl:template>

</xsl:stylesheet>

		
		
		
