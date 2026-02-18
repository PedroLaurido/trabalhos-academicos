xquery version "1.0";

<resultados>
{
    for $x in doc("paises.xml")/lista/pais
    order by $x/@nome
    return (
        "Nome: ", data($x/@nome), "&#10;"
    )
}
</resultados>
