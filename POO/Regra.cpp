//
// Created by fabio on 15/11/2023.
//

#include "Regra.h"

Regra::Regra(std::string tipo, int id,int x, int y,int valor_avaliar,Sensor* sensor) {
    this->tipo=tipo;
    this->id_regra=id;
    this->x=x;
    this->y=y;
    this->valor_avaliar=valor_avaliar;
    this->sensorAssociado=sensor;
}

int Regra::getValor_avaliar() {
    return valor_avaliar;
}

int Regra::getID() {
    return id_regra;
}

void Regra::setVar_Regra(bool valor) {
    this->var_regra=valor;
}

void Regra::setValor_avaliar(int valor) {
    this->valor_avaliar=valor;
}

bool Regra::getVar_Regra() const{
    return var_regra;
}

std::string Regra::getTipo() {
    return tipo;
}

void Regra::avaliar(){
    valor_avaliar=getValor_avaliar();
    if (tipo == "iguala") {
        if(x==valor_avaliar){
            setVar_Regra(true);
        }
    } else if (tipo == "menorque") {
        if(valor_avaliar<x){
            setVar_Regra(true);
        }
    } else if (tipo == "maiorque") {
        if(valor_avaliar>x){
            setVar_Regra(true);
        }
    } else if (tipo == "entre") {
        if(valor_avaliar>=x && valor_avaliar<=y){
            setVar_Regra(true);
        }
    } else if (tipo == "fora") {
        if(valor_avaliar<x || valor_avaliar>y){
            setVar_Regra(true);
        }
    } else {
        setVar_Regra(false);
    }
}

Sensor *Regra::getSensorAssociado() const {
    return sensorAssociado;
}

int Regra::getX() const {
    return x;
}

int Regra::getY() const {
    return y;
}


