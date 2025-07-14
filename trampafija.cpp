#include "trampafija.h"
#include <QTimer>


TrampaFija::TrampaFija(const QString& tipo) : Trampa(6)
{
    spriteSheet.load(":/imagenes/Trampas_fijas.png");

    anchoSprite = 40;
    altoSprite = 40;
    frameActual = 0;

    if (tipo == "estacas1") {
        fila = 0;
        totalFrames = 9;
    } else if (tipo == "estacas2") {
        fila = 1;
        totalFrames = 9;
    } else if (tipo == "pisoTrampa") {
        fila = 2;
        totalFrames = 6;
    } else if (tipo == "acido") {
        fila = 3;
        totalFrames = 8;
    } else {
        fila = 0;
        totalFrames = 1;
    }

    setPixmap(spriteSheet.copy(0, fila * altoSprite, anchoSprite, altoSprite).scaled(ancho, alto));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrampaFija::actualizar);
    if (totalFrames > 1) timer->start(100);
}

void TrampaFija::actualizar()
{
    int x = frameActual * anchoSprite;
    QPixmap frame = spriteSheet.copy(x, fila * altoSprite, anchoSprite, altoSprite);
    setPixmap(frame.scaled(ancho, alto));
    frameActual = (frameActual + 1) % totalFrames;
}
