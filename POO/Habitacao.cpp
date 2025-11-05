//
// Created by fabio on 25/11/2023.
//

#include "Habitacao.h"

using namespace std;

Habitacao::Habitacao(int linhas, int colunas) : linhas(linhas), colunas(colunas) {
    matrizZonas = new Zona**[linhas];       // array de ponteiros  para linhas
    for (int i = 0; i < linhas; ++i) {      // ciclo for para criar array de ponteiros para as colunas
        matrizZonas[i] = new Zona*[colunas];
        for (int j = 0; j < colunas; ++j) {   // iteração para colocar todos os ponteiros a nullptr
            matrizZonas[i][j] = nullptr;
        }
    }
}

Habitacao::~Habitacao() {
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            delete matrizZonas[i][j];
        }
        delete[] matrizZonas[i];
    }
    delete[] matrizZonas;
}

void Habitacao::adicionaZona(const Zona& zona, int linha, int coluna) {
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        matrizZonas[linha][coluna] = new Zona(zona);  // Cria uma cópia da zona e armazena o ponteiro
    }
}

void Habitacao::removeZona(int linha, int coluna) {
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        delete matrizZonas[linha][coluna];
        matrizZonas[linha][coluna] = nullptr;
    }
}

int Habitacao::getLinhas() const {
    return linhas;
}

int Habitacao::getColunas() const {
    return colunas;
}

Zona* Habitacao::getZona(int linha, int coluna) const {
    return matrizZonas[linha][coluna];
}



