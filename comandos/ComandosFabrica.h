#ifndef COMANDOS_FABRICA_H
#define COMANDOS_FABRICA_H

#include "Comando.h"

class ComandoFabrica : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoCarrega : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoLPosicao : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

#endif
