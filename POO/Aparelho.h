//
// Created by fabio on 15/11/2023.
//

#ifndef FICHEIROSTP_APARELHO_H
#define FICHEIROSTP_APARELHO_H
#include "string"
#include "Propriedade.h"

class Aparelho {
    std::string letra;
    bool ligado;
    int id;
    int instante_inicial;
    int instante_ligou;
    int instante_desligou;
    int ultimo_incremento=0;
public:
    Aparelho(std::string letra, bool ligado,int id,int instante_inicial);
    const std::string &getLetra() const;
    bool isLigado() const;
    void setLigado(bool ligado);
    int getId()const;
    void setLetra(const std::string &letra);
    int getInstante()const;
    int getInstante_ligou() const;
    void setInstante_ligou(int valor);
    int getInstante_desligou() const;
    void setInstante_desligou(int valor);
    int getUltimo_Incremento() const;
    void setUltimo_Incremento(int valor);
};

class Aquecedor : public Aparelho {
public:
    Aquecedor(bool ligado, int id, int instante_inicial);
};
class Aspersor : public Aparelho {
public:
    Aspersor(bool ligado, int id, int instante_inicial);
};
class Refrigerador : public Aparelho {
public:
    Refrigerador(bool ligado, int id, int instante_inicial);
};
class Lampada : public Aparelho {
public:
    Lampada(bool ligado, int id, int instante_inicial);
};


#endif //FICHEIROSTP_APARELHO_H
