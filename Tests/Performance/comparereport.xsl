<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"> 
	<xsl:output method="html"/>
	<xsl:param name="baseline" select="ResultReport_latest.xml"/>
	<xsl:param name="threshold" select="5"/>

	<xsl:template match="/">
		<html>
			<title>Comparison Result Report</title>
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
				<td colspan='12'> </td>
			</tr>
			<tr>
			<xsl:variable name="ss" select="@stylesheet"/>
			<xsl:variable name="baselineTestCase" select="document($baseline)/resultsfile/testfile/testcase[attribute::stylesheet = $ss]"/> 
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
				<td>Baseline</td>
				<td><xsl:value-of select="$baselineTestCase/@verify"/></td>
				<td><xsl:value-of select="$baselineTestCase/@result"/></td>
				<td><xsl:value-of select="$baselineTestCase/@compile-xsl"/></td>
				<td><xsl:value-of select="$baselineTestCase/@num-iterations"/></td>
				<td><xsl:value-of select="$baselineTestCase/@elapsed-time"/></td>
				<td><xsl:value-of select="$baselineTestCase/@min-parse-input"/></td>
				<td><xsl:value-of select="$baselineTestCase/@avg-parse-input"/></td>
				<td><xsl:value-of select="$baselineTestCase/@max-parse-input"/></td>
				<td><xsl:value-of select="$baselineTestCase/@min-transform"/></td>
				<td><xsl:value-of select="$baselineTestCase/@avg-transform"/></td>
				<td><xsl:value-of select="$baselineTestCase/@max-transform"/></td>
			</tr>
			<tr>
				<xsl:variable name="ss" select="@stylesheet"/>
				<xsl:variable name="baselineTestCase" select="document($baseline)/resultsfile/testfile/testcase[attribute::stylesheet = $ss]"/> 
				<td>Difference:</td>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@verify"/>
					<xsl:with-param name="new" select="@verify"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@result"/>
					<xsl:with-param name="new" select="@result"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@compile-xsl"/>
					<xsl:with-param name="new" select="@compile-xsl"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="new" select="$baselineTestCase/@num-iterations"/>
					<xsl:with-param name="orig" select="@num-iterations"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@elapsed-time"/>
					<xsl:with-param name="new" select="@elapsed-time"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@min-parse-input"/>
					<xsl:with-param name="new" select="@min-parse-input"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@avg-parse-input"/>
					<xsl:with-param name="new" select="@avg-parse-input"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@max-parse-input"/>
					<xsl:with-param name="new" select="@max-parse-input"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@min-transform"/>
					<xsl:with-param name="new" select="@min-transform"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@avg-transform"/>
					<xsl:with-param name="new" select="@avg-transform"/>
				</xsl:call-template>
				<xsl:call-template name="HighlightChange">
					<xsl:with-param name="orig" select="$baselineTestCase/@max-transform"/>
					<xsl:with-param name="new" select="@max-transform"/>
				</xsl:call-template>
			</tr>
	</xsl:template>

	
	
	<xsl:template name="HighlightChange">
		<xsl:param name="orig"/>
		<xsl:param name="new"/>

		<xsl:choose>
			<xsl:when test="number($orig) or $orig = 0">

				<xsl:variable name="n-orig">
					<xsl:choose>
						<xsl:when test="$orig= 0">
							<xsl:value-of select="1"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$orig"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
			
				<xsl:variable name="n-new">
					<xsl:choose>
						<xsl:when test="$new= 0">
							<xsl:value-of select="1"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$new"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="$n-new > $n-orig">
						<xsl:variable name="difference" select="(($n-new div $n-orig) - 1) * 100"/>
						<xsl:choose>
							<xsl:when test="$difference = 0">
								<td align="center">--</td>
							</xsl:when>
							<xsl:when test="$difference > $threshold">
								<td bgcolor="red">
									<xsl:value-of select="concat($difference,'%')"/>
								</td>	
							</xsl:when>
							<xsl:otherwise>
								<td>
									<xsl:value-of select="concat($difference,'%')"/>
								</td>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name="difference" select="(1 - ($n-new div $n-orig)) * 100"/>
						<xsl:choose>
							<xsl:when test="$difference = 0">
								<td align="center">--</td>
							</xsl:when>
							<xsl:when test="$difference > $threshold">
								<td bgcolor="#10F0F0">
									<xsl:value-of select="concat($difference,'%')"/>
								</td>
							</xsl:when>
							<xsl:otherwise>
								<td>
									<xsl:value-of select="concat($difference,'%')"/>
								</td>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>		
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="not($orig) and not($new)">
						<td></td>
					</xsl:when>
					<xsl:when test="$orig = $new">
						<td>Same</td>
					</xsl:when>
					<xsl:otherwise>
						<td bgcolor="red">Diff</td>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>

