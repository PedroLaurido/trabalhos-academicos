<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" indent="yes"/>
  
  <xsl:key name="continent" match="pais" use="continente"/>

  <xsl:template match="/lista">
    <html>
      <head>
        <title>Lista de Países por Continente</title>
      </head>
      <body>
        <xsl:apply-templates select="pais[generate-id() = generate-id(key('continent', continente)[1])]"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="pais">
    <xsl:variable name="continent" select="continente"/>
    <h2><xsl:value-of select="$continent"/></h2>
    <ul>
      <xsl:for-each select="key('continent', $continent)">
        <li>
          <xsl:value-of select="@nome"/>
        </li>
      </xsl:for-each>
    </ul>
  </xsl:template>

</xsl:stylesheet>
