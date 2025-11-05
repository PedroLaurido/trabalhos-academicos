//
// Created by fabio on 15/11/2023.
//

#ifndef FICHEIROSTP_SENSOR_H
#define FICHEIROSTP_SENSOR_H
#include "Sensor.h"
#include "Regra.h"
#include "string"
#include "Propriedade.h"
#include <vector>
#include <iostream>
class Regra;

class Sensor {
    std::string prop_observada;
    std::string letra_visualizacao;
    int id;
    int valor_medido;

public:
    Sensor(std::string prop_observada,std::string letra_visualizacao,int id);
    const std::string &getPropObservada() const;
    const std::string &getLetraVisualizacao() const;
    int getValorMedido() const;
    void setValorMedido(int valor);
    int getId()const;
    virtual void get_propriedade(std::vector<Propriedade> propriedades);
};

class sTemperatura : public Sensor{
public:
    sTemperatura(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

class sMovimento : public Sensor{
public:
    sMovimento(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

class sLuz : public Sensor{
public:
    sLuz(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

class sRadiacao : public Sensor{
public:
    sRadiacao(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

class sHumidade : public Sensor{
public:
    sHumidade(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

class sSom : public Sensor{
public:
    sSom(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

class sFumo : public Sensor{
public:
    sFumo(int id);
    void get_propriedade (std::vector<Propriedade> propriedades) override;
};

#endif //FICHEIROSTP_SENSOR_H
