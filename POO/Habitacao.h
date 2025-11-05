//
// Created by fabio on 25/11/2023.
//

#ifndef FICHEIROSTP_HABITACAO_H
#define FICHEIROSTP_HABITACAO_H

#include "Zona.h"

class Habitacao {
private:
    int linhas;
    int colunas;
    Zona*** matrizZonas;

public:
    Zona* getZona(int linha, int coluna) const;
    int getLinhas() const;
    int getColunas() const;
    Habitacao(int linhas, int colunas);
    void adicionaZona(const Zona& zona, int linha, int coluna);
    void removeZona(int linha, int coluna);
    ~Habitacao();
};

#endif //FICHEIROSTP_HABITACAO_H
