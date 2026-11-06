#ifndef POSICAO_H
#define POSICAO_H

class Terreno;
class Maquina;

class Posicao {
public:
    explicit Posicao(Terreno& terreno);

    Terreno& obtemTerreno();
    const Terreno& obtemTerreno() const;
    Maquina* obtemMaquina() const;

    bool podeReceberMaquina() const;
    bool colocaMaquina(Maquina* maquina);
    void removeMaquina();

private:
    Terreno* terreno;
    Maquina* maquina;
};

#endif
