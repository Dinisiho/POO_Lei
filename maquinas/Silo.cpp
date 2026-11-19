#include "Silo.h"
#include "../settings/Settings.h"

#include <ostream>

Silo::Silo(char direcao) : Maquina("Silo", 'S', Settings::silo_custo, direcao) {}

Silo::~Silo() {
    while (primeiro != nullptr) removeSaida();
}

bool Silo::recebe(Item item, long long instante) {
    if (!item.existe()) return false;
    No* novo = new No{item, instante, nullptr};
    if (ultimo == nullptr) primeiro = novo;
    else ultimo->seguinte = novo;
    ultimo = novo;
    ++quantidade;
    return true;
}

Item Silo::obtemSaida(long long instante) const {
    if (primeiro == nullptr || primeiro->instanteEntrada >= instante) return {};
    return primeiro->item;
}

void Silo::removeSaida() {
    No* retirado = primeiro;
    primeiro = primeiro->seguinte;
    if (primeiro == nullptr) ultimo = nullptr;
    delete retirado;
    --quantidade;
}

void Silo::preparaInstante(Terreno&, long long instante,
                           std::mt19937& gerador, std::string& evento) {
    const Item item = obtemSaida(instante);
    if (!item.existe()) return;
    std::uniform_int_distribution<int> sorteia(1, 100);
    if (sorteia(gerador) > Settings::silo_prob_perda) return;
    evento = std::string("Perdeu ") + item.obtemNome();
    removeSaida();
}

void Silo::descreveConteudo(std::ostream& saida, bool detalhe) const {
    saida << quantidade << " item(ns)";
    if (!detalhe && primeiro != nullptr) saida << ", mais antigo: " << primeiro->item.obtemNome();
    if (detalhe) {
        saida << " | perda por instante: " << Settings::silo_prob_perda << '%';
        for (const No* no = primeiro; no != nullptr; no = no->seguinte) {
            saida << " | " << no->item.descricao();
        }
    }
}
