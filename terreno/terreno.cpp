//
// Created by admin on 10/26/2026.
//

#include "terreno.h"

char Solo::obterChar() const {
    return ' ';
}

bool Solo::permiteMaquina() const {
    return true;
}

char Cratera::obterChar() const {
    return ' ';
}

bool Cratera::permiteMaquina() const {
    return true;
}

Deposito::Deposito(char tipo, int qInicial): tipoRecurso(tipo), qRestante(qInicial){}

char Deposito::obterChar() const {
    return tipoRecurso;
}
bool Deposito::permiteMaquina() const {
    return true;
}

bool Deposito::esgotado() const {
    return qRestante <= 0;
}

Item *Deposito::extraiMateriaP() {
    if (esgotado()) {
        return nullptr;
    }
    --qRestante;
    //temporario
    return nullptr;
}



