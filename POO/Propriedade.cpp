//
// Created by fabio on 15/11/2023.
//

#include "Propriedade.h"

Propriedade::Propriedade(std::string chave,int valor,int valor_min,int valor_max) {
    this->chave=chave;
    this->valor=valor;
    this->valor_min=valor_min;
    this->valor_max=valor_max;
}

const std::string &Propriedade::getChave() const {
    return chave;
}

int Propriedade::getValor() const {
    return valor;
}

void Propriedade::setValor(int valor) {
    this->valor=valor;
}

int Propriedade::getValor_min() const {
    return valor_min;
}

int Propriedade::getValor_max() const {
    return valor_max;
}

