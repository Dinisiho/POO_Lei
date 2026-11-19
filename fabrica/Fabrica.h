#ifndef FABRICA_H
#define FABRICA_H

#include "../settings/Settings.h"
#include <string>
#include <vector>

class Posicao;

class Fabrica {
public:
    Fabrica(int nLinhas, int nColunas);
    explicit Fabrica(const std::string& caminhoMapa);
    ~Fabrica();

    Fabrica(const Fabrica&) = delete;
    Fabrica& operator=(const Fabrica&) = delete;

    int obtemLinhas() const;
    int obtemColunas() const;
    long long obtemCreditos() const;
    long long obtemInstante() const;
    int obtemNumeroMaquinas() const;
    const std::vector<std::string>& obtemEventos() const;
    bool contem(int linha, int coluna) const;

    Posicao& obtemPosicao(int linha, int coluna);
    const Posicao& obtemPosicao(int linha, int coluna) const;

    const char* compra(char tipo, int linha, int coluna, char direcao);
    const char* vende(int linha, int coluna);
    const char* roda(int linha, int coluna, char direcao);
    void avanca();

private:
    int linhas;
    int colunas;
    long long creditos;
    long long instante = 0;
    int numeroMaquinas = 0;
    std::vector<std::string> eventos;

    Posicao* posicoes[Settings::fabrica_max_linhas][Settings::fabrica_max_colunas]{};

    void registaEvento(int linha, int coluna, const std::string& descricao);
    void validaDimensoes() const;
    void criaTerrenos(const char mapa[][Settings::fabrica_max_colunas]);
    void libertaRecursos();
};

#endif
