#include "Posicao.h"
#include "../terreno/terreno.h"

Posicao::Posicao(Terreno& terreno)
    : terreno(&terreno) {
}

Terreno& Posicao::obtemTerreno() {
    return *terreno;
}

const Terreno& Posicao::obtemTerreno() const {
    return *terreno;
}
