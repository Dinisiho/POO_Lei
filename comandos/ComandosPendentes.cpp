#include "ComandosPendentes.h"
#include "Interpretador.h"

#include <ostream>

ComandoPendente::ComandoPendente(const std::string& nome,
                                 const std::string& descricao)
    : nomeComando(nome), descricaoAjuda(descricao) {}

const std::string& ComandoPendente::nome() const { return nomeComando; }

void ComandoPendente::ajuda(std::ostream& saida) const {
    saida << descricaoAjuda;
}

void ComandoPendente::executa(Interpretador& interpretador, const std::string&,
                             std::ostream& saida) const {
    if (!interpretador.temFabrica()) {
        saida << "Erro: cria primeiro a fabrica (fabrica ou carrega).\n";
        return;
    }
    saida << "Comando '" << nomeComando
          << "': parametros validos, mas operacao ainda nao implementada.\n";
}

ComandoAvanca::ComandoAvanca()
    : ComandoPendente("avanca",
        "avanca [n]\n"
        "  n: inteiro positivo opcional (omissao: 1).\n"
        "  Avanca n instantes, apresentando a fabrica apos cada instante.\n"
        "  Falha se n nao for positivo ou nao existir fabrica.\n"
        "  Exemplo: avanca 10\n") {}

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

ComandoCompra::ComandoCompra()
    : ComandoPendente("compra",
        "compra <tipo> <lc> <dir>\n"
        "  tipo: p, t, r, o, s, l; lc: duas letras; dir: c, b, e, d.\n"
        "  Compra e coloca maquina na posicao e direcao indicadas.\n"
        "  Falha sem fabrica, posicao invalida/ocupada/cratera ou creditos insuficientes.\n"
        "  Exemplo: compra t ce d\n") {}

bool ComandoCompra::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 3, 3, erros)) return false;
    if (!tipoMaquinaValido(p[0]) || !posicaoValida(p[1]) || !direcaoValida(p[2])) {
        erros << "Erro: tipo, posicao ou direcao invalido.\n";
        return false;
    }
    return true;
}

ComandoVende::ComandoVende()
    : ComandoPendente("vende",
        "vende <lc>\n"
        "  lc: posicao de duas letras minusculas.\n"
        "  Vende a maquina, recuperando a percentagem definida em Settings.\n"
        "  Falha sem fabrica, se nao houver maquina ou a posicao for invalida.\n"
        "  Exemplo: vende ce\n") {}

bool ComandoVende::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 1, 1, erros)) return false;
    if (!posicaoValida(p[0])) {
        erros << "Erro: posicao invalida.\n";
        return false;
    }
    return true;
}

ComandoRoda::ComandoRoda()
    : ComandoPendente("roda",
        "roda <lc> <dir>\n"
        "  lc: posicao; dir: c, b, e, d.\n"
        "  Altera a direcao de saida da maquina.\n"
        "  Falha sem fabrica, sem maquina ou com direcao/posicao invalida.\n"
        "  Exemplo: roda ce b\n") {}

bool ComandoRoda::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 2, 2, erros)) return false;
    if (!posicaoValida(p[0]) || !direcaoValida(p[1])) {
        erros << "Erro: posicao ou direcao invalida.\n";
        return false;
    }
    return true;
}

ComandoMelhora::ComandoMelhora()
    : ComandoPendente("melhora",
        "melhora <lc>\n"
        "  lc: posicao de duas letras minusculas.\n"
        "  Melhora a maquina nessa posicao, se for melhoravel.\n"
        "  Falha sem fabrica, sem maquina, sem creditos ou quando nao admite melhoria.\n"
        "  Exemplo: melhora hi\n") {}

bool ComandoMelhora::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 1, 1, erros)) return false;
    if (!posicaoValida(p[0])) {
        erros << "Erro: posicao invalida.\n";
        return false;
    }
    return true;
}

ComandoContrata::ComandoContrata()
    : ComandoPendente("contrata",
        "contrata <nome>\n"
        "  nome: uma palavra para identificar o engenheiro.\n"
        "  Contrata um engenheiro, atribuindo-lhe um ID unico.\n"
        "  Falha sem fabrica ou sem creditos suficientes.\n"
        "  Exemplo: contrata joaquim\n") {}

bool ComandoContrata::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 1, 1, erros);
}

ComandoAtribui::ComandoAtribui()
    : ComandoPendente("atribui",
        "atribui <id> <lc>\n"
        "  id: numero inteiro positivo do engenheiro; lc: posicao.\n"
        "  Associa o engenheiro a uma maquina da fabrica.\n"
        "  Falha sem fabrica, engenheiro ou maquina validos.\n"
        "  Exemplo: atribui 2 hr\n") {}

bool ComandoAtribui::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 2, 2, erros)) return false;
    int id;
    if (!numeroInteiro(p[0], id) || id <= 0 || !posicaoValida(p[1])) {
        erros << "Erro: ID ou posicao invalido.\n";
        return false;
    }
    return true;
}

ComandoDespede::ComandoDespede()
    : ComandoPendente("despede",
        "despede <id>\n"
        "  id: numero inteiro positivo do engenheiro.\n"
        "  Remove o engenheiro da fabrica e deixa de pagar salario.\n"
        "  Falha sem fabrica ou com identificador inexistente.\n"
        "  Exemplo: despede 2\n") {}

bool ComandoDespede::valida(const std::string& argumentos, std::ostream& erros) const {
    const auto p = separa(argumentos);
    if (!quantidadeValida(p, 1, 1, erros)) return false;
    int id;
    if (!numeroInteiro(p[0], id) || id <= 0) {
        erros << "Erro: ID deve ser inteiro positivo.\n";
        return false;
    }
    return true;
}

ComandoEstado::ComandoEstado()
    : ComandoPendente("estado",
        "estado\n"
        "  Sem parametros.\n"
        "  Lista instante, creditos, maquinas, engenheiros e itens enviados.\n"
        "  Falha se a fabrica nao existir ou forem indicados parametros.\n"
        "  Exemplo: estado\n") {}

bool ComandoEstado::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 0, 0, erros);
}

ComandoGrava::ComandoGrava()
    : ComandoPendente("grava",
        "grava <nome>\n"
        "  nome: identificador da copia da fabrica em memoria.\n"
        "  Guarda um estado independente da fabrica.\n"
        "  Falha sem fabrica ou com parametros invalidos.\n"
        "  Exemplo: grava antes\n") {}

bool ComandoGrava::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 1, 1, erros);
}

ComandoRecupera::ComandoRecupera()
    : ComandoPendente("recupera",
        "recupera <nome>\n"
        "  nome: identificador de copia existente em memoria.\n"
        "  Recupera a copia sem a eliminar, deixando os engenheiros livres.\n"
        "  Falha sem fabrica, sem copia com o nome indicado ou com parametros invalidos.\n"
        "  Exemplo: recupera antes\n") {}

bool ComandoRecupera::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 1, 1, erros);
}
