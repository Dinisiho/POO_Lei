#ifndef COMANDOS_PENDENTES_H
#define COMANDOS_PENDENTES_H

#include "Comando.h"

// Partilha apenas o comportamento de operacoes ainda nao implementadas.
class ComandoPendente : public Comando {
public:
    ComandoPendente(const std::string& nome, const std::string& descricao);
    const std::string& nome() const override;
    void executa(Interpretador& interpretador, const std::string& argumentos,
                std::ostream& saida) const override;
    void ajuda(std::ostream& saida) const override;
    bool estaPendente() const override;

private:
    std::string nomeComando;
    std::string descricaoAjuda;
};

class ComandoMelhora : public ComandoPendente {
public:
    ComandoMelhora();
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
};

class ComandoContrata : public ComandoPendente {
public:
    ComandoContrata();
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
};

class ComandoAtribui : public ComandoPendente {
public:
    ComandoAtribui();
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
};

class ComandoDespede : public ComandoPendente {
public:
    ComandoDespede();
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
};

class ComandoGrava : public ComandoPendente {
public:
    ComandoGrava();
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
};

class ComandoRecupera : public ComandoPendente {
public:
    ComandoRecupera();
    bool valida(const std::string& argumentos, std::ostream& erros) const override;
};

#endif
