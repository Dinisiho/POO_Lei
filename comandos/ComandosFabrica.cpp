#include "ComandosFabrica.h"
#include "Interpretador.h"
#include "../settings/Settings.h"

#include <ostream>

const std::string& ComandoFabrica::nome() const {
    static const std::string valor = "fabrica";
    return valor;
}

bool ComandoFabrica::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 2, 2, erros)) return false;
    int linhas, colunas;
    if (!numeroInteiro(p[0], linhas) || !numeroInteiro(p[1], colunas) ||
        linhas < 1 || linhas > Settings::fabrica_max_linhas ||
        colunas < 1 || colunas > Settings::fabrica_max_colunas) {
        erros << "Erro: dimensoes invalidas. Usa 'ajuda fabrica'.\n";
        return false;
    }
    return true;
}

void ComandoFabrica::executa(Interpretador& interpretador, const std::string& argumentos,
                            std::ostream& saida) const {
    const auto p = separa(argumentos);
    int linhas, colunas;
    numeroInteiro(p[0], linhas);
    numeroInteiro(p[1], colunas);
    interpretador.criaFabrica(linhas, colunas, saida);
}

void ComandoFabrica::ajuda(std::ostream& saida) const {
    saida << "fabrica <nl> <nc>\n"
          << "  nl: numero inteiro de linhas; nc: numero inteiro de colunas.\n"
          << "  Cria uma unica fabrica aleatoria com crateras e depositos de Settings.\n"
          << "  Falha se a fabrica ja existir, as dimensoes excederem Settings,\n"
          << "  ou nao houver posicoes para os terrenos especiais.\n"
          << "  Exemplo: fabrica 12 20\n";
}

const std::string& ComandoCarrega::nome() const {
    static const std::string valor = "carrega";
    return valor;
}

bool ComandoCarrega::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 1, 1, erros);
}

void ComandoCarrega::executa(Interpretador& interpretador, const std::string& argumentos,
                            std::ostream& saida) const {
    interpretador.carregaFabrica(separa(argumentos)[0], saida);
}

void ComandoCarrega::ajuda(std::ostream& saida) const {
    saida << "carrega <nome-do-ficheiro>\n"
          << "  nome-do-ficheiro: ficheiro de mapa com dimensoes e caracteres . # % $ ~.\n"
          << "  Cria a fabrica a partir de mapa.\n"
          << "  Falha se a fabrica ja existir ou o mapa nao existir/for invalido.\n"
          << "  Exemplo: carrega exemplos/mapa1.txt\n";
}

const std::string& ComandoLPosicao::nome() const {
    static const std::string valor = "lposicao";
    return valor;
}

bool ComandoLPosicao::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 1, 2, erros)) return false;
    int raio = 0;
    if (!posicaoValida(p[0]) || (p.size() == 2 &&
        (!numeroInteiro(p[1], raio) || raio < 0))) {
        erros << "Erro: posicao ou raio invalido. Usa 'ajuda lposicao'.\n";
        return false;
    }
    return true;
}

void ComandoLPosicao::executa(Interpretador& interpretador, const std::string& argumentos,
                             std::ostream& saida) const {
    const auto p = separa(argumentos);
    int raio = 0;
    if (p.size() == 2) numeroInteiro(p[1], raio);
    interpretador.consultaPosicoes(p[0][0] - 'a', p[0][1] - 'a', raio, saida);
}

void ComandoLPosicao::ajuda(std::ostream& saida) const {
    saida << "lposicao <lc> [n]\n"
          << "  lc: duas letras minusculas (linha e coluna); n: raio inteiro nao negativo.\n"
          << "  Mostra terreno, quantidade do deposito e detalhes da maquina, incluindo os itens.\n"
          << "  Com raio, lista o quadrado centrado em lc ate aos limites do mapa.\n"
          << "  Falha sem fabrica, com posicao fora da grelha ou parametros invalidos.\n"
          << "  Exemplo: lposicao df 2\n";
}
