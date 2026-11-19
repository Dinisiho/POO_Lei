#include "TiposMaquina.h"
#include "../settings/Settings.h"
#include "../terreno/terreno.h"

#include <ostream>

Perfuradora::Perfuradora(char direcao)
    : Maquina("Perfuradora", 'P', Settings::perfuradora_custo, direcao) {}

void Perfuradora::preparaInstante(Terreno& terreno, long long instante,
                                 std::mt19937&, std::string& evento) {
    // A extracção básica fornece itens ao circuito do checkpoint.
    const int periodo = Settings::perfuradora_periodo;
    if (pendente.existe() || periodo <= 0 || instante % periodo != 0) return;
    pendente = terreno.extrai();
    if (!pendente.existe()) return;
    instanteExtracao = instante;
    evento = std::string("Extraiu ") + pendente.obtemNome();
}

Item Perfuradora::obtemSaida(long long instante) const {
    return instanteExtracao < instante ? pendente : Item();
}

void Perfuradora::removeSaida() { pendente = Item(); }

void Perfuradora::descreveConteudo(std::ostream& saida, bool detalhe) const {
    saida << (pendente.existe() ? pendente.descricao() : "vazia");
    if (detalhe) saida << " | periodo de extracao: " << Settings::perfuradora_periodo;
}

Transportador::Transportador(char direcao)
    : Maquina("Transportador", '>', Settings::transportador_custo, direcao) {}

char Transportador::obtemSimbolo() const {
    switch (obtemDirecao()) {
        case 'c': return '^';
        case 'b': return 'v';
        case 'e': return '<';
        default: return '>';
    }
}

Refinaria::Refinaria(char direcao)
    : Maquina("Refinaria", 'R', Settings::refinaria_custo, direcao) {}

Oficina::Oficina(char direcao)
    : Maquina("Oficina", 'O', Settings::oficina_custo, direcao) {}

Lancador::Lancador(char direcao)
    : Maquina("Lancador", 'L', Settings::lancador_custo, direcao) {}
