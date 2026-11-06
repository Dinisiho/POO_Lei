#include "Posicao.h"
#include "../terreno/terreno.h"

Posicao::Posicao(Terreno& terreno)
    : terreno(&terreno), maquina(nullptr) {
}

Terreno& Posicao::obtemTerreno() {
    return *terreno;
}

const Terreno& Posicao::obtemTerreno() const {
    return *terreno;
}

Maquina* Posicao::obtemMaquina() const {
    return maquina;
}

bool Posicao::podeReceberMaquina() const {
    return maquina == nullptr && terreno->permiteMaquina();
}

bool Posicao::colocaMaquina(Maquina* novaMaquina) {
    if (novaMaquina == nullptr || !podeReceberMaquina()) {
        return false;
    }

    maquina = novaMaquina;
    return true;
}

void Posicao::removeMaquina() {
    maquina = nullptr;
}
