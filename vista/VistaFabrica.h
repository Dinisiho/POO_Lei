#ifndef VISTAFABRICA_H
#define VISTAFABRICA_H

#include <iosfwd>

class Fabrica;
class Ecra;

class VistaFabrica {
public:
    void desenha(const Fabrica& fabrica, Ecra& ecra) const;
    bool consultaTerrenos(const Fabrica& fabrica, int linha, int coluna,
                         int raio, std::ostream& saida) const;
};

#endif
