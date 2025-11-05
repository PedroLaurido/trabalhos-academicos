//
// Created by fabio on 15/11/2023.
//

#ifndef FICHEIROSTP_PROCESSADOR_H
#define FICHEIROSTP_PROCESSADOR_H
#include <vector>
#include "Sensor.h"
#include "Aparelho.h"
#include "Regra.h"


class Processador {
private:
    std::vector<Regra> regras;
    std::vector<Aparelho> aparelhos;
    int id_processador;
    std::string comando;
    int idZonaAssociada;
    std::string nome;

public:
    Processador(std::string comando,int id, int idZona);
    Processador(int id,std::string comando,int zona_associada,std::vector<Regra> regras,std::vector<Aparelho> aparelhos);
    bool mandar_comando(int instante,int mandar_comando);
    int getId_Proc();
    std::string getComando();
    int getIdZonaAssociada();
    std::vector<Regra> getVetorRegras() const;
    std::vector<Aparelho> getVetorAparelhos() const;
    void setVetorRegras (const std::vector<Regra>& novoVetorRegras);
    void setVetorAparelhos (const std::vector<Aparelho>& novoVetorAparelhos);
    int adicionaRegra(std::string tipo,int id,int x, int y,int valor_avaliar,Sensor* sensor);
    int removeRegra(int id_regra);
    int getNumRegras()const;
    void setComando(const std::string &comando);
    void setNome(const std::string &nome);
    const std::string getNome();
    void atualizaRegra();
    std::string lista_regras();
    int adicionaAparelho(std::string letra,bool ligado,int id,int instante_inicial);
    int removeAparelho(int id);
    int getNumAparelhos()const;
    Aparelho getAparelho(int indiceVetorAparelhos);
    Regra getRegra(int indiceVetorRegras);
    ~Processador();
};


#endif //FICHEIROSTP_PROCESSADOR_H
