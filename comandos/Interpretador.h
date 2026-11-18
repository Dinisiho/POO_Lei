#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include <iosfwd>
#include <string>
#include <vector>

class Comando;
class Fabrica;

class Interpretador {
public:
    Interpretador();
    ~Interpretador();

    Interpretador(const Interpretador&) = delete;
    Interpretador& operator=(const Interpretador&) = delete;

    void processaLinha(const std::string& linha, std::ostream& saida);
    void executaFicheiro(const std::string& caminho, std::ostream& saida);
    void mostraAjuda(const std::string& nome, std::ostream& saida) const;

    bool temFabrica() const;
    void criaFabrica(int linhas, int colunas, std::ostream& saida);
    void carregaFabrica(const std::string& caminho, std::ostream& saida);
    void consultaPosicoes(int linha, int coluna, int raio, std::ostream& saida) const;
    void mostraFabrica() const;

    void termina();
    bool terminou() const;

private:
    std::vector<Comando*> comandos;
    std::vector<std::string> ficheirosEmExecucao;
    Fabrica* fabrica;
    bool terminar;

    const Comando* procuraComando(const std::string& nome) const;
};

#endif
