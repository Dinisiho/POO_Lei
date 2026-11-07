#ifndef TERRENO_H
#define TERRENO_H

#include <string>

class Terreno {
public:
    virtual ~Terreno() = default;

    virtual char obterChar() const = 0;
    virtual std::string descricao() const = 0;
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
};

class Deposito : public Terreno {
public:
    Deposito(char tipo, int qInicial);

    char obterChar() const override;
    std::string descricao() const override;
    int obtemQuantidade() const;

private:
    char tipoRecurso;
    int qRestante;
};

#endif
