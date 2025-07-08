#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Personaje(int vidaInicial = 100);
    virtual ~Personaje();

    virtual void recibirDanio(int cantidad);
    int getVida() const;
    bool estaMuerto() const { return muerto; }

protected:
    QPixmap hojaSprite;
    QTimer* timer;

    int vida;
    int frameActual;
    int anchoFrame;
    int altoFrame;
    int totalFrames;
    bool muerto;



    virtual void siguienteFrame() = 0;
    virtual void mostrarAnimacionDanio();
};

#endif // PERSONAJE_H
