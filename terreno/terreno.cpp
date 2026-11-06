#include "terreno.h"

char Solo::obterChar() const {
    return ' ';
}

bool Solo::permiteMaquina() const {
    return true;
}

char Cratera::obterChar() const {
    return '#';
}

bool Cratera::permiteMaquina() const {
    return false;
}

Deposito::Deposito(char tipo, int qInicial)
    : tipoRecurso(tipo), qRestante(qInicial) {
}

char Deposito::obterChar() const {
    return tipoRecurso;
}

bool Deposito::permiteMaquina() const {
    return true;
}

bool Deposito::esgotado() const {
    return qRestante <= 0;
}

Item* Deposito::extraiMateriaP() {
    // A extração fica por implementar até existir a classe Item.
    // Não altera a quantidade enquanto não puder devolver um Item válido.
    return nullptr;
}
