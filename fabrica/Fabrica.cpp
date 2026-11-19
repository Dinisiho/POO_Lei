#include "Fabrica.h"

#include "../maquinas/Maquina.h"
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
        if (Settings::deposito_quantidade_min < 0 ||
            Settings::deposito_quantidade_min > Settings::deposito_quantidade_max) {
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
    long long totalEspeciais = static_cast<long long>(numCrateras) + numFerro + numSilicio + numGelo;

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
    std::string textoLinhas, textoColunas;
    std::string textoExtra;
    entradaCabecalho >> textoLinhas >> textoColunas;
    std::istringstream entradaLinhas(textoLinhas), entradaColunas(textoColunas);
    char resto;
    if (!(entradaLinhas >> linhas) || (entradaLinhas >> resto) ||
        !(entradaColunas >> colunas) || (entradaColunas >> resto) ||
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
                const char tipo = mapa[linha][coluna];
                const bool deposito = tipo == '%' || tipo == '$' || tipo == '~';
                const int quantidade = deposito ? sorteiaQuantidade(obtemGerador()) : 0;
                posicoes[linha][coluna] = new Posicao(tipo, quantidade);
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

long long Fabrica::obtemCreditos() const {
    return creditos;
}

long long Fabrica::obtemInstante() const { return instante; }
int Fabrica::obtemNumeroMaquinas() const { return numeroMaquinas; }
const std::vector<std::string>& Fabrica::obtemEventos() const { return eventos; }

bool Fabrica::contem(int linha, int coluna) const {
    return linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas;
}

Posicao& Fabrica::obtemPosicao(int linha, int coluna) {
    if (!contem(linha, coluna)) {
        throw std::string("Posicao fora dos limites da fabrica.");
    }

    return *posicoes[linha][coluna];
}

const Posicao& Fabrica::obtemPosicao(int linha, int coluna) const {
    if (!contem(linha, coluna)) {
        throw std::string("Posicao fora dos limites da fabrica.");
    }

    return *posicoes[linha][coluna];
}

const char* Fabrica::compra(char tipo, int linha, int coluna, char direcao) {
    if (!contem(linha, coluna)) return "Posicao fora da fabrica.";
    Posicao& posicao = obtemPosicao(linha, coluna);
    if (posicao.obtemMaquina() != nullptr) return "A posicao ja tem uma maquina.";
    if (!posicao.obtemTerreno().permiteMaquina()) return "Nao se pode comprar numa cratera.";
    Maquina* maquina = Maquina::cria(tipo, direcao);
    if (maquina == nullptr) return "Tipo de maquina ou direcao invalida.";
    if (creditos < maquina->obtemCusto()) {
        delete maquina;
        return "Creditos insuficientes.";
    }
    creditos -= maquina->obtemCusto();
    posicao.instalaMaquina(maquina);
    ++numeroMaquinas;
    return nullptr;
}

const char* Fabrica::vende(int linha, int coluna) {
    if (!contem(linha, coluna)) return "Posicao fora da fabrica.";
    Posicao& posicao = obtemPosicao(linha, coluna);
    const Maquina* maquina = posicao.obtemMaquina();
    if (maquina == nullptr) return "A posicao nao tem maquina.";
    creditos += static_cast<long long>(maquina->obtemCusto()) *
                Settings::maquina_percentagem_venda / 100;
    posicao.removeMaquina();
    --numeroMaquinas;
    return nullptr;
}

const char* Fabrica::roda(int linha, int coluna, char direcao) {
    if (!contem(linha, coluna)) return "Posicao fora da fabrica.";
    if (!Maquina::direcaoValida(direcao)) return "Direcao invalida.";
    Maquina* maquina = obtemPosicao(linha, coluna).obtemMaquina();
    if (maquina == nullptr) return "A posicao nao tem maquina.";
    maquina->roda(direcao);
    return nullptr;
}

void Fabrica::registaEvento(int linha, int coluna, const std::string& descricao) {
    const std::string posicao{char('a' + linha), char('a' + coluna)};
    eventos.push_back(posicao + ": " + descricao);
}

void Fabrica::avanca() {
    ++instante;
    eventos.clear();

    // Todas as máquinas fazem primeiro o trabalho local.
    for (int linha = 0; linha < linhas; ++linha) {
        for (int coluna = 0; coluna < colunas; ++coluna) {
            Posicao& posicao = obtemPosicao(linha, coluna);
            Maquina* maquina = posicao.obtemMaquina();
            if (maquina == nullptr) continue;
            std::string evento;
            maquina->preparaInstante(posicao.obtemTerreno(), instante, obtemGerador(), evento);
            if (!evento.empty()) registaEvento(linha, coluna, evento);
            if (posicao.transformaDepositoEsgotado()) {
                registaEvento(linha, coluna, "Deposito esgotado");
            }
        }
    }

    // Só saem itens que já estavam na origem antes deste instante.
    for (int linha = 0; linha < linhas; ++linha) {
        for (int coluna = 0; coluna < colunas; ++coluna) {
            Maquina* origem = obtemPosicao(linha, coluna).obtemMaquina();
            if (origem == nullptr) continue;
            const Item item = origem->itemParaEntregar(instante);
            if (!item.existe()) continue;

            int vizinhaLinha = linha;
            int vizinhaColuna = coluna;
            switch (origem->obtemDirecao()) {
                case 'c': --vizinhaLinha; break;
                case 'b': ++vizinhaLinha; break;
                case 'e': --vizinhaColuna; break;
                case 'd': ++vizinhaColuna; break;
            }
            Maquina* destino = contem(vizinhaLinha, vizinhaColuna)
                ? obtemPosicao(vizinhaLinha, vizinhaColuna).obtemMaquina() : nullptr;
            if (destino != nullptr && origem->tentaEntregar(*destino, instante)) {
                const std::string vizinha{char('a' + vizinhaLinha), char('a' + vizinhaColuna)};
                registaEvento(linha, coluna, std::string("Entregou ") + item.obtemNome() + " a " + vizinha);
            } else {
                registaEvento(linha, coluna, std::string("Bloqueada com ") + item.obtemNome());
            }
        }
    }
}

void Fabrica::libertaRecursos() {
    for (int linha = 0; linha < linhas; ++linha) {
        for (int coluna = 0; coluna < colunas; ++coluna) {
            delete posicoes[linha][coluna];
            posicoes[linha][coluna] = nullptr;

        }
    }
}
