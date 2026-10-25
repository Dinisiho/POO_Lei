#include "Ecra.h"
#include <iostream>

using namespace std;

Ecra::Ecra(int nLinhas, int nColunas)
    : linhasUsadas(nLinhas), colunasUsadas(nColunas), cursorLin(0), cursorCol(0) {
    // Garante que as dimensões úteis nunca ultrapassam os máximos físicos permitidos
    if (linhasUsadas < 1) linhasUsadas = 1;
    if (linhasUsadas > MAX_LINHAS) linhasUsadas = MAX_LINHAS;

    if (colunasUsadas < 1) colunasUsadas = 1;
    if (colunasUsadas > MAX_COLUNAS) colunasUsadas = MAX_COLUNAS;

    limpa();
}

void Ecra::limpa() {
    for (int i = 0; i < linhasUsadas; ++i) {
        for (int j = 0; j < colunasUsadas; ++j) {
            buffer[i][j] = ' ';
        }
    }
    cursorLin = 0;
    cursorCol = 0;
}

void Ecra::posiciona(int l, int c) {
    if (posicaoValida(l, c)) {
        cursorLin = l;
        cursorCol = c;
    }
}

void Ecra::escreve(char c) {
    // Se o cursor estiver fora dos limites úteis, a escrita é ignorada silenciosamente
    if (!posicaoValida(cursorLin, cursorCol)) {
        return;
    }

    buffer[cursorLin][cursorCol] = c;
    cursorCol++;

    // Se atingir o fim da linha útil, salta para a linha seguinte
    if (cursorCol >= colunasUsadas) {
        cursorCol = 0;
        cursorLin++;
    }
}

void Ecra::escreve(const string& texto) {
    for (char c : texto) {
        if (c == '\n') {
            cursorCol = 0;
            cursorLin++;
        } else {
            escreve(c);
        }
    }
}

void Ecra::escreve(int l, int c, char car) {
    posiciona(l, c);
    escreve(car);
}

void Ecra::escreve(int l, int c, const string& texto) {
    posiciona(l, c);
    escreve(texto);
}

void Ecra::desenhaMoldura(int l, int c, int alt, int larg) {
    if (alt <= 0 || larg <= 0) return;

    // Cantos
    escreve(l, c, '+');
    escreve(l, c + larg - 1, '+');
    escreve(l + alt - 1, c, '+');
    escreve(l + alt - 1, c + larg - 1, '+');

    // Linhas horizontais (superior e inferior)
    for (int j = c + 1; j < c + larg - 1; ++j) {
        escreve(l, j, '-');
        escreve(l + alt - 1, j, '-');
    }

    // Linhas verticais (esquerda e direita)
    for (int i = l + 1; i < l + alt - 1; ++i) {
        escreve(i, c, '|');
        escreve(i, c + larg - 1, '|');
    }
}

void Ecra::mostra() const {
    for (int i = 0; i < linhasUsadas; ++i) {
        for (int j = 0; j < colunasUsadas; ++j) {
            cout << buffer[i][j];
        }
        cout << '\n';
    }
}

int Ecra::obtemLinhas() const {
    return linhasUsadas;
}

int Ecra::obtemColunas() const {
    return colunasUsadas;
}

bool Ecra::posicaoValida(int l, int c) const {
    return (l >= 0 && l < linhasUsadas && c >= 0 && c < colunasUsadas);
}