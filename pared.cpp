#include "pared.h"
#include <QPixmap>
#include <QtMath>
#include <QGraphicsScene>

Pared::Pared(QGraphicsItem* parent) : Trampa(90)  // daño un poco mayor que otras trampas
{
    QPixmap img(":/imagenes/pared.png");
    QPixmap escalada = img.scaled(500, 520, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //evita las colisiones anticipadas
    setPixmap(escalada);
    setOffset(50, 0);

    posicionXInicial = 1600;  // ahora comienza al borde derecho de la 2da habitación
    setPos(posicionXInicial, 50);

    tiempo = 0.0;
    aceleracion = -0.5;         // avanza hacia la izquierda (aceleración negativa)
    velocidadInicial = -2.0;    // velocidad inicial hacia la izquierda

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Pared::actualizar);
    timer->start(30);
}

void Pared::actualizar()
{
    tiempo += 0.1;

    double x = posicionXInicial + velocidadInicial * tiempo + 0.5 * aceleracion * tiempo * tiempo;
    setPos(x, y());

    // Si llega demasiado a la izquierda (fuera de escena), eliminarla
    if (x < 800) {
        if (scene()) {
            scene()->removeItem(this);
        }
        delete this;
    }
}

void Pared::recibirImpacto()
{
    // Retroceder disminuyendo el tiempo de avance
    tiempo -= 2;
    if (tiempo < 0.0)
        tiempo = 0.0;
}

