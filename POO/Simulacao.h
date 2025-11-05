//
// Created by Pedro Pereira on 06/12/2023.
//

#ifndef FICHEIROSTP_SIMULACAO_H
#define FICHEIROSTP_SIMULACAO_H

#include "Habitacao.h"

class Simulacao {
private:
    Habitacao* habitacao;
    int idZona;

    std::vector<Processador> processadoresGuardados;

public:
    Simulacao();
    ~Simulacao();
    int getIdZona() const;
    void incrementaIdZona();
    Habitacao* getHabitacao() const;
    void setHabitacao(Habitacao* habitacao);
    int getNumProcessadoresGuardados() const;
    Processador getProcessadorGuardado(int indice) const;
    int guardaProcessador(Processador processador , const std::string nome);
    int removeProcGuardado(const std::string nome);
};

#endif //FICHEIROSTP_SIMULACAO_H
