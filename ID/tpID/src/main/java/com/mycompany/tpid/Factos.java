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
public class Factos {
    
    String codigoISO, nome, capital, moeda, dominioInternet, idiomasOficiais, areaKm2, crescimentoPopulacional/*, populacao*/;
    int populacao;
    /*double populacao, areaKm2, crescimentoPopulacional;*/
    ArrayList<String> maioresCidades, paisesVizinhos/*, idiomasOficiais,*/;
    
    public Factos(String nome, String codigoISO, String capital, String moeda, String dominioInternet,
                /*String*/ int populacao, String areaKm2, String crescimentoPopulacional,
                ArrayList<String> maioresCidades, /*ArrayList<String>*/ String idiomasOficiais,
                ArrayList<String> paisesVizinhos) {
        this.codigoISO = codigoISO;
        this.nome = nome;
        this.capital = capital;
        this.moeda = moeda;
        this.dominioInternet = dominioInternet;
        this.populacao = populacao;
        this.areaKm2 = areaKm2;
        this.crescimentoPopulacional = crescimentoPopulacional;
        this.maioresCidades = maioresCidades;
        this.idiomasOficiais = idiomasOficiais;
        this.paisesVizinhos = paisesVizinhos;
    }

    public String getCodigoISO() {
        return codigoISO;
    }

    public void setCodigoISO(String codigoISO) {
        this.codigoISO = codigoISO;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public String getCapital() {
        return capital;
    }

    public void setCapital(String capital) {
        this.capital = capital;
    }

    public String getMoeda() {
        return moeda;
    }

    public void setMoeda(String moeda) {
        this.moeda = moeda;
    }

    public String getDominioInternet() {
        return dominioInternet;
    }

    public void setDominioInternet(String dominioInternet) {
        this.dominioInternet = dominioInternet;
    }
    
    public int getPopulacao() {
        return populacao;
    }

    public void setPopulacao(int populacao) {
        this.populacao = populacao;
    }

    public String getAreaKm2() {
        return areaKm2;
    }

    public void setAreaKm2(String areaKm2) {
        this.areaKm2 = areaKm2;
    }

    public String getCrescimentoPopulacional() {
        return crescimentoPopulacional;
    }

    public void setCrescimentoPopulacional(String crescimentoPopulacional) {
        this.crescimentoPopulacional = crescimentoPopulacional;
    }

    public ArrayList<String> getMaioresCidades() {
        return maioresCidades;
    }

    public void setMaioresCidades(ArrayList<String> maioresCidades) {
        this.maioresCidades = maioresCidades;
    }

    public /*ArrayList<String>*/String getIdiomasOficiais() {
        return idiomasOficiais;
    }

    public void setIdiomasOficiais(/*ArrayList<String>*/String idiomasOficiais) {
        this.idiomasOficiais = idiomasOficiais;
    }

    public ArrayList<String> getPaisesVizinhos() {
        return paisesVizinhos;
    }

    public void setPaisesVizinhos(ArrayList<String> paisesVizinhos) {
        this.paisesVizinhos = paisesVizinhos;
    }
}
