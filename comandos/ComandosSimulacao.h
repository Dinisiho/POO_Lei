#ifndef COMANDOS_SIMULACAO_H
#define COMANDOS_SIMULACAO_H

#include "Comando.h"

class ComandoCompra : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                 std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoVende : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                 std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoRoda : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                 std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoAvanca : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                 std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

class ComandoEstado : public Comando {
public:
    const std::string& nome() const override;
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                 std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
};

#endif
