<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <top5_paises>
      <xsl:for-each select="lista/factos">
        <xsl:sort select="populacao" data-type="number" order="descending"/>
        <xsl:if test="position() &lt;= 5">
          <xsl:copy-of select="."/>
        </xsl:if>
      </xsl:for-each>
    </top5_paises>
  </xsl:template>
</xsl:stylesheet>
