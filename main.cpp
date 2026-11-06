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
        if (!(std::cin >> opcao)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Opcao invalida. Introduz 0, 1 ou 2.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');

        if (opcao == 1) {
            const int totalDepositos = Settings::fabrica_num_depositos_ferro +
                                       Settings::fabrica_num_depositos_silicio +
                                       Settings::fabrica_num_depositos_gelo;
            const int minimoPosicoes = Settings::fabrica_num_crateras + totalDepositos;

            std::cout << "\n--- Criar fabrica aleatoria ---\n"
                      << "Dimensoes permitidas: 1 a " << Settings::fabrica_max_linhas
                      << " linhas e 1 a " << Settings::fabrica_max_colunas << " colunas.\n"
                      << "Terrenos a distribuir: " << Settings::fabrica_num_crateras
                      << " crateras + " << totalDepositos << " depositos.\n"
                      << "Sao necessarias pelo menos " << minimoPosicoes << " posicoes.\n"
                      << "Introduz 0 para voltar ao menu.\n";

            while (fabrica == nullptr && std::cin) {
                int linhas = 0;
                int colunas = 0;

                std::cout << "\nLinhas: ";
                if (!(std::cin >> linhas)) {
                    if (std::cin.eof()) break;
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Erro: introduz um numero inteiro de linhas.\n";
                    continue;
                }
                if (linhas == 0) {
                    std::cin.ignore(10000, '\n');
                    break;
                }

                std::cout << "Colunas: ";
                if (!(std::cin >> colunas)) {
                    if (std::cin.eof()) break;
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Erro: introduz um numero inteiro de colunas.\n";
                    continue;
                }
                std::cin.ignore(10000, '\n');
                if (colunas == 0) break;

                if (linhas < 1 || linhas > Settings::fabrica_max_linhas ||
                    colunas < 1 || colunas > Settings::fabrica_max_colunas) {
                    std::cout << "Erro: as dimensoes devem estar entre 1 e "
                              << Settings::fabrica_max_linhas << " linhas e entre 1 e "
                              << Settings::fabrica_max_colunas << " colunas.\n";
                    continue;
                }

                const int posicoesDisponiveis = linhas * colunas;
                if (posicoesDisponiveis < minimoPosicoes) {
                    std::cout << "\nErro: a fabrica de " << linhas << " x " << colunas
                              << " tem apenas " << posicoesDisponiveis << " posicoes.\n"
                              << "Sao necessarias pelo menos " << minimoPosicoes
                              << " posicoes para colocar " << Settings::fabrica_num_crateras
                              << " crateras e " << totalDepositos << " depositos.\n";
                    std::cout << "Tenta outras dimensoes ou introduz 0 para voltar.\n";
                    continue;
                }

                try {
                    fabrica = new Fabrica(linhas, colunas);
                    std::cout << "Fabrica criada com " << linhas << " x "
                              << colunas << " posicoes.\n";
                } catch (const std::string& erro) {
                    std::cout << "Erro: " << erro << '\n';
                } catch (...) {
                    std::cout << "Erro inesperado ao criar a fabrica.\n";
                }
            }
        } else if (opcao == 2) {
            std::string caminho;
            std::cout << "Caminho do ficheiro do mapa: ";
            if (!std::getline(std::cin, caminho)) break;
            try {
                fabrica = new Fabrica(caminho);
            } catch (const std::string& erro) {
                std::cout << "Erro: " << erro << '\n';
            } catch (...) {
                std::cout << "Erro inesperado ao carregar o mapa.\n";
            }
        } else if (opcao == 0) {
            break;
        } else {
            std::cout << "Opcao invalida. Introduz 0, 1 ou 2.\n";
        }
    }

    while (fabrica != nullptr && std::cin) {
        // Reserva espaco para o painel inferior quando nao cabe ao lado.
        int alturaEcra = fabrica->obtemLinhas() + 14;
        if (alturaEcra < 10) alturaEcra = 10;
        Ecra ecra(alturaEcra, Settings::ecra_max_colunas);
        vista.desenha(*fabrica, ecra);
        ecra.mostra();

        std::cout << "\n1 - Consultar terreno / regiao\n"
                     "0 - Terminar\nOpcao: ";
        int opcao = -1;
        if (!(std::cin >> opcao)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Opcao invalida. Introduz 0 ou 1.\n";
            continue;
        }
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
