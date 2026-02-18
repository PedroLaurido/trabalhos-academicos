/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 */

package com.mycompany.tpid;

import java.awt.Frame;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import net.sf.saxon.s9api.SaxonApiException;
import net.sf.saxon.s9api.XdmValue;
import org.jdom2.Document;

/**
 *
 * @author Pedro Pereira
 */
public class TpID {

    public static void main(String[] args) throws IOException, SaxonApiException {
        
        FormMenu menu = new FormMenu();
        menu.setVisible(true);
        
        String link = "https://en.wikipedia.org/wiki/";
        String pesquisa ="";
        HttpRequestFunctions.httpRequest1(link, pesquisa, "paises.html");
        
    }
    
    public static Pais criaPais(String pais) throws IOException{
        
        String nomePais = WrappersTP.obtem_nome_pais(pais);
        String codigoISO = WrappersTP.obtem_iso_pais(pais);
        String continente = WrappersTP.obtem_continente_pais(pais);
        String nomeGovernante = WrappersTP.obtem_nomeGovernante_pais(pais);
        String linkBandeira = WrappersTP.obtem_linkBandeira_pais(pais);
        
        Pais a = new Pais(nomePais, codigoISO, continente, nomeGovernante, linkBandeira/*, factos*/);
        return a;
    }
    
    public static Factos criaFactos(String pais) throws IOException{
        String nomePais = WrappersTP.obtem_nome_pais(pais);
        String codigoISO = WrappersTP.obtem_iso_pais(pais);
        String moeda = WrappersTP.obtem_moeda_pais(pais);
        String capital = WrappersTP.obtem_capital_pais(pais);
        int populacao = WrappersTP.obtem_populacao_pais(pais);
        String areaKM2 = WrappersTP.obtem_areaKM2_pais(pais);
        String crescimentoPop = WrappersTP.obtem_crescimentoPopulacao_pais(pais);
        ArrayList<String> cidadesMaisPop = WrappersTP.obtem_cidadesPopulosas_pais(pais);
        String dominio = WrappersTP.obtem_dominioInternet_pais(pais);
        String idiomasOficiais = WrappersTP.obtem_idiomasOficiais_pais(pais);
        ArrayList<String> paisesVizinhos = WrappersTP.obtem_paisesVizinhos_pais(pais);
        
        Factos b = new Factos(nomePais, codigoISO, capital, moeda, dominio,
                populacao, areaKM2, crescimentoPop, cidadesMaisPop, 
                idiomasOficiais, paisesVizinhos);
        return b;
    }
    
}
