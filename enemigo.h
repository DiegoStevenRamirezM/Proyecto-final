#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
class Protagonista;
class Nivel;
#include <QGraphicsScene>

class Enemigo : public Personaje
{
    Q_OBJECT

public:
    explicit Enemigo(QGraphicsItem* parent = nullptr);
    void setObjetivos(Protagonista* goku, Nivel* nivel);
    void siguienteFrame() override;
    void recibirDanio(int cantidad) override;
    void lanzarMisiles(QGraphicsScene* escena, QGraphicsItem* objetivo);
    bool estaMuerto() const ;

private:
    Protagonista* objetivoGoku;
    Nivel* nivelPadre;
};

#endif // ENEMIGO_H
