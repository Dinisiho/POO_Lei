#ifndef TERRENO_H
#define TERRENO_H

#include <string>

class Item;

class Terreno {
public:
    virtual ~Terreno() = default;

    virtual char obterChar() const = 0;
    virtual bool permiteMaquina() const = 0;
    virtual std::string descricao() const = 0;

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
    std::string descricao() const override;
};

class Cratera : public Terreno {
public:
    char obterChar() const override;
    bool permiteMaquina() const override;
    std::string descricao() const override;
};

class Deposito : public Terreno {
public:
    Deposito(char tipo, int qInicial);

    char obterChar() const override;
    bool permiteMaquina() const override;
    std::string descricao() const override;
    bool esgotado() const override;
    Item* extraiMateriaP() override;
    int obtemQuantidade() const;

private:
    char tipoRecurso;
    int qRestante;
};

#endif
