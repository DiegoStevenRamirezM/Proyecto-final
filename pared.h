#ifndef PARED_H
#define PARED_H

#include "trampa.h"

class Pared : public Trampa
{
    Q_OBJECT

public:
    Pared(QGraphicsItem* parent = nullptr);
    void actualizar() override;
    void recibirImpacto(); // nuevo método para retroceso

private:
    double tiempo;
    double aceleracion;
    double velocidadInicial;
    double posicionXInicial;
};

#endif // PARED_H
