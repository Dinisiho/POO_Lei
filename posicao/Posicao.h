#ifndef POSICAO_H
#define POSICAO_H

class Terreno;
class Maquina;
class Fabrica;

class Posicao {
public:
    Posicao(char tipoTerreno, int quantidade);
    ~Posicao();
    Posicao(const Posicao&) = delete;
    Posicao& operator=(const Posicao&) = delete;

    Terreno& obtemTerreno();
    const Terreno& obtemTerreno() const;
    Maquina* obtemMaquina();
    const Maquina* obtemMaquina() const;

private:
    friend class Fabrica;
    Terreno* terreno;
    Maquina* maquina = nullptr;

    void instalaMaquina(Maquina* nova);
    void removeMaquina();
    bool transformaDepositoEsgotado();
};

#endif
