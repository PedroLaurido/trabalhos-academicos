var zona_1 = document.getElementById("zona_1");
var zona_2 = document.getElementById("zona_2");
var zona_3 = document.getElementById("zona_3");
    
var preco_zona_1 = document.getElementById("preco_zona1");
var zona1;
var zona2;
var zona3;
    
function Results(e){

    const area_util_1 = document.getElementById("area-util-im").value;
    const idade_imovel_1 = document.getElementById("idade_im").value; 


    
    let areaValor = document.getElementById('area-util-im').value;
    let idadeValor = document.getElementById('idade_im').value;
    

    if( document.getElementById("tipo-0").checked == false && document.getElementById("tipo-1").checked == false && document.getElementById("tipo-2").checked == false && document.getElementById("tipo-3").checked == false && document.getElementById("tipo-4").checked == false && document.getElementById("tipo-5").checked == false && document.getElementById("tipo-6").checked == false ||areaValor=="" || idadeValor=="" || document.getElementById("garagem_sim").checked == false  && document.getElementById("garagem_nao").checked == false|| document.getElementById("transportes_sim").checked == false  && document.getElementById("transportes_nao").checked == false){
        alert('Preencha todos os campos')
    }
    else{
    if(idade_imovel_1<=5){
        if(document.getElementById("garagem_sim").checked && document.getElementById("transportes_sim").checked){
            zona1 = ((1200*area_util_1)*1);
            zona2 = ((2000*area_util_1)*1);
            zona3 = ((2500*area_util_1)*1);
            zonas(zona1,zona2,zona3);
        }
        
        if(document.getElementById("garagem_nao").checked && document.getElementById("transportes_nao").checked){
             zona1= ((1200*area_util_1)*0.95)*0.9;
             zona2= ((2000*area_util_1)*0.95)*0.9;
             zona3= ((2500*area_util_1)*0.95)*0.9;
            zonas(zona1,zona2,zona3);

            }
        
        
        if(document.getElementById("garagem_nao").checked && document.getElementById("transportes_sim").checked){
             zona1= ((1200*area_util_1)*0.95);
             zona2= ((2000*area_util_1)*0.95);
             zona3= ((2500*area_util_1)*0.95);
            zonas(zona1,zona2,zona3);

            }

        if(document.getElementById("garagem_sim").checked && document.getElementById("transportes_nao").checked){
             zona1= ((1200*area_util_1)*0.90);
             zona2= ((2000*area_util_1)*0.90);
             zona3= ((2500*area_util_1)*0.90);
            zonas(zona1,zona2,zona3);

            }

}
    
    
    if(idade_imovel_1>5 && idade_imovel_1<=10){
        if(document.getElementById("garagem_sim").checked && document.getElementById("transportes_sim").checked){
            zona1= ((1200*area_util_1)*0.95);
             zona2= ((2000*area_util_1)*0.95);
             zona3= ((2500*area_util_1)*0.95);
            zonas(zona1,zona2,zona3);

            }
            
            if(document.getElementById("garagem_nao").checked && document.getElementById("transportes_nao").checked){
                 zona1= (((1200*area_util_1)*0.95)*0.95)*0.9;
                 zona2= (((2000*area_util_1)*0.95)*0.95)*0.9;
                 zona3= (((2500*area_util_1)*0.95)*0.95)*0.9;
                zonas(zona1,zona2,zona3);

                }
            
    
            if(document.getElementById("garagem_nao").checked && document.getElementById("transportes_sim").checked){
                 zona1= ((1200*area_util_1)*0.95)*0.95;
                 zona2= ((2000*area_util_1)*0.95)*0.95;
                 zona3= ((2500*area_util_1)*0.95)*0.95;
                zonas(zona1,zona2,zona3);

                }
    
            if(document.getElementById("garagem_sim").checked && document.getElementById("transportes_nao").checked){
                 zona1= ((1200*area_util_1)*0.95)*0.9;
                 zona2= ((2000*area_util_1)*0.95)*0.9;
                 zona3= ((2500*area_util_1)*0.95)*0.9;
                zonas(zona1,zona2,zona3);

                }}


    if(idade_imovel_1>10){
        if(document.getElementById("garagem_sim").checked && document.getElementById("transportes_sim").checked){
             zona1= ((1200*area_util_1)*0.90);
             zona2= ((2000*area_util_1)*0.90);
             zona3= ((2500*area_util_1)*0.90);
            zonas(zona1,zona2,zona3);

                
            }
            
            if(document.getElementById("garagem_nao").checked && document.getElementById("transportes_nao").checked){
                 zona1= (((1200*area_util_1)*0.90)*0.95)*0.9;
                 zona2= (((2000*area_util_1)*0.90)*0.95)*0.9;
                 zona3= (((2500*area_util_1)*0.90)*0.95)*0.9;
                zonas(zona1,zona2,zona3);

                }
            
    
            if(document.getElementById("garagem_nao").checked && document.getElementById("transportes_sim").checked){
                 zona1= ((1200*area_util_1)*0.90)*0.95;
                 zona2= ((2000*area_util_1)*0.90)*0.95;
                 zona3= ((2500*area_util_1)*0.90)*0.95;
                zonas(zona1,zona2,zona3);

                }
    
            if(document.getElementById("garagem_sim").checked && document.getElementById("transportes_nao").checked){
                 zona1= ((1200*area_util_1)*0.90)*0.9;
                 zona2= ((2000*area_util_1)*0.90)*0.9;
                 zona3= ((2500*area_util_1)*0.90)*0.9;
                zonas(zona1,zona2,zona3);

                }}
            document.getElementById('area-util-im_1').innerHTML = area_util_1 + "m<sup>2</sup>";
            document.getElementById('area-util-im_2').innerHTML = area_util_1 + "m<sup>2</sup>";
            document.getElementById('area-util-im_3').innerHTML = area_util_1 + "m<sup>2</sup>";

            var tipologia = document.querySelector('input[name="tipo"]:checked');
            document.getElementById('tipologia_1').innerHTML = tipologia.value;
            document.getElementById('tipologia_2').innerHTML = tipologia.value;
            document.getElementById('tipologia_3').innerHTML = tipologia.value;

            var garagem = document.querySelector('input[name="garagem"]:checked');
            document.getElementById('garagem_1').innerHTML = garagem.value;
            document.getElementById('garagem_2').innerHTML = garagem.value;
            document.getElementById('garagem_3').innerHTML = garagem.value;

            var transporte = document.querySelector('input[name="transportes"]:checked');
            document.getElementById('transporte_1').innerHTML = transporte.value;
            document.getElementById('transporte_2').innerHTML = transporte.value;
            document.getElementById('transporte_3').innerHTML = transporte.value;

            }    
    }

