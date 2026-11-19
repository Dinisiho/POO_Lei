#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
    enum class Tipo { Vazio, Ferro, Silicio, Gelo, Placa, SilicioPuro, Agua, Kit, Total };

    Item() = default;
    explicit Item(Tipo tipo);
    bool existe() const;
    const char* obtemNome() const;
    std::string descricao() const;

private:
    Tipo tipo = Tipo::Vazio;
};

#endif
