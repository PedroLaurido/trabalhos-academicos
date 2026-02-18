<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html"/>
    <xsl:template match="/">
        <html>
        <head>
            <title>Paises e Bandeiras</title>
        </head>
        <body>
            <h1>Paises e respetivas Bandeiras</h1>
            <table border="1">
                <tr>
                    <th>Nome do País</th>
                    <th>Bandeira</th>
                </tr>
                <xsl:for-each select="//pais">
                    <tr>
                        <td><xsl:value-of select="@nome"/></td>
                        <td><img src="{link_imagem}" alt="{@nome}" width="100"/></td>
                    </tr>
                </xsl:for-each>
            </table>
        </body>
        </html>
    </xsl:template>

</xsl:stylesheet>
