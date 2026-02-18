xquery version "1.0";

<lista>
{
    for $pais in doc("paises.xml")//pais
    let $facto := doc("factos.xml")//facto[nome = $pais/nome]
    return
        <pais nome="{data($pais/nome)}">
            <codigo_ISO>{data($pais/codigo_ISO)}</codigo_ISO>
            <continente>{data($pais/continente)}</continente>
            <nome_governante>{data($facto/nome_governante)}</nome_governante>
            <link_imagem>{data($facto/link_imagem)}</link_imagem>
            <capital>{data($facto/capital)}</capital>
            <moeda>{data($facto/moeda)}</moeda>
            <dominio>{data($facto/dominio)}</dominio>
            <populacao>{data($facto/populacao)}</populacao>
            <area>{data($facto/area)}</area>
            <crescimentoPop>{data($facto/crescimentoPop)}</crescimentoPop>
            <cidadesPopulosas>
                {for $cidade in $facto/cidadesPopulosas/cidade
                return <cidade>{data($cidade)}</cidade>}
            </cidadesPopulosas>
            <idiomas>{data($facto/idiomas)}</idiomas>
            <paisesVizinhos>
                {for $vizinho in $facto/paisesVizinhos/vizinho
                return <vizinho>{data($vizinho)}</vizinho>}
            </paisesVizinhos>
        </pais>
}
</lista>