#ifndef ECRA_H
#define ECRA_H

#include <string>

class Ecra {
public:
    // Construtor com dimensões por omissão
    Ecra(int nLinhas = MAX_LINHAS, int nColunas = MAX_COLUNAS);

    // Limpa o conteúdo do ecrã
    void limpa();

    // Posiciona o cursor virtual
    void posiciona(int linha, int coluna);

    // Escreve a partir da posição atual do cursor
    void escreve(char caracter);
    void escreve(const std::string& texto);

    // Escreve numa posição específica
    void escreve(int linha, int coluna, char caracter);
    void escreve(int linha, int coluna, const std::string& texto);

    // Desenha uma moldura retangular
    void desenhaMoldura(int linha, int coluna, int altura, int largura);

    // Apresenta o conteúdo do buffer na consola
    void mostra() const;

    // Consulta as dimensões utilizadas
    int obtemLinhas() const;
    int obtemColunas() const;

private:
    static const int MAX_LINHAS = 40;
    static const int MAX_COLUNAS = 80;

    char buffer[MAX_LINHAS][MAX_COLUNAS];

    int linhasUsadas;
    int colunasUsadas;

    int cursorLinha;
    int cursorColuna;

    // Verifica se uma posição está dentro dos limites
    bool posicaoValida(int linha, int coluna) const;
};

#endif
