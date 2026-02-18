<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html" encoding="UTF-8"/>

    <xsl:template match="/">
        <html>
            <head>
                <title>Lista de Países com População e Área</title>
            </head>
            <body>
                <h1>Lista de Países com População e Área</h1>
                <table border="1">
                    <tr>
                        <th>Nome do País</th>
                        <th>População</th>
                        <th>Área (km²)</th>
                        <th>Densidade Populacional (pessoas/km²)</th>
                    </tr>
                    <xsl:for-each select="//factos">
                        <tr>
                            <td><xsl:value-of select="@nome"/></td>
                            <td><xsl:value-of select="populacao"/></td>
                            <td>
                                <xsl:value-of select="translate(area, ',', '')"/>
                            </td>
                            <td>
                                <xsl:variable name="populacao" select="number(populacao)"/>
                                <xsl:variable name="area" select="number(translate(area, ',', ''))"/>
                                <xsl:value-of select="format-number($populacao div $area, '#.##')"/>
                            </td>
                        </tr>
                    </xsl:for-each>
                </table>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
