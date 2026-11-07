#ifndef POSICAO_H
#define POSICAO_H

class Terreno;

class Posicao {
public:
    explicit Posicao(Terreno& terreno);

    Terreno& obtemTerreno();
    const Terreno& obtemTerreno() const;

private:
    Terreno* terreno;
};

#endif
