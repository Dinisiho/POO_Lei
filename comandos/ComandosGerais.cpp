#include "ComandosGerais.h"
#include "Interpretador.h"

#include <ostream>
#include <string>
#include <vector>

const std::string& ComandoAjuda::nome() const {
    static const std::string valor = "ajuda";
    return valor;
}

bool ComandoAjuda::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 0, 1, erros);
}

void ComandoAjuda::executa(Interpretador& interpretador, const std::string& argumentos,
                          std::ostream& saida) const {
    const std::vector<std::string> parametros = separa(argumentos);
    interpretador.mostraAjuda(parametros.empty() ? "" : parametros[0], saida);
}

void ComandoAjuda::ajuda(std::ostream& saida) const {
    saida << "ajuda [comando]\n"
          << "  comando: nome opcional de um comando conhecido.\n"
          << "  Sem argumento lista os comandos; com argumento explica sintaxe, parametros e falhas.\n"
          << "  Falha com parametros a mais ou nome de comando inexistente.\n"
          << "  Exemplo: ajuda compra\n";
}

const std::string& ComandoExecuta::nome() const {
    static const std::string valor = "executa";
    return valor;
}

bool ComandoExecuta::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 1, 1, erros);
}

void ComandoExecuta::executa(Interpretador& interpretador, const std::string& argumentos,
                            std::ostream& saida) const {
    interpretador.executaFicheiro(separa(argumentos)[0], saida);
}

void ComandoExecuta::ajuda(std::ostream& saida) const {
    saida << "executa <nome-do-ficheiro>\n"
          << "  nome-do-ficheiro: caminho para ficheiro de texto com um comando por linha.\n"
          << "  Processa os comandos pelo mesmo interpretador utilizado no teclado.\n"
          << "  Falha se nao conseguir abrir/ler o ficheiro ou detetar execucao recursiva.\n"
          << "  Exemplo: executa teste1.txt\n";
}

const std::string& ComandoFim::nome() const {
    static const std::string valor = "fim";
    return valor;
}

bool ComandoFim::valida(const std::string& argumentos, std::ostream& erros) const {
    return quantidadeValida(separa(argumentos), 0, 0, erros);
}

void ComandoFim::executa(Interpretador& interpretador, const std::string&,
                        std::ostream& saida) const {
    saida << "A terminar o simulador.\n";
    interpretador.termina();
}

void ComandoFim::ajuda(std::ostream& saida) const {
    saida << "fim\n"
          << "  Sem parametros. Encerra o simulador, libertando os recursos.\n"
          << "  Falha se forem indicados parametros.\n"
          << "  Exemplo: fim\n";
}