function zonas(zona1,zona2,zona3){
    document.getElementById('cartoes').style.visibility = 'visible';
    zona_1.innerHTML = zona1 + "€";
    zona_2.innerHTML = zona2 + "€";
    zona_3.innerHTML = zona3 + "€";
    
}

document.getElementById('parte_seccao-1').addEventListener('mouseover',function(){
    document.querySelector('.parte_seccao_zona1').style.visibility = 'visible';
});
document.getElementById('parte_seccao-1').addEventListener('mouseout',function(){
    document.querySelector('.parte_seccao_zona1').style.visibility = 'hidden';
});

document.getElementById('parte_seccao-2').addEventListener('mouseover',function(){
    document.querySelector('.parte_seccao_zona2').style.visibility = 'visible';
});
document.getElementById('parte_seccao-2').addEventListener('mouseout',function(){
    document.querySelector('.parte_seccao_zona2').style.visibility = 'hidden';
});


document.getElementById('parte_seccao-3').addEventListener('mouseover',function(){
    document.querySelector('.parte_seccao_zona3').style.visibility = 'visible';
});
document.getElementById('parte_seccao-3').addEventListener('mouseout',function(){
    document.querySelector('.parte_seccao_zona3').style.visibility = 'hidden';
});

  function Janela(){
    document.getElementById('parte_financiamento').style.visibility = 'visible';
    document.getElementById('cartoes').style.visibility = 'hidden';
    preco_zona_1.innerHTML ="Valor do Imóvel: " + zona1 + "€";

  }

  function Janela2(){
    document.getElementById('parte_financiamento').style.visibility = 'visible';
    document.getElementById('cartoes').style.visibility = 'hidden';
    preco_zona_1.innerHTML ="Valor do Imóvel: " + zona2 + "€";

}

