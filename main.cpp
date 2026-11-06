#include "ecra/Ecra.h"
#include "fabrica/Fabrica.h"
#include "vista/VistaFabrica.h"

#include <iostream>
#include <string>

// Interface provisoria de demonstracao das funcionalidades do checkpoint 1.
// O processamento completo dos comandos pertence ao checkpoint 2.
int main() {
    VistaFabrica vista;
    Fabrica* fabrica = nullptr;

    while (fabrica == nullptr && std::cin) {
        std::cout << "\nCheckpoint 1 - Criacao da fabrica\n"
                     "1 - Criar fabrica aleatoria\n"
                     "2 - Carregar fabrica de um ficheiro\n"
                     "0 - Terminar\nOpcao: ";
        int opcao = -1;
        if (!(std::cin >> opcao)) break;
        std::cin.ignore(10000, '\n');

        try {
            if (opcao == 1) {
                int linhas = 0;
                int colunas = 0;
                std::cout << "Linhas e colunas: ";
                if (!(std::cin >> linhas >> colunas)) {
                    std::cout << "Dimensoes invalidas.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore(10000, '\n');
                fabrica = new Fabrica(linhas, colunas);
            } else if (opcao == 2) {
                std::string caminho;
                std::cout << "Nome do ficheiro do mapa: ";
                if (!std::getline(std::cin, caminho)) break;
                fabrica = new Fabrica(caminho);
            } else if (opcao == 0) {
                break;
            } else {
                std::cout << "Opcao invalida.\n";
            }
        } catch (const std::string& erro) {
            std::cout << "Erro: " << erro << '\n';
        } catch (...) {
            std::cout << "Erro inesperado ao criar a fabrica.\n";
        }
    }

    while (fabrica != nullptr && std::cin) {
        int alturaEcra = fabrica->obtemLinhas() + 3;
        if (alturaEcra < 10) alturaEcra = 10;
        Ecra ecra(alturaEcra, Settings::ecra_max_colunas);
        vista.desenha(*fabrica, ecra);
        ecra.mostra();

        std::cout << "\n1 - Consultar terreno / regiao\n"
                     "0 - Terminar\nOpcao: ";
        int opcao = -1;
        if (!(std::cin >> opcao)) break;
        std::cin.ignore(10000, '\n');
        if (opcao == 0) break;
        if (opcao != 1) {
            std::cout << "Opcao invalida.\n";
            continue;
        }

        std::string codigo;
        int raio = 0;
        std::cout << "Posicao (duas letras, ex. ab): ";
        if (!(std::cin >> codigo)) break;
        std::cin.ignore(10000, '\n');
        if (codigo.size() != 2 || codigo[0] < 'a' || codigo[0] > 'z' ||
            codigo[1] < 'a' || codigo[1] > 'z') {
            std::cout << "Posicao invalida.\n";
            continue;
        }
        std::cout << "Raio (0 para uma posicao): ";
        if (!(std::cin >> raio)) {
            std::cout << "Raio invalido.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');
        vista.consultaTerrenos(*fabrica, codigo[0] - 'a', codigo[1] - 'a',
                              raio, std::cout);
    }

    delete fabrica;
    return 0;
}
