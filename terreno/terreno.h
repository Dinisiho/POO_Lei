//
// Created by admin on 10/26/2026.
//

#ifndef TERRENO_H
#define TERRENO_H

class Item;
using namespace std;

class Terreno {
    public:
    //deconstrutor virtual
    virtual ~Terreno() = default;
    virtual char obterChar() const = 0;
    virtual bool permiteMaquina() const = 0;
    virtual Item* extraiMateriaP() {
        return nullptr;
    };
    virtual bool esgotado() const {
        return false;
    }
};
class Solo : public Terreno {
    public:
    char obterChar() const override;
    bool permiteMaquina() const override;
};
class Cratera : public Terreno {
    public:
    char obterChar() const override;
    bool permiteMaquina() const override;
};

class Deposito : public Terreno {
    public:
    Deposito(char tipo, int qInicial);
    char obterChar() const override;
    bool permiteMaquina() const override;
    bool esgotado() const override;
    Item* extraiMateriaP() override;

    private:
    char tipoRecurso;
    int qRestante;

};

#endif //TERRENO_H
