#include "Maquina.h"
#include "Silo.h"
#include "TiposMaquina.h"

#include <ostream>

Maquina::Maquina(const std::string& nome, char simbolo, int custo, char direcao)
    : nome(nome), simbolo(simbolo), custo(custo), direcao(direcao) {}

Maquina* Maquina::cria(char tipo, char direcao) {
    if (!direcaoValida(direcao)) return nullptr;
    switch (tipo) {
        case 'p': return new Perfuradora(direcao);
        case 't': return new Transportador(direcao);
        case 'r': return new Refinaria(direcao);
        case 'o': return new Oficina(direcao);
        case 's': return new Silo(direcao);
        case 'l': return new Lancador(direcao);
        default: return nullptr;
    }
}

bool Maquina::direcaoValida(char direcao) {
    return direcao == 'c' || direcao == 'b' || direcao == 'e' || direcao == 'd';
}

char Maquina::obtemDirecao() const { return direcao; }
int Maquina::obtemCusto() const { return custo; }
char Maquina::obtemSimbolo() const { return simbolo; }

void Maquina::roda(char novaDirecao) {
    if (direcaoValida(novaDirecao)) direcao = novaDirecao;
}

void Maquina::descreve(std::ostream& saida, bool detalhe) const {
    saida << nome << " | direcao: " << direcao << " | engenheiros: nenhum";
    if (detalhe) saida << " | custo de compra: " << custo;
    saida << " | ";
    descreveConteudo(saida, detalhe);
}

void Maquina::descreveConteudo(std::ostream& saida, bool) const {
    saida << "vazia";
}

void Maquina::preparaInstante(Terreno&, long long, std::mt19937&, std::string&) {}
Item Maquina::obtemSaida(long long) const { return {}; }
bool Maquina::recebe(Item, long long) { return false; }
void Maquina::removeSaida() {}

Item Maquina::itemParaEntregar(long long instante) const {
    return obtemSaida(instante);
}

bool Maquina::tentaEntregar(Maquina& destino, long long instante) {
    const Item item = obtemSaida(instante);
    if (this == &destino || !item.existe() || !destino.recebe(item, instante)) return false;
    removeSaida();
    return true;
}
