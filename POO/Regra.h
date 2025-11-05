//
// Created by fabio on 15/11/2023.
//

#ifndef FICHEIROSTP_REGRA_H
#define FICHEIROSTP_REGRA_H
#include "Sensor.h"
#include <iostream>
#include <string>
class Sensor;

class Regra {
private:
    Sensor * sensorAssociado;
    int id_regra;
    int x;
    int y;
    int valor_avaliar;
    std::string tipo;
    bool var_regra=false;
public:
    Sensor *getSensorAssociado() const;
    Regra(std::string tipo,int id ,int x, int y,int valor_avaliar,Sensor* sensor);
    void setVar_Regra(bool valor);
    bool getVar_Regra() const;
    int getValor_avaliar();
    int getID();
    std::string getTipo();
    void setValor_avaliar(int valor);
    void avaliar();
    int getX() const;
    int getY() const;
};



#endif //FICHEIROSTP_REGRA_H
