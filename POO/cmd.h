//
// Created by 35191 on 31/10/2023.
//
#include "Sensor.h"
#include "Regra.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "Terminal.h"
#include "Simulacao.h"

using namespace std;

#ifndef TP_POO_CMD_H
#define TP_POO_CMD_H

void
processarComando(const string &comando, const string &arg1, const string &arg2, const string &arg3, const string &arg4, const string &arg5,const string &arg6,Simulacao &simulacao, term::Window &w,int &instante,int &id_ap,int &id_s,int &id_p,int &id_r);

#endif //TP_POO_CMD_H
