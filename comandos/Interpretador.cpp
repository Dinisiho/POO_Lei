#include "Interpretador.h"

#include "Comando.h"
#include "ComandosFabrica.h"
#include "ComandosGerais.h"
#include "ComandosPendentes.h"
#include "../ecra/Ecra.h"
#include "../fabrica/Fabrica.h"
#include "../settings/Settings.h"
#include "../vista/VistaFabrica.h"

#include <fstream>
#include <ostream>
#include <sstream>

Interpretador::Interpretador() : fabrica(nullptr), terminar(false) {
    // O interpretador e proprietario das instancias dos comandos.
    comandos.reserve(17);
    try {
        comandos.push_back(new ComandoFabrica());
        comandos.push_back(new ComandoCarrega());
        comandos.push_back(new ComandoAvanca());
        comandos.push_back(new ComandoCompra());
        comandos.push_back(new ComandoVende());
        comandos.push_back(new ComandoRoda());
        comandos.push_back(new ComandoMelhora());
        comandos.push_back(new ComandoContrata());
        comandos.push_back(new ComandoAtribui());
        comandos.push_back(new ComandoDespede());
        comandos.push_back(new ComandoEstado());
        comandos.push_back(new ComandoLPosicao());
        comandos.push_back(new ComandoGrava());
        comandos.push_back(new ComandoRecupera());
        comandos.push_back(new ComandoExecuta());
        comandos.push_back(new ComandoAjuda());
        comandos.push_back(new ComandoFim());
    } catch (...) {
        for (Comando* comando : comandos) {
            delete comando;
        }
        throw;
    }
}

Interpretador::~Interpretador() {
    for (Comando* comando : comandos) {
        delete comando;
    }
    delete fabrica;
}

const Comando* Interpretador::procuraComando(const std::string& nome) const {
    for (const Comando* comando : comandos) {
        if (comando->nome() == nome) {
            return comando;
        }
    }
    return nullptr;
}

void Interpretador::processaLinha(const std::string& linha, std::ostream& saida) {
    if (terminar) return;

    std::istringstream entrada(linha);
    std::string palavra;
    if (!(entrada >> palavra)) return;

    const Comando* comando = procuraComando(palavra);
    if (comando == nullptr) {
        saida << "Comando desconhecido: " << palavra << ". Usa 'ajuda'.\n";
        return;
    }

    std::string argumentos;
    std::getline(entrada, argumentos);
    if (!comando->valida(argumentos, saida)) return;
    comando->executa(*this, argumentos, saida);
}

void Interpretador::executaFicheiro(const std::string& caminho, std::ostream& saida) {
    for (const std::string& ativo : ficheirosEmExecucao) {
        if (ativo == caminho) {
            saida << "Erro: execucao recursiva do mesmo ficheiro: " << caminho << "\n";
            return;
        }
    }

    std::ifstream ficheiro(caminho);
    if (!ficheiro) {
        saida << "Erro: nao foi possivel abrir o ficheiro '" << caminho << "'.\n";
        return;
    }

    ficheirosEmExecucao.push_back(caminho);
    std::string linha;
    int numeroLinha = 0;
    while (!terminar && std::getline(ficheiro, linha)) {
        ++numeroLinha;
        saida << "[" << caminho << ":" << numeroLinha << "] " << linha << '\n';
        processaLinha(linha, saida);
    }
    if (ficheiro.bad()) {
        saida << "Erro: falha de leitura no ficheiro '" << caminho << "'.\n";
    }
    ficheirosEmExecucao.pop_back();
}

void Interpretador::mostraAjuda(const std::string& nomeComando, std::ostream& saida) const {
    if (nomeComando.empty()) {
        saida << "Comandos disponiveis:\n";
        for (const Comando* comando : comandos) {
            std::ostringstream texto;
            comando->ajuda(texto);
            std::istringstream linhas(texto.str());
            std::string sintaxe;
            std::getline(linhas, sintaxe);
            saida << "  " << sintaxe << '\n';
        }
        return;
    }

    const Comando* comando = procuraComando(nomeComando);
    if (comando == nullptr) {
        saida << "Erro: nao existe o comando '" << nomeComando << "'.\n";
        return;
    }
    comando->ajuda(saida);
}

bool Interpretador::temFabrica() const {
    return fabrica != nullptr;
}

void Interpretador::criaFabrica(int linhas, int colunas, std::ostream& saida) {
    if (fabrica != nullptr) {
        saida << "Erro: ja existe uma fabrica.\n";
        return;
    }
    try {
        fabrica = new Fabrica(linhas, colunas);
        saida << "Fabrica criada: " << linhas << " x " << colunas << ".\n";
        mostraFabrica();
    } catch (const std::string& erro) {
        saida << "Erro ao criar fabrica: " << erro << '\n';
    }
}

void Interpretador::carregaFabrica(const std::string& caminho, std::ostream& saida) {
    if (fabrica != nullptr) {
        saida << "Erro: ja existe uma fabrica.\n";
        return;
    }
    try {
        fabrica = new Fabrica(caminho);
        saida << "Fabrica carregada de '" << caminho << "'.\n";
        mostraFabrica();
    } catch (const std::string& erro) {
        saida << "Erro ao carregar fabrica: " << erro << '\n';
    }
}

void Interpretador::consultaPosicoes(int linha, int coluna, int raio,
                                   std::ostream& saida) const {
    if (fabrica == nullptr) {
        saida << "Erro: cria primeiro uma fabrica.\n";
        return;
    }
    VistaFabrica vista;
    vista.consultaTerrenos(*fabrica, linha, coluna, raio, saida);
}

void Interpretador::mostraFabrica() const {
    if (fabrica == nullptr) return;
    Ecra ecra(fabrica->obtemLinhas() + 14, Settings::ecra_max_colunas);
    VistaFabrica vista;
    vista.desenha(*fabrica, ecra);
    ecra.mostra();
}

void Interpretador::termina() { terminar = true; }
bool Interpretador::terminou() const { return terminar; }
