#include "terreno.h"

char Solo::obterChar() const {
    return ' ';
}

char Cratera::obterChar() const {
    return '#';
}

Deposito::Deposito(char tipo, int qInicial)
    : tipoRecurso(tipo), qRestante(qInicial) {
}

char Deposito::obterChar() const {
    return tipoRecurso;
}

std::string Solo::descricao() const {
    return "Solo";
}

std::string Cratera::descricao() const {
    return "Cratera";
}

int Deposito::obtemQuantidade() const {
    return qRestante;
}

std::string Deposito::descricao() const {
    std::string recurso;
    switch (tipoRecurso) {
        case '%': recurso = "minerio de ferro"; break;
        case '$': recurso = "silicio bruto"; break;
        case '~': recurso = "gelo"; break;
        default: recurso = "desconhecido"; break;
    }
    return "Deposito de " + recurso + " (" + std::to_string(qRestante) + " unidades)";
}
