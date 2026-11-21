#include "VistaFabrica.h"
#include "../ecra/Ecra.h"
#include "../fabrica/Fabrica.h"
#include "../maquinas/Maquina.h"
#include "../posicao/Posicao.h"
#include "../terreno/terreno.h"

#include <ostream>
#include <vector>
#include <string>

void VistaFabrica::desenha(const Fabrica& fabrica, Ecra& ecra) const {
    ecra.limpa();
    const int nLinhas = fabrica.obtemLinhas();
    const int nColunas = fabrica.obtemColunas();

    const int alturaMapa = nLinhas + 3;
    const int larguraMapa = nColunas + 4;

    ecra.desenhaMoldura(0, 0, alturaMapa, larguraMapa);
    for (int coluna = 0; coluna < nColunas; ++coluna) {
        ecra.escreve(1, coluna + 2, char('A' + coluna));
    }
    for (int linha = 0; linha < nLinhas; ++linha) {
        ecra.escreve(linha + 2, 1, char('A' + linha));
        for (int coluna = 0; coluna < nColunas; ++coluna) {
            const Posicao& posicao = fabrica.obtemPosicao(linha, coluna);
            const Maquina* maquina = posicao.obtemMaquina();
            ecra.escreve(linha + 2, coluna + 2, maquina != nullptr
                ? maquina->obtemSimbolo() : posicao.obtemTerreno().obterChar());
        }
    }

    const int painelLinha = 0;
    const int painelColuna = larguraMapa + 2;
    const int larguraPainel = ecra.obtemColunas() - painelColuna;

    if (alturaPainel > ecra.obtemLinhas() || larguraPainel < 5) return;
    ecra.desenhaMoldura(painelLinha, painelColuna, alturaPainel, larguraPainel);
    const int texto = painelColuna + 2;
    const std::size_t larguraTexto = larguraPainel - 3;
    ecra.escreve(1, texto, ("Instante: " + std::to_string(fabrica.obtemInstante())).substr(0, larguraTexto));
    ecra.escreve(2, texto, ("Creditos: " + std::to_string(fabrica.obtemCreditos())).substr(0, larguraTexto));
    ecra.escreve(3, texto, ("Maquinas: " + std::to_string(fabrica.obtemNumeroMaquinas())).substr(0, larguraTexto));
    ecra.escreve(4, texto, std::string("Engenheiros: 0").substr(0, larguraTexto));
    ecra.escreve(6, texto, std::string("No ultimo instante:").substr(0, larguraTexto));

    const std::vector<std::string>& eventos = fabrica.obtemEventos();
    if (eventos.empty()) ecra.escreve(7, texto, std::string("Sem eventos").substr(0, larguraTexto));
    const std::size_t limite = eventos.size() > 5 ? 4 : eventos.size();
    for (std::size_t i = 0; i < limite; ++i) {
        ecra.escreve(7 + static_cast<int>(i), texto, eventos[i].substr(0, larguraTexto));
    }
    if (eventos.size() > limite) {
        const std::string restantes = "+ " + std::to_string(eventos.size() - limite) + " eventos (estado)";
        ecra.escreve(11, texto, restantes.substr(0, larguraTexto));
    }
}

void VistaFabrica::mostraEstado(const Fabrica& fabrica, std::ostream& saida) const {
    saida << "Instante: " << fabrica.obtemInstante()
          << "\nCreditos: " << fabrica.obtemCreditos()
          << "\nMaquinas: " << fabrica.obtemNumeroMaquinas() << '\n';
    for (int i = 0; i < fabrica.obtemLinhas(); ++i) {
        for (int j = 0; j < fabrica.obtemColunas(); ++j) {
            const Maquina* maquina = fabrica.obtemPosicao(i, j).obtemMaquina();
            if (maquina == nullptr) continue;
            saida << char('a' + i) << char('a' + j) << ": ";
            maquina->descreve(saida, false);
            saida << '\n';
        }
    }
    saida << "Engenheiros: 0\nItens enviados com sucesso:\n";
    // O Lançador é comprável, mas ainda não efectua envios nesta meta.
    for (int i = 1; i < static_cast<int>(Item::Tipo::Total); ++i) {
        saida << "  " << Item(static_cast<Item::Tipo>(i)).obtemNome() << ": 0\n";
    }
    saida << "Eventos do ultimo instante:\n";
    if (fabrica.obtemEventos().empty()) saida << "  Sem eventos\n";
    for (const std::string& evento : fabrica.obtemEventos()) saida << "  " << evento << '\n';
}

bool VistaFabrica::consultaPosicoes(const Fabrica& fabrica, int linha, int coluna,
                                   int raio, std::ostream& saida) const {
    if (!fabrica.contem(linha, coluna) || raio < 0) {
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

            const Posicao& posicao = fabrica.obtemPosicao(i, j);
            saida << char('a' + i) << char('a' + j)
                  << ": " << posicao.obtemTerreno().descricao();
            const Maquina* maquina = posicao.obtemMaquina();
            if (maquina != nullptr) {
                saida << " | ";
                maquina->descreve(saida, true);
            }
            saida << '\n';
        }
    }
    return true;
}
