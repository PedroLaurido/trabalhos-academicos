//
// Created by fabio on 15/11/2023.
//

#ifndef FICHEIROSTP_PROPRIEDADE_H
#define FICHEIROSTP_PROPRIEDADE_H

#include "string"

class Propriedade {
    std::string chave;
    int valor;
    int valor_min;
    int valor_max;
public:
    Propriedade(std::string chave,int valor,int valor_min,int valor_max);
    const std::string &getChave() const;
    int getValor() const;
    void setValor(int valor);
    int getValor_min() const;
    int getValor_max() const;
};


#endif //FICHEIROSTP_PROPRIEDADE_H
