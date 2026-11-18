#include "comandos/Interpretador.h"

#include <exception>
#include <iostream>
#include <string>

int main() {
    try {
        Interpretador interpretador;
        std::cout << "Simulador de fabrica em Marte. Escreve 'ajuda' para ver os comandos.\n";

        std::string linha;
        while (!interpretador.terminou() && std::getline(std::cin, linha)) {
            interpretador.processaLinha(linha, std::cout);
        }
    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    } catch (const std::string& erro) {
        std::cerr << "Erro inesperado: " << erro << '\n';
        return 1;
    }
    return 0;
}
