#ifndef ECRA_H
#define ECRA_H

#include <string>
using namespace std;

class Ecra {
public:
    // Construtor: define as dimensões úteis do ecrã virtual (com valores por omissão para os máximos)
    Ecra(int nLinhas = MAX_LINHAS, int nColunas = MAX_COLUNAS);

    // Limpa o buffer interno preenchendo-o com espaços e coloca o cursor na posição (0, 0)
    void limpa();

    // Posiciona o cursor virtual numa linha e coluna específicas
    void posiciona(int l, int c);

    // Sobrecargas de escrita a partir da posição atual do cursor virtual
    void escreve(char c);
    void escreve(const string& texto);

    // Sobrecargas de escrita direta em coordenadas arbitrárias
    void escreve(int l, int c, char var);
    void escreve(int l, int c, const string& texto);

    // Desenha uma moldura retangular com caracteres de canto e linha
    void desenhaMoldura(int l, int c, int alt, int larg);

    // Envia todo o conteúdo do buffer para a consola de uma só vez
    void mostra() const;

    // Métodos de consulta de dimensões
    int obtemLinhas() const;
    int obtemColunas() const;

private:
    // Constantes físicas máximas do buffer definidas no enunciado
    static const int MAX_LINHAS = 40;
    static const int MAX_COLUNAS = 80;

    // Buffer bidimensional estático (proibido o uso de contentores standard)
    char buffer[MAX_LINHAS][MAX_COLUNAS]{};

    // Dimensões ativas em uso na simulação
    int linhasUsadas;
    int colunasUsadas;

    // Posição atual do cursor virtual
    int cursorLin;
    int cursorCol;

    // Função membro auxiliar privada para validação de limites
    bool posicaoValida(int l, int c) const;
};

#endif // ECRA_H

