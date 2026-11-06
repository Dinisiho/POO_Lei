#include "Ecra.h"
#include "../settings/Settings.h"
#include <iostream>

// Construtor
Ecra::Ecra(int nLinhas, int nColunas)
    : linhasUsadas(nLinhas),
      colunasUsadas(nColunas),
      cursorLinha(0),
      cursorColuna(0) {
    // Garantir que as dimensões respeitam os limites do buffer
    if (linhasUsadas < 1)
        linhasUsadas = 1;

    if (linhasUsadas > Settings::ecra_max_linhas)
        linhasUsadas = MAX_LINHAS;

    if (colunasUsadas < 1)
        colunasUsadas = 1;

    if (colunasUsadas > Settings::ecra_max_colunas)
        colunasUsadas = MAX_COLUNAS;

    limpa();
}

// Limpar o ecrã
void Ecra::limpa() {
    for (int i = 0; i < MAX_LINHAS; ++i) {
        for (int j = 0; j < MAX_COLUNAS; ++j) {
            buffer[i][j] = ' ';
        }
    }

    cursorLinha = 0;
    cursorColuna = 0;
}

// Posicionar o cursor
void Ecra::posiciona(int linha, int coluna) {
    cursorLinha = linha;
    cursorColuna = coluna;
}

// Escrever um caracter na posição atual
void Ecra::escreve(char caracter) {
    if (!posicaoValida(cursorLinha, cursorColuna))
        return;

    buffer[cursorLinha][cursorColuna] = caracter;

    ++cursorColuna;

    // Passar para a linha seguinte quando chegar ao fim
    // da linha atual
    if (cursorColuna >= colunasUsadas) {
        cursorColuna = 0;
        ++cursorLinha;
    }
}

// Escrever uma string na posição atual
void Ecra::escreve(const std::string& texto) {
    for (char caracter : texto) {
        // Tratar a mudança de linha
        if (caracter == '\n') {
            cursorColuna = 0;
            ++cursorLinha;
        } else {
            escreve(caracter);
        }
    }
}

// Escrever um caracter numa posição específica
void Ecra::escreve(int linha, int coluna, char caracter) {
    if (!posicaoValida(linha, coluna))
        return;

    posiciona(linha, coluna);
    escreve(caracter);
}

// Escrever uma string numa posição específica
void Ecra::escreve(int linha, int coluna, const std::string& texto) {
    if (!posicaoValida(linha, coluna))
        return;

    posiciona(linha, coluna);
    escreve(texto);
}

// Desenhar uma moldura retangular
void Ecra::desenhaMoldura(int linha, int coluna, int altura, int largura) {
    if (altura <= 0 || largura <= 0)
        return;

    const int ultimaLinha = linha + altura - 1;
    const int ultimaColuna = coluna + largura - 1;

    // Desenhar as linhas horizontais
    for (int j = 0; j < colunasUsadas; ++j) {
        if (linha >= 0 && linha < linhasUsadas &&
            j >= coluna && j <= ultimaColuna) {
            if (j == coluna || j == ultimaColuna)
                buffer[linha][j] = '+';
            else
                buffer[linha][j] = '-';
        }

        if (ultimaLinha >= 0 && ultimaLinha < linhasUsadas &&
            j >= coluna && j <= ultimaColuna) {
            if (j == coluna || j == ultimaColuna)
                buffer[ultimaLinha][j] = '+';
            else
                buffer[ultimaLinha][j] = '-';
        }
    }

    // Desenhar as linhas verticais
    for (int i = 0; i < linhasUsadas; ++i) {
        if (i > linha && i < ultimaLinha) {
            if (coluna >= 0 && coluna < colunasUsadas)
                buffer[i][coluna] = '|';

            if (ultimaColuna >= 0 && ultimaColuna < colunasUsadas)
                buffer[i][ultimaColuna] = '|';
        }
    }
}

// Apresentar o conteúdo do buffer
void Ecra::mostra() const {
    for (int i = 0; i < linhasUsadas; ++i) {
        for (int j = 0; j < colunasUsadas; ++j) {
            std::cout << buffer[i][j];
        }

        std::cout << '\n';
    }
}

// Obter o número de linhas utilizadas
int Ecra::obtemLinhas() const {
    return linhasUsadas;
}

// Obter o número de colunas utilizadas
int Ecra::obtemColunas() const {
    return colunasUsadas;
}

// Verificar se uma posição está dentro dos limites
bool Ecra::posicaoValida(int linha, int coluna) const {
    return linha >= 0 &&
        linha < linhasUsadas &&
        coluna >= 0 &&
        coluna < colunasUsadas;
}
