#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include <cstddef>
#include <fstream>
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

    Fabrica& obtemFabrica();
    void criaFabrica(int linhas, int colunas, std::ostream& saida);
    void carregaFabrica(const std::string& caminho, std::ostream& saida);
    void consultaPosicoes(int linha, int coluna, int raio, std::ostream& saida) const;
    void mostraFabrica(std::ostream& saida) const;

    void termina();
    bool terminou() const;

private:
    struct FicheiroComandos {
        std::string caminho;
        std::ifstream entrada;
        std::size_t numeroLinha = 0;
    };

    std::vector<Comando*> comandos;
    std::vector<FicheiroComandos> ficheirosEmExecucao;
    Fabrica* fabrica;
    bool terminar;

    const Comando* procuraComando(const std::string& nome) const;
};

#endif
