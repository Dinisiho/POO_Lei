#ifndef SILO_H
#define SILO_H

#include "Maquina.h"
#include <cstddef>

class Silo : public Maquina {
public:
    explicit Silo(char direcao);
    ~Silo() override;
    void preparaInstante(Terreno& terreno, long long instante,
                         std::mt19937& gerador, std::string& evento) override;

private:
    struct No {
        Item item;
        long long instanteEntrada;
        No* seguinte;
    };

    No* primeiro = nullptr;
    No* ultimo = nullptr;
    std::size_t quantidade = 0;

    void descreveConteudo(std::ostream& saida, bool detalhe) const override;
    Item obtemSaida(long long instante) const override;
    bool recebe(Item item, long long instante) override;
    void removeSaida() override;
};

#endif
