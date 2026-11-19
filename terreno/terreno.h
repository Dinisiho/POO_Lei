#ifndef TERRENO_H
#define TERRENO_H

#include "../itens/Item.h"
#include <string>

class Terreno {
public:
    virtual ~Terreno() = default;

    virtual char obterChar() const = 0;
    virtual std::string descricao() const = 0;
    virtual bool permiteMaquina() const;
    virtual Item extrai();
    virtual bool esgotado() const;
};

class Solo : public Terreno {
public:
    char obterChar() const override;
    std::string descricao() const override;
};

class Cratera : public Terreno {
public:
    char obterChar() const override;
    std::string descricao() const override;
    bool permiteMaquina() const override;
};

class Deposito : public Terreno {
public:
    Deposito(char tipo, int qInicial);

    char obterChar() const override;
    std::string descricao() const override;
    int obtemQuantidade() const;
    Item extrai() override;
    bool esgotado() const override;

private:
    char tipoRecurso;
    int qRestante;
};

#endif
