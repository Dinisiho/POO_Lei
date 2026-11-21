#ifndef VISTAFABRICA_H
#define VISTAFABRICA_H

#include <iosfwd>

class Fabrica;
class Ecra;

class VistaFabrica {
public:
    static const int alturaPainel = 13;

    void desenha(const Fabrica& fabrica, Ecra& ecra) const;
    void mostraEstado(const Fabrica& fabrica, std::ostream& saida) const;
    bool consultaPosicoes(const Fabrica& fabrica, int linha, int coluna,
                         int raio, std::ostream& saida) const;
};

#endif
            