function Janela3(){
    document.getElementById('parte_financiamento').style.visibility = 'visible';
    document.getElementById('cartoes').style.visibility = 'hidden';
    preco_zona_1.innerHTML ="Valor do Imóvel: " + zona3 + "€";

}

  
  function financ_calc(){
      let empVal = document.getElementById('emprestimo').value;
      let entrVal = document.getElementById('entrada_inicial').value;
      let prazoVal = document.getElementById('prazo').value;
      var x=0;
        if(empVal=="" || entrVal=="" || prazoVal==""){
            alert('Preencha todos os campos')
        }
        else{
            let valor_emp = document.getElementById('emprestimo').value; 
            if(empVal>zona1 || empVal>zona2 || empVal>zona3){
                alert('O valor do empréstimo excede o valor da casa')
            }
            else{
                if(entrVal>=empVal){
                alert('A entrada inicial é maior ou igual do que o valor do empréstimo')
            }

            
            else{

                        let min = 1.00;
                        let max = 5.00;
                        let RandomNumber1 = (Math.random() * (max - min) + min);
                        let RandomNumber2 = (Math.random() * (max - min) + min);
                        let RandomNumber3 = (Math.random() * (max - min) + min);

                        let taxa_juro = 0.5;
                        
                        let taxa_juro_spread1 =  RandomNumber1 + taxa_juro ;
                        let taxa_juro_spread2 =  RandomNumber2 + taxa_juro ;
                        let taxa_juro_spread3 =  RandomNumber3 + taxa_juro ;
                        
                        let taxafinal1 = Math.round(taxa_juro_spread1*100)/100;
                        let taxafinal2 = Math.round(taxa_juro_spread2*100)/100;
                        let taxafinal3 = Math.round(taxa_juro_spread3*100)/100;


                    
                        let anos1 = document.getElementById('prazo').value;
                        let entrada = document.getElementById('entrada_inicial').value;
                        
                        let convertmeses=anos1*12;
                        
                        let valor_mensal1 = (+valor_emp - -entrada)*(taxafinal1/100);
                        let valor_mensal11 = +valor_mensal1 + +(valor_emp-entrada);
                        let valor_final1 = valor_mensal11/convertmeses;

                        let valor_mensal2 = (+valor_emp - -entrada)*(taxafinal2/100);
                        let valor_mensal22 = +valor_mensal2 + +(valor_emp-entrada);
                        let valor_final2 = valor_mensal22/convertmeses;

                        let valor_mensal3 = (+valor_emp - -entrada)*(taxafinal3/100);
                        let valor_mensal33 = +valor_mensal3 + +(valor_emp-entrada);
                        let valor_final3 = valor_mensal33/convertmeses;

                        
                        valor_final_mensal1.innerHTML =  valor_final1.toFixed(2) +" €";
                        valor_spread1.innerHTML= RandomNumber1.toFixed(2) + " %";
                        valor_taxa1.innerHTML = taxa_juro + " %";

                        valor_final_mensal2.innerHTML =  valor_final2.toFixed(2) +" €";
                        valor_spread2.innerHTML= RandomNumber2.toFixed(2) + " %";
                        valor_taxa2.innerHTML = taxa_juro + " %";

                        valor_final_mensal3.innerHTML =  valor_final3.toFixed(2) +" €";
                        valor_spread3.innerHTML= RandomNumber3.toFixed(2) + " %";
                        valor_taxa3.innerHTML = taxa_juro + " %";
                    
                        document.querySelector('.cartao_santander').style.visibility = 'visible';
                        document.querySelector('.cartao_BPI').style.visibility = 'visible';
                        document.querySelector('.cartao_BIC').style.visibility = 'visible';

                }
             }
        }

  }

  function CloseFinanc(){
    document.getElementById('cartoes').style.visibility = 'visible';
    document.getElementById('cartoes').style.visibility = 'visible';
    document.getElementById('parte_financiamento').style.visibility = 'hidden';
    document.querySelector('.cartao_santander').style.visibility = 'hidden';
    document.querySelector('.cartao_BPI').style.visibility = 'hidden';
    document.querySelector('.cartao_BIC').style.visibility = 'hidden';
    document.getElementById('emprestimo').value="";
    document.getElementById('entrada_inicial').value="";
    document.getElementById('prazo').value="";
    
}