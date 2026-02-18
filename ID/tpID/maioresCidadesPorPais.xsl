<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html"/>
    
    <xsl:template match="/">
        <html>
            <head>
                <title>Lista de Cidades Populosas por País</title>
            </head>
            <body>
                <h1>Lista de Cidades Populosas por País</h1>
                <table border="1">
                    <tr>
                        <th>Nome do País</th>
                        <th>Cidades</th>
                    </tr>
                    <xsl:apply-templates select="//factos"/>
                </table>
            </body>
        </html>
    </xsl:template>
    
    <xsl:template match="factos">
        <tr>
            <td><xsl:value-of select="@nome"/></td>
            <td>
                <xsl:apply-templates select="cidadesPopulosas/cidade"/>
            </td>
        </tr>
    </xsl:template>
    
    <xsl:template match="cidade">
        <xsl:value-of select="."/><xsl:text>, </xsl:text>
    </xsl:template>
    
</xsl:stylesheet>
