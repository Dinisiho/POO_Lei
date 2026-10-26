#include "ecra/Ecra.h"

int main() {
    Ecra ecra(30, 75); // Instancia o ecrã virtual com área útil

    // Moldura exterior
    ecra.desenhaMoldura(1, 2, 20, 30);
    ecra.escreve(1, 5, " FABRICA ");

    // Moldura do painel de estado lateral
    ecra.desenhaMoldura(1, 35, 12, 35);
    ecra.escreve(2, 37, "Instante: 0");
    ecra.escreve(3, 37, "Creditos: 1000");
    ecra.escreve(4, 37, "Maquinas: 0");
    ecra.escreve(5, 37, "Engenheiros: 0");

    // Imprime tudo de uma só vez na consola
    ecra.mostra();

    return 0;
}
