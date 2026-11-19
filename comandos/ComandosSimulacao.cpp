#include "ComandosSimulacao.h"
#include "Interpretador.h"
#include "../fabrica/Fabrica.h"
#include "../vista/VistaFabrica.h"

#include <ostream>

namespace {
    void apresentaAlteracao(Interpretador& interpretador, const char* erro,
                            const char* confirmacao, std::ostream& saida) {
        if (erro != nullptr) {
            saida << "Erro: " << erro << '\n';
            return;
        }
        saida << confirmacao << '\n';
        interpretador.mostraFabrica(saida);
    }
}

const std::string& ComandoCompra::nome() const {
    static const std::string valor = "compra";
    return valor;
}

bool ComandoCompra::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 3, 3, erros)) return false;
    if (!tipoMaquinaValido(p[0]) || !posicaoValida(p[1]) || !direcaoValida(p[2])) {
        erros << "Erro: tipo, posicao ou direcao invalido.\n";
        return false;
    }
    return true;
}

void ComandoCompra::executa(Interpretador& interpretador, const std::string& argumentos,
                            std::ostream& saida) const {
    const auto p = separa(argumentos);
    const char* erro = interpretador.obtemFabrica().compra(
        p[0][0], p[1][0] - 'a', p[1][1] - 'a', p[2][0]);
    apresentaAlteracao(interpretador, erro, "Maquina comprada.", saida);
}

void ComandoCompra::ajuda(std::ostream& saida) const {
    saida << "compra <tipo> <lc> <dir>\n"
          << "  tipo: p perfuradora, t transportador, r refinaria, o oficina, s silo, l lancador.\n"
          << "  lc: duas letras (linha e coluna); dir: c cima, b baixo, e esquerda, d direita.\n"
          << "  Coloca a maquina e desconta o custo definido em Settings.\n"
          << "  Falha sem fabrica, sem saldo ou com posicao fora do mapa, ocupada ou cratera.\n"
          << "  Exemplo: compra t ce d\n";
}

const std::string& ComandoVende::nome() const {
    static const std::string valor = "vende";
    return valor;
}

bool ComandoVende::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 1, 1, erros)) return false;
    if (!posicaoValida(p[0])) {
        erros << "Erro: posicao invalida.\n";
        return false;
    }
    return true;
}

void ComandoVende::executa(Interpretador& interpretador, const std::string& argumentos,
                           std::ostream& saida) const {
    const auto p = separa(argumentos);
    const char* erro = interpretador.obtemFabrica().vende(p[0][0] - 'a', p[0][1] - 'a');
    apresentaAlteracao(interpretador, erro, "Maquina vendida.", saida);
}

void ComandoVende::ajuda(std::ostream& saida) const {
    saida << "vende <lc>\n"
          << "  lc: duas letras (linha e coluna).\n"
          << "  Remove a maquina e os seus itens e devolve a percentagem do custo de Settings.\n"
          << "  Falha sem fabrica, com posicao fora do mapa ou sem maquina nessa posicao.\n"
          << "  Exemplo: vende ce\n";
}

const std::string& ComandoRoda::nome() const {
    static const std::string valor = "roda";
    return valor;
}

bool ComandoRoda::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 2, 2, erros)) return false;
    if (!posicaoValida(p[0]) || !direcaoValida(p[1])) {
        erros << "Erro: posicao ou direcao invalida.\n";
        return false;
    }
    return true;
}

void ComandoRoda::executa(Interpretador& interpretador, const std::string& argumentos,
                          std::ostream& saida) const {
    const auto p = separa(argumentos);
    const char* erro = interpretador.obtemFabrica().roda(p[0][0] - 'a', p[0][1] - 'a', p[1][0]);
    apresentaAlteracao(interpretador, erro, "Direcao atualizada.", saida);
}

void ComandoRoda::ajuda(std::ostream& saida) const {
    saida << "roda <lc> <dir>\n"
          << "  lc: duas letras (linha e coluna); dir: c cima, b baixo, e esquerda, d direita.\n"
          << "  Muda a direcao de saida. Falha sem fabrica ou sem maquina na posicao indicada.\n"
          << "  Exemplo: roda ce b\n";
}

const std::string& ComandoAvanca::nome() const {
    static const std::string valor = "avanca";
    return valor;
}

bool ComandoAvanca::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 0, 1, erros)) return false;
    int n;
    if (!p.empty() && (!numeroInteiro(p[0], n) || n <= 0)) {
        erros << "Erro: numero de instantes deve ser inteiro positivo.\n";
        return false;
    }
    return true;
}

void ComandoAvanca::executa(Interpretador& interpretador, const std::string& argumentos,
                            std::ostream& saida) const {
    const auto p = separa(argumentos);
    int n = 1;
    if (!p.empty()) numeroInteiro(p[0], n);
    for (int i = 0; i < n; ++i) {
        interpretador.obtemFabrica().avanca();
        interpretador.mostraFabrica(saida);
    }
}

void ComandoAvanca::ajuda(std::ostream& saida) const {
    saida << "avanca [n]\n"
          << "  n: inteiro positivo opcional (por omissao: 1).\n"
          << "  Executa n instantes, com atuacao das maquinas e novo ecra em cada um.\n"
          << "  Falha sem fabrica, com n invalido ou com argumentos a mais.\n"
          << "  Exemplo: avanca 10\n";
}

const std::string& ComandoEstado::nome() const {
    static const std::string valor = "estado";
    return valor;
}

bool ComandoEstado::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 0, 0, erros);
}

void ComandoEstado::executa(Interpretador& interpretador, const std::string&,
                            std::ostream& saida) const {
    VistaFabrica().mostraEstado(interpretador.obtemFabrica(), saida);
}

void ComandoEstado::ajuda(std::ostream& saida) const {
    saida << "estado\n"
          << "  Sem parametros. Lista instante, saldo, maquinas com posicao, direcao e conteudo,\n"
          << "  engenheiros e associacoes, envios com sucesso por tipo e eventos recentes.\n"
          << "  Falha sem fabrica ou com argumentos a mais.\n"
          << "  Exemplo: estado\n";
}
