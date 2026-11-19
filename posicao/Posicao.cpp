#include "Posicao.h"
#include "../maquinas/Maquina.h"
#include "../terreno/terreno.h"

namespace {
    Terreno* criaTerreno(char tipo, int quantidade) {
        switch (tipo) {
            case '#': return new Cratera();
            case '%':
            case '$':
            case '~': return new Deposito(tipo, quantidade);
            default: return new Solo();
        }
    }
}

Posicao::Posicao(char tipoTerreno, int quantidade)
    : terreno(criaTerreno(tipoTerreno, quantidade)) {}

Posicao::~Posicao() {
    delete maquina;
    delete terreno;
}

Terreno& Posicao::obtemTerreno() {
    return *terreno;
}

const Terreno& Posicao::obtemTerreno() const {
    return *terreno;
}

Maquina* Posicao::obtemMaquina() { return maquina; }
const Maquina* Posicao::obtemMaquina() const { return maquina; }

void Posicao::instalaMaquina(Maquina* nova) { maquina = nova; }

void Posicao::removeMaquina() {
    delete maquina;
    maquina = nullptr;
}

bool Posicao::transformaDepositoEsgotado() {
    if (!terreno->esgotado()) return false;
    Terreno* solo = new Solo();
    delete terreno;
    terreno = solo;
    return true;
}
