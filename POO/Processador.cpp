//
// Created by fabio on 15/11/2023.
//
#include "Sensor.h"
#include "Regra.h"
#include "Processador.h"

Processador::Processador(std::string comando,int id, int idZona) {
    this->comando=comando;
    this->id_processador=id;
    this->idZonaAssociada=idZona;
}

Processador::Processador(int id, std::string comando, int zona_associada, std::vector<Regra> regras,
                         std::vector<Aparelho> aparelhos) {
    this->id_processador=id;
    this->comando=comando;
    this->idZonaAssociada=zona_associada;
    this->regras=regras;
    this->aparelhos=aparelhos;
}

int Processador::adicionaRegra(std::string tipo,int id ,int x, int y, int valor_avaliar,Sensor* sensor) {
    if(tipo!="iguala" && tipo!="menorque" && tipo!="maiorque" && tipo!="entre" && tipo!="fora"){
        return 0;
    }
    regras.push_back(Regra(tipo,id,x,y,valor_avaliar,sensor));
    return 1;
}

int Processador::removeRegra(int id_regra) {

    if(!regras.empty()){
        for (int i = 0; i < regras.size(); i++) {
            if(regras[i].getID()==id_regra){
                regras.erase(regras.begin() + i);
                return 1;
            }
        }
    }
    return 0;
}

void Processador::atualizaRegra() {
    if(!regras.empty()){
        for (int i = 0; i < regras.size(); i++) {
            regras[i].setValor_avaliar(regras[i].getSensorAssociado()->getValorMedido());
            regras[i].avaliar();
        }
    }
}

bool Processador::mandar_comando(int instante,int mandar_comando) {
    if(mandar_comando==0){
        return false;
    }

    if (regras.size() > 0) {
        for (int i = 0; i < regras.size(); i++) {
            if (!regras[i].getVar_Regra()) {
                return false;
            }
        }
            if(comando=="liga"){
                for(int j=0;j<aparelhos.size();j++){
                    aparelhos[j].setLigado(true);
                    aparelhos[j].setInstante_ligou(instante);
                }
            }else if(comando=="desliga"){
                for(int j=0;j<aparelhos.size();j++){
                    aparelhos[j].setLigado(false);
                    aparelhos[j].setInstante_desligou(instante);
                    aparelhos[j].setUltimo_Incremento(0);
                }
            }
            return true;
    }
    return false;
}

int Processador::getId_Proc() {
 return id_processador;
}

std::string Processador::getComando() {
    return comando;
}

int Processador::getIdZonaAssociada(){
    return idZonaAssociada;
}

std::string Processador::lista_regras() {
    std::string info;

    for (int i = 0; i < regras.size(); ++i) {
        info += "*Nome : " + regras[i].getTipo() + ", ID_regra: " + std::to_string(regras[i].getID()) +"\nID_sensor: "+std::to_string(regras[i].getSensorAssociado()->getId())+
                ", Nome_sensor: "+regras[i].getSensorAssociado()->getPropObservada() +
                " Valor1: "+std::to_string(regras[i].getX())+" Valor2: "+std::to_string(regras[i].getY())+"\n";
    }

    return info;
}

int Processador::getNumRegras() const {
    return regras.size();
}

void Processador::setComando(const std::string &comando) {
    this->comando = comando;
}

void Processador::setNome(const std::string &nome) {
    this->nome = nome;
}

const std::string Processador::getNome(){
    return nome;
}

int Processador::adicionaAparelho(std::string letra, bool ligado, int id,int instante_inicial) {
    aparelhos.push_back(Aparelho(letra,ligado,id,instante_inicial));
    return 1;
}

int Processador::removeAparelho(int id) {

        for(int i=0;i<aparelhos.size();i++){
            if(aparelhos[i].getId()==id){
                aparelhos.erase(aparelhos.begin()+i);
                return 1;
            }
        }

    return 0;
}

int Processador::getNumAparelhos() const {
    return aparelhos.size();
}

Aparelho Processador::getAparelho(int indiceVetorAparelhos) {
    return aparelhos[indiceVetorAparelhos];
}

Regra Processador::getRegra(int indiceVetorRegras) {
    return regras[indiceVetorRegras];
}

std::vector<Regra> Processador::getVetorRegras() const {
    return regras;
}

std::vector<Aparelho> Processador::getVetorAparelhos() const {
    return aparelhos;
}

void Processador::setVetorRegras(const std::vector<Regra> &novoVetorRegras) {
    regras=novoVetorRegras;
}

void Processador::setVetorAparelhos(const std::vector<Aparelho> &novoVetorAparelhos) {
    aparelhos=novoVetorAparelhos;
}

Processador::~Processador() {
    for (int i = 0; i < regras.size(); i++) {
        regras.erase(regras.begin() + i);
    }
}



