#include "Item.h"
#include "../settings/Settings.h"

namespace {
    struct DadosItem {
        std::string nome;
        char simbolo;
        int valor;
    };

    const DadosItem dados[] = {
        {"Vazio", ' ', 0},
        {"Minerio de ferro", 'f', Settings::valor_minerio_ferro},
        {"Silicio bruto", 's', Settings::valor_silicio_bruto},
        {"Gelo", 'g', Settings::valor_gelo},
        {"Placa de ferro", 'p', Settings::valor_placa_ferro},
        {"Silicio puro", 'i', Settings::valor_silicio_puro},
        {"Agua", 'a', Settings::valor_agua},
        {"Kit de habitat", 'k', Settings::valor_kit_habitat}
    };
}

Item::Item(Tipo tipo) : tipo(tipo) {}

bool Item::existe() const {
    return tipo > Tipo::Vazio && tipo < Tipo::Total;
}

std::string Item::obtemNome() const {
    return existe() ? dados[static_cast<int>(tipo)].nome : dados[0].nome;
}

std::string Item::descricao() const {
    if (!existe()) return obtemNome();
    const DadosItem& info = dados[static_cast<int>(tipo)];
    return info.nome + " (" + info.simbolo + ", " +
           std::to_string(info.valor) + " creditos)";
}
