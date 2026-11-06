#ifndef TERRENO_H
#define TERRENO_H

class Item;

class Terreno {
public:
    virtual ~Terreno() = default;

    virtual char obterChar() const = 0;
    virtual bool permiteMaquina() const = 0;

    // A extração será implementada quando existir a classe Item.
    virtual Item* extraiMateriaP() {
        return nullptr;
    }

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

#endif
