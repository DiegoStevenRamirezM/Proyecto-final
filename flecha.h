#ifndef FLECHA_H
#define FLECHA_H

#include "trampa.h"
#include <QGraphicsScene>

class Flecha : public Trampa
{
    Q_OBJECT

public:
    Flecha(bool haciaDerecha, QGraphicsItem* parent = nullptr);
    void actualizar() override;

private:
    bool direccion;  // true = derecha, false = izquierda
    int velocidad;
    double amplitud;
    double frecuencia;
    int tiempo;
    int yBase;
};

#endif // FLECHA_H
