//
// Created by Pedro Pereira on 06/12/2023.
//

#include "Simulacao.h"

Simulacao::Simulacao() {
    idZona = 1;
    habitacao = nullptr;
}

Simulacao::~Simulacao() {
    if (habitacao != nullptr) {
        delete habitacao;
    }
}

int Simulacao::getIdZona() const {
    return idZona;
}

Habitacao* Simulacao::getHabitacao() const {
    return habitacao;
}

void Simulacao::setHabitacao(Habitacao* habitacao) {
    this->habitacao = habitacao;
}

void Simulacao::incrementaIdZona() {
    ++idZona;
}

int Simulacao::guardaProcessador(Processador processador, const std::string nome){

    for(int i=0;i<processadoresGuardados.size();i++){
        if(processadoresGuardados[i].getNome()==nome){
            return 0;
        }
    }
    processador.setNome(nome);
    processadoresGuardados.push_back(processador);
    return 1;
}

int Simulacao::getNumProcessadoresGuardados() const {
    return processadoresGuardados.size();
}

Processador Simulacao::getProcessadorGuardado(int indice) const {
    return processadoresGuardados[indice];
}

int Simulacao::removeProcGuardado(const std::string nome){

    if(processadoresGuardados.empty())
        return 0;

    for(int i=0; i<processadoresGuardados.size(); i++){
        if(processadoresGuardados[i].getNome() == nome){
            processadoresGuardados.erase(processadoresGuardados.begin() + i);
            return 1;
        }
    }
    return 0;
}

