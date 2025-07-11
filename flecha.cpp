#include "flecha.h"
#include <QPixmap>
#include <QtMath>
#include <QTransform>

Flecha::Flecha(bool haciaDerecha, QGraphicsItem* parent) : Trampa(1)
{
    direccion = haciaDerecha;
    velocidad = 5;
    amplitud = 20.0;
    frecuencia = 0.1;
    tiempo = 0;
    yBase = 460;

    // Cargar imagen
    QPixmap img(":/imagenes/Flecha_trampa.png");

    // Voltear horizontal si va hacia la derecha
    if (direccion) {
        img = img.transformed(QTransform().scale(-1, 1));
    }

    // Escalar más grande para que se vea bien
    setPixmap(img.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Posición inicial según dirección
    setPos(direccion ? 0 : 800, yBase);

    // Iniciar animación
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Flecha::actualizar);
    timer->start(30);
}

void Flecha::actualizar()
{
    tiempo++;

    double x = pos().x() + (direccion ? velocidad : -velocidad);
    double y = yBase + amplitud * qSin(frecuencia * tiempo);

    setPos(x, y);

    if (x < -100 || x > 900) {
        this->scene()->removeItem(this);
        delete this;
    }
}
