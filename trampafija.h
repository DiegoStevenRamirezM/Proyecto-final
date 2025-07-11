#ifndef TRAMPAFIJA_H
#define TRAMPAFIJA_H

#include "trampa.h"
#include <QString>
#include <QPixmap>

class TrampaFija : public Trampa
{
    Q_OBJECT

public:
    TrampaFija(const QString& tipo);
    void actualizar() override;

private:
    QPixmap spriteSheet;
    int frameActual;
    int totalFrames;
    int fila;
    int anchoSprite;
    int altoSprite;
};

#endif // TRAMPAFIJA_H
