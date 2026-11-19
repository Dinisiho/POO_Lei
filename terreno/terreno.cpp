#include "terreno.h"

bool Terreno::permiteMaquina() const { return true; }
Item Terreno::extrai() { return {}; }
bool Terreno::esgotado() const { return false; }
bool Cratera::permiteMaquina() const { return false; }

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

Item Deposito::extrai() {
    if (esgotado()) return {};
    Item::Tipo tipo;
    switch (tipoRecurso) {
        case '%': tipo = Item::Tipo::Ferro; break;
        case '$': tipo = Item::Tipo::Silicio; break;
        case '~': tipo = Item::Tipo::Gelo; break;
        default: return {};
    }
    --qRestante;
    return Item(tipo);
}

bool Deposito::esgotado() const { return qRestante <= 0; }

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
