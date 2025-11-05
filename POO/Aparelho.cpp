//
// Created by fabio on 15/11/2023.
//

#include "Aparelho.h"

Aparelho::Aparelho(std::string letra, bool ligado,int id,int instante_inicial) {
    this->letra=letra;
    this->ligado=ligado;
    this->id=id;
    this->instante_inicial=instante_inicial;
}

const std::string &Aparelho::getLetra() const {
    return letra;
}

void Aparelho::setLetra(const std::string &letra) {
    Aparelho::letra = letra;
}

bool Aparelho::isLigado() const {
    return ligado;
}

void Aparelho::setLigado(bool ligado) {
    this->ligado = ligado;
}

int Aparelho::getId() const {
    return id;
};

int Aparelho::getInstante() const {
    return instante_inicial;
};

int Aparelho::getInstante_ligou() const {
    return instante_ligou;
};

void Aparelho::setInstante_ligou(int valor) {
    this->instante_ligou=valor;
}

int Aparelho::getInstante_desligou() const {
    return instante_desligou;
};

void Aparelho::setInstante_desligou(int valor) {
    this->instante_desligou=valor;
}

int Aparelho::getUltimo_Incremento() const {
    return ultimo_incremento;
}

void Aparelho::setUltimo_Incremento(int valor) {
    this->ultimo_incremento=valor;
}

Aquecedor::Aquecedor(bool ligado, int id, int instante_inicial): Aparelho("a", ligado, id, instante_inicial) {

}

Aspersor::Aspersor(bool ligado, int id, int instante_inicial): Aparelho("s", ligado, id, instante_inicial) {

}

Refrigerador::Refrigerador(bool ligado, int id, int instante_inicial): Aparelho("r", ligado, id, instante_inicial) {

}

Lampada::Lampada(bool ligado, int id, int instante_inicial): Aparelho("l", ligado, id, instante_inicial) {

}