#include "Comando.h"

#include <ostream>
#include <sstream>

std::vector<std::string> Comando::separa(const std::string& argumentos) {
    std::istringstream entrada(argumentos);
    std::vector<std::string> parametros;
    std::string palavra;
    while (entrada >> palavra) {
        parametros.push_back(palavra);
    }
    return parametros;
}

bool Comando::numeroInteiro(const std::string& texto, int& numero) {
    std::istringstream entrada(texto);
    char restante;
    return (entrada >> numero) && !(entrada >> restante);
}

bool Comando::posicaoValida(const std::string& texto) {
    return texto.size() == 2 &&
           texto[0] >= 'a' && texto[0] <= 'z' &&
           texto[1] >= 'a' && texto[1] <= 'z';
}

bool Comando::direcaoValida(const std::string& texto) {
    return texto == "c" || texto == "b" || texto == "e" || texto == "d";
}

bool Comando::tipoMaquinaValido(const std::string& texto) {
    return texto == "p" || texto == "t" || texto == "r" ||
           texto == "o" || texto == "s" || texto == "l";
}

bool Comando::quantidadeValida(const std::vector<std::string>& parametros,
                              std::size_t minimo, std::size_t maximo, std::ostream& erros) {
    const std::size_t quantidade = parametros.size();
    if (quantidade < minimo || quantidade > maximo) {
        erros << "Erro: numero incorreto de parametros. Consulta 'ajuda <comando>'.\n";
        return false;
    }
    return true;
}
