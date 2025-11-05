//
// Created by fabio on 15/11/2023.
//

#include "Sensor.h"

Sensor::Sensor(std::string prop_observada,std::string letra_visualizacao,int id){
    this->prop_observada=prop_observada;
    this->letra_visualizacao=letra_visualizacao;
    this->id=id;
}

const std::string &Sensor::getPropObservada() const {
    return prop_observada;
}

const std::string &Sensor::getLetraVisualizacao() const {
    return letra_visualizacao;
}

int Sensor::getId() const {
    return id;
};

void Sensor::get_propriedade(std::vector<Propriedade> propriedades) {
    for(int i = 0; i < propriedades.size(); i++) {
        valor_medido = propriedades[i].getValor();
    }
}

int Sensor::getValorMedido() const {
    return valor_medido;
}

void Sensor::setValorMedido(int valor){
    this->valor_medido=valor;
}

/*int Sensor::adicionaRegra(std::string tipo,int id,int x, int y) {
    regras.push_back(Regra(tipo,id,x,y,getValorMedido()));
    return 1;
}*/



sTemperatura::sTemperatura(int id): Sensor("Temperatura","t",id) {

}

void sTemperatura::get_propriedade(std::vector<Propriedade> propriedades) {
    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Temperatura") {
            setValorMedido(propriedades[i].getValor());
        }
    }
}

sMovimento::sMovimento(int id): Sensor("Vibracao","v",id) {

}

void sMovimento::get_propriedade(std::vector<Propriedade> propriedades) {

    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Vibracao")
            setValorMedido(propriedades[i].getValor());
    }
}

sLuz::sLuz(int id):Sensor("Luz","m",id) {

}

void sLuz::get_propriedade(std::vector<Propriedade> propriedades) {
    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Luz")
            setValorMedido(propriedades[i].getValor());
    }
}

sRadiacao::sRadiacao(int id): Sensor("Radiacao","d",id) {

}

void sRadiacao::get_propriedade(std::vector<Propriedade> propriedades) {
    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Radiacao")
            setValorMedido(propriedades[i].getValor());
    }
}

sHumidade::sHumidade(int id): Sensor("Humidade","h",id){

}

void sHumidade::get_propriedade(std::vector<Propriedade> propriedades) {
    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Humidade")
            setValorMedido(propriedades[i].getValor());
    }
}

sSom::sSom(int id):Sensor("Som","o",id) {

}

void sSom::get_propriedade(std::vector<Propriedade> propriedades) {
    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Som")
            setValorMedido(propriedades[i].getValor());
    }
}

sFumo::sFumo(int id):Sensor("Fumo","f",id) {

}

void sFumo::get_propriedade(std::vector<Propriedade> propriedades) {
    for (int i = 0; i < propriedades.size(); i++) {
        if (propriedades[i].getChave() == "Fumo")
            setValorMedido(propriedades[i].getValor());
    }
}

