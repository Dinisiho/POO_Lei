#ifndef MAQUINA_H
#define MAQUINA_H

#include "../itens/Item.h"
#include <iosfwd>
#include <random>
#include <string>

class Terreno;

class Maquina {
public:
    virtual ~Maquina() = default;
    Maquina(const Maquina&) = delete;
    Maquina& operator=(const Maquina&) = delete;

    static Maquina* cria(char tipo, char direcao);
    static bool direcaoValida(char direcao);
    char obtemDirecao() const;
    int obtemCusto() const;
    void roda(char novaDirecao);
    virtual char obtemSimbolo() const;
    void descreve(std::ostream& saida, bool detalhe) const;

    virtual void preparaInstante(Terreno& terreno, long long instante,
                                 std::mt19937& gerador, std::string& evento);
    Item itemParaEntregar(long long instante) const;
    bool tentaEntregar(Maquina& destino, long long instante);

protected:
    Maquina(const std::string& nome, char simbolo, int custo, char direcao);
    virtual void descreveConteudo(std::ostream& saida, bool detalhe) const;

private:
    std::string nome;
    char simbolo;
    int custo;
    char direcao;

    virtual Item obtemSaida(long long instante) const;
    virtual bool recebe(Item item, long long instante);
    virtual void removeSaida();
};

#endif
