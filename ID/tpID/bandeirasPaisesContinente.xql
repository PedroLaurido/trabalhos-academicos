xquery version "1.0";

declare namespace xsi="http://www.w3.org/2001/XMLSchema-instance";

let $input := doc("input.xml")//input
let $continent := $input/text()

return
<html>
    <head>
        <title>Bandeiras dos paises do continente "{$continent}"</title>
    </head>
    <body>
        <h1>Bandeiras dos paises do continente "{$continent}"</h1>
        <div class="flags">
            {
                for $pais in doc("paises.xml")//pais[continente = $continent]
                return
                    let $bandeira := $pais/link_imagem
                    return
                        <img src="{$bandeira}" alt="Bandeira de {$pais/@nome}"/>
            }
        </div>
    </body>
</html>

