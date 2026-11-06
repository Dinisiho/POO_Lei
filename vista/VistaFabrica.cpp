#include "VistaFabrica.h"
#include "../ecra/Ecra.h"
#include "../fabrica/Fabrica.h"
#include "../posicao/Posicao.h"
#include "../terreno/terreno.h"

#include <ostream>
#include <string>

void VistaFabrica::desenha(const Fabrica& fabrica, Ecra& ecra) const {
    ecra.limpa();
    const int nLinhas = fabrica.obtemLinhas();
    const int nColunas = fabrica.obtemColunas();

    // Uma linha para a regua e uma coluna para as letras das linhas.
    ecra.desenhaMoldura(0, 0, nLinhas + 3, nColunas + 4);
    for (int coluna = 0; coluna < nColunas; ++coluna) {
        ecra.escreve(1, coluna + 2, char('A' + coluna));
    }
    for (int linha = 0; linha < nLinhas; ++linha) {
        ecra.escreve(linha + 2, 1, char('A' + linha));
        for (int coluna = 0; coluna < nColunas; ++coluna) {
            const Terreno& terreno = fabrica.obtemPosicao(linha, coluna).obtemTerreno();
            ecra.escreve(linha + 2, coluna + 2, terreno.obterChar());
        }
    }

    const int painelColuna = nColunas + 6;
    const int painelLargura = ecra.obtemColunas() - painelColuna;
    if (painelLargura >= 30) {
        ecra.desenhaMoldura(0, painelColuna, 10, painelLargura);
        ecra.escreve(1, painelColuna + 2, "Instante: 0");
        ecra.escreve(2, painelColuna + 2,
                    "Creditos: " + std::to_string(fabrica.obtemCreditos()));
        ecra.escreve(3, painelColuna + 2, "Maquinas: 0");
        ecra.escreve(4, painelColuna + 2, "Engenheiros: 0");
        ecra.escreve(6, painelColuna + 2, "No ultimo instante:");
        ecra.escreve(7, painelColuna + 2, "Sem eventos (checkpoint 1)");
    }
}

bool VistaFabrica::consultaTerrenos(const Fabrica& fabrica, int linha, int coluna,
                                   int raio, std::ostream& saida) const {
    if (linha < 0 || linha >= fabrica.obtemLinhas() ||
        coluna < 0 || coluna >= fabrica.obtemColunas() || raio < 0) {
        saida << "Posicao ou raio invalido.\n";
        return false;
    }

    // Um raio maior que as dimensoes cobre toda a fabrica.
    int raioMaximo = Settings::fabrica_max_linhas;
    if (Settings::fabrica_max_colunas > raioMaximo) {
        raioMaximo = Settings::fabrica_max_colunas;
    }
    if (raio > raioMaximo) raio = raioMaximo;

    for (int i = 0; i < fabrica.obtemLinhas(); ++i) {
        for (int j = 0; j < fabrica.obtemColunas(); ++j) {
            int distanciaLinha = i - linha;
            int distanciaColuna = j - coluna;
            if (distanciaLinha < 0) distanciaLinha = -distanciaLinha;
            if (distanciaColuna < 0) distanciaColuna = -distanciaColuna;
            if (distanciaLinha > raio || distanciaColuna > raio) continue;

            const Terreno& terreno = fabrica.obtemPosicao(i, j).obtemTerreno();
            saida << char('a' + i) << char('a' + j)
                  << ": " << terreno.descricao() << '\n';
        }
    }
    return true;
}
