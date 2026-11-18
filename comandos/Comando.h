#ifndef COMANDO_H
#define COMANDO_H

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

class Interpretador;

class Comando {
public:
    virtual ~Comando() = default;

    virtual const std::string& nome() const = 0;
    virtual bool valida(const std::string& argumentos, std::ostream& erros) const = 0;
    virtual void executa(Interpretador& interpretador, const std::string& argumentos,
                        std::ostream& saida) const = 0;
    virtual void ajuda(std::ostream& saida) const = 0;

protected:
    static std::vector<std::string> separa(const std::string& argumentos);
    static bool numeroInteiro(const std::string& texto, int& numero);
    static bool posicaoValida(const std::string& texto);
    static bool direcaoValida(const std::string& texto);
    static bool tipoMaquinaValido(const std::string& texto);
    static bool quantidadeValida(const std::vector<std::string>& parametros,
                                std::size_t minimo, std::size_t maximo, std::ostream& erros);
};

#endif
