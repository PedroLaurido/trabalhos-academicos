xquery version "1.0";


<juntar>
{
for $x in doc("C:/Users/Pedro%20Pereira/Documents/NetBeansProjects/Ficha4_P1/escritores.xml")//escritor
	for $y in doc("C:/Users/Pedro%20Pereira/Documents/NetBeansProjects/Ficha4_P1/escritores_b.xml")//escritor
let $stock := number($y/@nome)                               : o number é para forçar a strng/variavel como numero:
where $x/@nome = $y
order by $stock
return if($stock < 10) then <autor nome="{$x/@nome}"stock = "{$stock}" alerta="sim"/>
	   else then <autor nome="{$x/@nome}"stock = "{$stock}" alerta="sim"/>
}
</juntar>
