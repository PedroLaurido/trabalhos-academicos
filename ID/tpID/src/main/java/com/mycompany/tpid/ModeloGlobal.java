/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.tpid;

import java.util.List;
import org.jdom2.Attribute;
import org.jdom2.Document;
import org.jdom2.Element;

/**
 *
 * @author Pedro Pereira
 */
public class ModeloGlobal {
    public static Document adicionaPais(Pais elem, Document doc) {
        Element raiz;
        if (doc == null) {
            raiz = new Element("lista");
            doc = new Document(raiz);
        } else {
            raiz = doc.getRootElement();
        }
        Element pai = new Element("pais");
        Attribute a = new Attribute("nome", elem.getNome());
        pai.setAttribute(a);
        
        Element x = new Element("codigo_ISO").addContent(elem.getCodigoISO());
        pai.addContent(x);

        x = new Element("continente").addContent(elem.getContinente());
        pai.addContent(x);

        x = new Element("nome_governante").addContent(elem.getNomeGovernante());
        pai.addContent(x);

        x = new Element("link_imagem").addContent(elem.getLinkImagem());
        pai.addContent(x);
        
        raiz.addContent(pai);
        return doc;
    }
    
    public static Document removePais(String procura, Document doc) {
        Element raiz;
        if (doc == null) {
            System.out.println("Ficheiro nao existe - nao dá para remover informação");
            return null;
        } else {
            raiz = doc.getRootElement();
        }
        List todos = raiz.getChildren("pais");
        boolean found = false;
        for (int i = 0; i < todos.size(); i++) {
            Element esc = (Element) todos.get(i); //obtem pais i da Lista 
            if (esc.getAttributeValue("nome").contains(procura)) {
                esc.getParent().removeContent(esc);
                System.out.println("pais removido com sucesso!");
                found = true;
            }
        }
        if (!found) {
            System.out.println("país " + procura + " não foi encontrado");
            return null;
        }
        return doc;
    }
    
    public static Document alteraContinente(String pais, String novoContinente, Document doc) {
        Element raiz;
        if (doc == null) {
            System.out.println("Ficheiro nao existe - nao dá para alterar informação");
            return null;
        } else {
            raiz = doc.getRootElement();
        }
        List todos = raiz.getChildren("factos");
        boolean found = false;
        for (int i = 0; i < todos.size(); i++) {
            Element factos = (Element) todos.get(i); // obtém o elemento factos i da Lista 
            if (factos.getAttributeValue("nome").equals(pais)) {
                Element continenteElement = factos.getChild("continente");
                if (continenteElement != null) {
                    String continente = continenteElement.getText();
                    System.out.println("Continente " + continente);
                    continenteElement.setText(novoContinente);
                    System.out.println("Continente alterado com sucesso!");
                    found = true;
                } else {
                    System.out.println("Continente não encontrado para o país " + pais);
                }
            }
        }

        if (!found) {
            System.out.println("Pais " + pais + " não foi encontrado");
            return null;
        }
        return doc;
    }

    
    public static Document adicionaFactos(Factos elem, Document doc) {
        Element raiz;
        if (doc == null) {
            raiz = new Element("lista");
            doc = new Document(raiz);
        } else {
            raiz = doc.getRootElement();
        }
        Element pai = new Element("factos");
        Attribute a = new Attribute("nome", elem.getNome());
        pai.setAttribute(a);

        Element x = new Element("codigo_ISO").addContent(elem.getCodigoISO());
        pai.addContent(x);
        
        x = new Element("capital").addContent(elem.getCapital());
        pai.addContent(x);

        x = new Element("moeda").addContent(elem.getMoeda());
        pai.addContent(x);

        x = new Element("dominio").addContent(elem.getDominioInternet());
        pai.addContent(x);
        
        /*x = new Element("populacao").addContent(elem.getPopulacao());
        pai.addContent(x);*/
        x = new Element("populacao").addContent(String.valueOf(elem.getPopulacao())); // Converter int para String
        pai.addContent(x);
        
        x = new Element("area").addContent(elem.getAreaKm2());
        pai.addContent(x);
        
        x = new Element("crescimentoPop").addContent(elem.getCrescimentoPopulacional());
        pai.addContent(x);
        
        x = new Element("cidadesPopulosas");
        pai.addContent(x);
        
        for (int i = 0; i < elem.getMaioresCidades().size(); i++) {
            Element y = new Element("cidade").addContent(elem.getMaioresCidades().get(i));
            x.addContent(y);
        }
        
        x = new Element("idiomas").addContent(elem.getIdiomasOficiais());
        pai.addContent(x);
        
        x = new Element("paisesVizinhos");
        pai.addContent(x);
        
        for (int i = 0; i < elem.getPaisesVizinhos().size(); i++) {
            Element y = new Element("vizinho").addContent(elem.getPaisesVizinhos().get(i));
            x.addContent(y);
        }
        
        raiz.addContent(pai);
        return doc;
    }
    
    
}
