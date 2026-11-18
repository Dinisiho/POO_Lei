#ifndef COMANDOS_GERAIS_H
#define COMANDOS_GERAIS_H

#include "Comando.h"

class ComandoAjuda : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoExecuta : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoFim : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

#endif
