#include "Fabrica.h"

#include "../posicao/Posicao.h"
#include "../terreno/terreno.h"

#include <fstream>
#include <random>
#include <sstream>
#include <string>

namespace {
    std::mt19937& obtemGerador() {
        static std::random_device dispositivo;
        static std::mt19937 gerador(dispositivo());
        return gerador;
    }

    void colocaAleatoriamente(
        char mapa[][Settings::fabrica_max_colunas],
        int linhas,
        int colunas,
        char tipo,
        int quantidade
    ) {
        std::uniform_int_distribution<int> sorteiaLinha(0, linhas - 1);
        std::uniform_int_distribution<int> sorteiaColuna(0, colunas - 1);

        int colocados = 0;

        while (colocados < quantidade) {
            int linha = sorteiaLinha(obtemGerador());
            int coluna = sorteiaColuna(obtemGerador());

            if (mapa[linha][coluna] == '\0') {
                mapa[linha][coluna] = tipo;
                ++colocados;
            }
        }
    }
    void validaIntervaloDepositos() {
        if (Settings::deposito_quantidade_min > Settings::deposito_quantidade_max) {
            throw std::string("Intervalo de quantidades invalido em Settings.");
        }
    }

    void removeRetornoCarro(std::string& linha) {
        if (!linha.empty() && linha.back() == '\r') {
            linha.pop_back();
        }
    }
}

Fabrica::Fabrica(int nLinhas, int nColunas)
    : linhas(nLinhas),
      colunas(nColunas),
      creditos(Settings::fabrica_creditos_iniciais) {

    validaDimensoes();
    validaIntervaloDepositos();

    int numCrateras = Settings::fabrica_num_crateras;
    int numFerro = Settings::fabrica_num_depositos_ferro;
    int numSilicio = Settings::fabrica_num_depositos_silicio;
    int numGelo = Settings::fabrica_num_depositos_gelo;
    int totalEspeciais = numCrateras + numFerro + numSilicio + numGelo;

    if (numCrateras < 0 || numFerro < 0 || numSilicio < 0 || numGelo < 0 ||
        totalEspeciais > linhas * colunas) {
        throw std::string("A fabrica nao tem posicoes suficientes para os terrenos definidos em Settings.");
    }

    char mapa[Settings::fabrica_max_linhas][Settings::fabrica_max_colunas]{};

    colocaAleatoriamente(mapa, linhas, colunas, '#', numCrateras);
    colocaAleatoriamente(mapa, linhas, colunas, '%', numFerro);
    colocaAleatoriamente(mapa, linhas, colunas, '$', numSilicio);
    colocaAleatoriamente(mapa, linhas, colunas, '~', numGelo);

    criaTerrenos(mapa);
}


Fabrica::Fabrica(const std::string& caminhoMapa)
    : linhas(0), colunas(0),
      creditos(Settings::fabrica_creditos_iniciais) {
    validaIntervaloDepositos();

    std::ifstream ficheiro(caminhoMapa);
    if (!ficheiro) {
        throw std::string("Nao foi possivel abrir o ficheiro de mapa.");
    }

    std::string cabecalho;
    if (!std::getline(ficheiro, cabecalho)) {
        throw std::string("Falta a primeira linha do mapa.");
    }
    removeRetornoCarro(cabecalho);

    std::istringstream entradaCabecalho(cabecalho);
    std::string textoExtra;
    if (!(entradaCabecalho >> linhas >> colunas) ||
        (entradaCabecalho >> textoExtra)) {
        throw std::string("A primeira linha do mapa deve conter apenas duas dimensoes inteiras.");
    }
    validaDimensoes();

    char mapa[Settings::fabrica_max_linhas][Settings::fabrica_max_colunas]{};
    for (int linha = 0; linha < linhas; ++linha) {
        std::string texto;
        if (!std::getline(ficheiro, texto)) {
            throw std::string("Faltam linhas no mapa.");
        }
        removeRetornoCarro(texto);
        const std::string::size_type comprimentoEsperado = colunas;
        if (texto.size() != comprimentoEsperado) {
            throw std::string("Uma linha do mapa nao tem o numero esperado de colunas.");
        }
        for (int coluna = 0; coluna < colunas; ++coluna) {
            const char caracter = texto[coluna];
            if (caracter != '.' && caracter != '#' && caracter != '%' &&
                caracter != '$' && caracter != '~') {
                throw std::string("O mapa contem um caracter de terreno invalido.");
            }
            mapa[linha][coluna] = caracter;
        }
    }

    std::string linhaExtra;
    if (std::getline(ficheiro, linhaExtra)) {
        throw std::string("O mapa contem linhas a mais.");
    }
    if (ficheiro.bad()) {
        throw std::string("Erro durante a leitura do mapa.");
    }
    criaTerrenos(mapa);
}

void Fabrica::validaDimensoes() const {
    if (linhas < 1 || linhas > Settings::fabrica_max_linhas ||
        colunas < 1 || colunas > Settings::fabrica_max_colunas) {
        throw std::string("Dimensoes da fabrica invalidas.");
    }
}

void Fabrica::criaTerrenos(const char mapa[][Settings::fabrica_max_colunas]) {
    std::uniform_int_distribution<int> sorteiaQuantidade(
        Settings::deposito_quantidade_min,
        Settings::deposito_quantidade_max
    );

    try {
        for (int linha = 0; linha < linhas; ++linha) {
            for (int coluna = 0; coluna < colunas; ++coluna) {
                switch (mapa[linha][coluna]) {
                    case '#':
                        terrenos[linha][coluna] = new Cratera();
                        break;
                    case '%':
                    case '$':
                    case '~':
                        terrenos[linha][coluna] = new Deposito(
                            mapa[linha][coluna],
                            sorteiaQuantidade(obtemGerador())
                        );
                        break;
                    default:
                        terrenos[linha][coluna] = new Solo();
                        break;
                }

                posicoes[linha][coluna] = new Posicao(*terrenos[linha][coluna]);
            }
        }
    } catch (...) {
        libertaRecursos();
        throw;
    }
}

Fabrica::~Fabrica() {
    libertaRecursos();
}

int Fabrica::obtemLinhas() const {
    return linhas;
}

int Fabrica::obtemColunas() const {
    return colunas;
}

int Fabrica::obtemCreditos() const {
    return creditos;
}

Posicao& Fabrica::obtemPosicao(int linha, int coluna) {
    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas) {
        throw std::string("Posicao fora dos limites da fabrica.");
    }

    return *posicoes[linha][coluna];
}

const Posicao& Fabrica::obtemPosicao(int linha, int coluna) const {
    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas) {
        throw std::string("Posicao fora dos limites da fabrica.");
    }

    return *posicoes[linha][coluna];
}

void Fabrica::libertaRecursos() {
    for (int linha = 0; linha < linhas; ++linha) {
        for (int coluna = 0; coluna < colunas; ++coluna) {
            delete posicoes[linha][coluna];
            posicoes[linha][coluna] = nullptr;

            delete terrenos[linha][coluna];
            terrenos[linha][coluna] = nullptr;
        }
    }
}
