#include "Interpretador.h"

#include "Comando.h"
#include "ComandosFabrica.h"
#include "ComandosGerais.h"
#include "ComandosPendentes.h"
#include "ComandosSimulacao.h"
#include "../ecra/Ecra.h"
#include "../fabrica/Fabrica.h"
#include "../settings/Settings.h"
#include "../vista/VistaFabrica.h"

#include <ostream>
#include <sstream>
#include <utility>

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
    if (comando->exigeFabrica() && fabrica == nullptr) {
        saida << "Erro: cria primeiro uma fabrica (fabrica ou carrega).\n";
        return;
    }
    comando->executa(*this, argumentos, saida);
}

void Interpretador::executaFicheiro(const std::string& caminho, std::ostream& saida) {
    for (const FicheiroComandos& ativo : ficheirosEmExecucao) {
        if (ativo.caminho == caminho) {
            saida << "Erro: execucao recursiva do mesmo ficheiro: " << caminho << "\n";
            return;
        }
    }

    FicheiroComandos novo{caminho, std::ifstream(caminho)};
    if (!novo.entrada) {
        saida << "Erro: nao foi possivel abrir o ficheiro '" << caminho << "'.\n";
        return;
    }

    const bool iniciaLeitura = ficheirosEmExecucao.empty();
    ficheirosEmExecucao.push_back(std::move(novo));
    if (!iniciaLeitura) return;

    std::string linha;
    while (!terminar && !ficheirosEmExecucao.empty()) {
        FicheiroComandos& ativo = ficheirosEmExecucao.back();
        if (std::getline(ativo.entrada, linha)) {
            ++ativo.numeroLinha;
            saida << "[" << ativo.caminho << ":" << ativo.numeroLinha << "] " << linha << '\n';
            // Um executa aninhado pode realocar o vector. Não se volta a usar ativo.
            processaLinha(linha, saida);
        } else {
            if (ativo.entrada.bad()) {
                saida << "Erro: falha de leitura no ficheiro '" << ativo.caminho << "'.\n";
            }
            ficheirosEmExecucao.pop_back();
        }
    }
    ficheirosEmExecucao.clear();
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
            saida << "  " << sintaxe ;
            if (comando->estaPendente()) {
               saida << " - (Pendente)";
            }
          saida << '\n';
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

Fabrica& Interpretador::obtemFabrica() { return *fabrica; }

void Interpretador::criaFabrica(int linhas, int colunas, std::ostream& saida) {
    if (fabrica != nullptr) {
        saida << "Erro: ja existe uma fabrica.\n";
        return;
    }
    try {
        fabrica = new Fabrica(linhas, colunas);
        saida << "Fabrica criada: " << linhas << " x " << colunas << ".\n";
        mostraFabrica(saida);
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
        mostraFabrica(saida);
    } catch (const std::string& erro) {
        saida << "Erro ao carregar fabrica: " << erro << '\n';
    }
}

void Interpretador::consultaPosicoes(int linha, int coluna, int raio,
                                   std::ostream& saida) const {
    VistaFabrica vista;
    vista.consultaPosicoes(*fabrica, linha, coluna, raio, saida);
}

void Interpretador::mostraFabrica(std::ostream& saida) const {
    if (fabrica == nullptr) return;
    const int alturaMapa = fabrica->obtemLinhas() + 3;
    const int altura = alturaMapa > VistaFabrica::alturaPainel ? alturaMapa : VistaFabrica::alturaPainel;
    Ecra ecra(altura, Settings::ecra_max_colunas);
    VistaFabrica vista;
    vista.desenha(*fabrica, ecra);
    ecra.mostra(saida);
}

void Interpretador::termina() { terminar = true; }
bool Interpretador::terminou() const { return terminar; }
