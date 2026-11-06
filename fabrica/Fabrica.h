#ifndef FABRICA_H
#define FABRICA_H

#include "../settings/Settings.h"

class Terreno;
class Posicao;

class Fabrica {
public:
    Fabrica(int linhas, int colunas);
    ~Fabrica();

    Fabrica(const Fabrica&) = delete;
    Fabrica& operator=(const Fabrica&) = delete;

    int obtemLinhas() const;
    int obtemColunas() const;
    int obtemCreditos() const;

    Posicao& obtemPosicao(int linha, int coluna) const;

private:
    int linhas;
    int colunas;
    int creditos;

    Terreno* terrenos[Settings::fabrica_max_linhas][Settings::fabrica_max_colunas]{};
    Posicao* posicoes[Settings::fabrica_max_linhas][Settings::fabrica_max_colunas]{};

    void libertaRecursos();
};

#endif
