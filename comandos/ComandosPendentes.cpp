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

void ComandoPendente::executa(Interpretador&, const std::string&,
                             std::ostream& saida) const {
    saida << "Comando '" << nomeComando
          << "': parametros validos. Operacao reservada para a entrega final.\n";
}

ComandoMelhora::ComandoMelhora()
    : ComandoPendente("melhora",
        "melhora <lc>\n"
        "  lc: posicao de duas letras minusculas.\n"
        "  Reduz perdas na perfuradora, silo ou lancador, ou uma vez a duracao da oficina.\n"
        "  Custos e reducoes em Settings. Falha sem fabrica, maquina, saldo ou melhoria disponivel.\n"
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
        "  Contrata um engenheiro livre, com ID unico e salario por instante. O nome pode repetir-se.\n"
        "  Falha sem fabrica ou sem creditos suficientes.\n"
        "  Exemplo: contrata joaquim\n") {}

bool ComandoContrata::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 1, 1, erros);
}

ComandoAtribui::ComandoAtribui()
    : ComandoPendente("atribui",
        "atribui <id> <lc>\n"
        "  id: numero inteiro positivo do engenheiro; lc: posicao.\n"
        "  Associa o engenheiro a uma maquina da fabrica, retirando-o da anterior, se existir.\n"
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

ComandoGrava::ComandoGrava()
    : ComandoPendente("grava",
        "grava <nome>\n"
        "  nome: identificador da copia da fabrica em memoria.\n"
        "  Guarda em memoria uma copia independente da fabrica, sem associacoes dos engenheiros.\n"
        "  Substitui a copia anterior se o nome ja existir.\n"
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
