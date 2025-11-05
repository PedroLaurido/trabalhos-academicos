//
// Created by fabio on 15/11/2023.
//

#ifndef FICHEIROSTP_ZONA_H
#define FICHEIROSTP_ZONA_H
#include "Sensor.h"
#include <string>
#include <iostream>
#include "vector"
#include "Propriedade.h"
#include "Regra.h"
#include "Processador.h"
#include "Aparelho.h"
#include "Terminal.h"

class Zona {
private:
    int id;
    int linha;
    int coluna;
    int indice_janela;
    std::vector<Propriedade> propriedades;
    std::vector<Sensor *> sensores;
    std::vector<Processador *> processadores;
    std::vector<Aparelho *> aparelhos;

public:
    int getIndiceJanela() const;
    Zona(const int id, const int linha, const int coluna,int indice_janela);
    int getId() const;
    int getLinha() const;
    int getColuna() const;
    int getNumProps() const;
    Propriedade getProp(int indiceVetorProps);
    void adicionaProps();
    int setPropsByName(std::string nome,int valor);
    int adicionaComponente(std::string tipo,std::string tipo_in,int id_ap,int id_s,int id_p,int instante_inicial);
    int removeComponente(std::string tipo,int id_aparelho,int id_sensor,int id_processador);
    Aparelho* getAparelho(int indiceVetorAparelhos);
    Sensor* getSensor(int indiceVetorSensores);
    Processador* getProcessador(int indiceVetorProcessadores);
    int getNumSensores()const;
    int getNumProcessadores()const;
    int getNumAparelhos()const;
    void medir_props_para_sensores();
    std::string descricao();
    std::string descricaoComponentes();
    ~Zona();
    std::string getConteudoZona() const;
    void atualizaAparelho();
    void setPropsbyAparelhos(int instante_atual,int instante_momentaneo);
    void substituiProc(Processador substitutoz,Zona* zonaAtual);
    void adicionaProcessador(Processador novo);
};
#endif //FICHEIROSTP_ZONA_H
