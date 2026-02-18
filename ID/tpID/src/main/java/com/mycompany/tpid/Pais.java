/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.tpid;

import java.util.ArrayList;

/**
 *
 * @author Pedro Pereira
 */
public class Pais {
    String codigoISO, nome, continente, nomeGovernante, linkImagem;
    /*ArrayList<String> factos;*/
    
    public Pais(String nome, String codigoISO, String continente, String nomeGovernante, String linkImagem/*, ArrayList<String> factos*/) {
        this.nome = nome;
        this.codigoISO = codigoISO;
        this.continente = continente;
        this.nomeGovernante = nomeGovernante;
        this.linkImagem = linkImagem;
        //this.factos = factos;
    }

    public void setCodigoISO(String codigoISO) {
        this.codigoISO = codigoISO;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }
    
    public void setContinente(String continente) {
        this.continente = continente;
    }
    
    public void setNomeGovernante(String nomeGovernante) {
        this.nomeGovernante = nomeGovernante;
    }
    
    public void setLinkImagem(String linkImagem) {
        this.linkImagem = linkImagem;
    }
    
    /*public void setFactos(ArrayList<String> factos) {
        this.factos = factos;
    }*/
   
    public String getCodigoISO() {
        return codigoISO;
    }

    public String getNome() {
        return nome;
    }
    
    public String getContinente() {
        return continente;
    }

    public String getNomeGovernante() {
        return nomeGovernante;
    }
    
    public String getLinkImagem() {
        return linkImagem;
    }
    
    /*public ArrayList<String> getFactos() {
        return factos;
    }*/
}
