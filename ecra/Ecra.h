#ifndef ECRA_H
#define ECRA_H

#include <iosfwd>
#include <string>
#include "../settings/Settings.h"

class Ecra {
public:
    // Construtor com dimensões por omissão
    Ecra(int nLinhas = Settings::ecra_max_linhas,
         int nColunas = Settings::ecra_max_colunas);

    // Limpa o conteúdo do ecrã
    void limpa();

    // Posiciona o cursor virtual
    void posiciona(int linha, int coluna);

    // Escreve a partir da posição atual do cursor
    void escreve(char caracter);

    void escreve(const std::string &texto);

    // Escreve numa posição específica
    void escreve(int linha, int coluna, char caracter);

    void escreve(int linha, int coluna, const std::string &texto);

    // Desenha uma moldura retangular
    void desenhaMoldura(long long linha, long long coluna, int altura, int largura);

    // Apresenta o conteúdo do buffer na consola
    void mostra(std::ostream& saida) const;

    // Consulta as dimensões utilizadas
    int obtemLinhas() const;

    int obtemColunas() const;

private:
    char buffer[Settings::ecra_max_linhas][Settings::ecra_max_colunas];

    int linhasUsadas;
    int colunasUsadas;

    int cursorLinha;
    int cursorColuna;

    // Verifica se uma posição está dentro dos limites
    bool posicaoValida(int linha, int coluna) const;
};

#endif
