#ifndef TIPOS_MAQUINA_H
#define TIPOS_MAQUINA_H

#include "Maquina.h"

class Perfuradora : public Maquina {
public:
    explicit Perfuradora(char direcao);
    void preparaInstante(Terreno& terreno, long long instante,
                         std::mt19937& gerador, std::string& evento) override;

private:
    Item pendente;
    long long instanteExtracao = 0;

    void descreveConteudo(std::ostream& saida, bool detalhe) const override;
    Item obtemSaida(long long instante) const override;
    void removeSaida() override;
};

class Transportador : public Maquina {
public:
    explicit Transportador(char direcao);
    char obtemSimbolo() const override;
};

class Refinaria : public Maquina {
public:
    explicit Refinaria(char direcao);
};

class Oficina : public Maquina {
public:
    explicit Oficina(char direcao);
};

class Lancador : public Maquina {
public:
    explicit Lancador(char direcao);
};

#endif
