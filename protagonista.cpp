#include "protagonista.h"
#include "pared.h"
#include "enemigo.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QTransform>

Protagonista::Protagonista() : Personaje(100)
{
    hojaSprite.load(":/imagenes/Sprites_Goku.png");
    anchoFrame = 40;
    altoFrame = 40;
    yFrameFijo = 3 * altoFrame;
    totalFrames = 8;
    frameActual = 0;
    setPixmap(hojaSprite.copy(0, yFrameFijo, anchoFrame, altoFrame).scaled(100, 100));
    setPos(300, 300);

    spriteKamehameha.load(":/imagenes/kamehameha1.png");


    connect(timer, &QTimer::timeout, this, &Protagonista::siguienteFrame);
    timer->start(120);

    timerSalto = new QTimer(this);
    connect(timerSalto, &QTimer::timeout, this, &Protagonista::animarSalto);

    g = 9.8;
    v0 = 50;
    velocidadHorizontal = 5;
    direccionSalto = 1;
    tiempo = 0;
    yInicial = 0;
    enElAire = false;
    estado = Quieto;
    limiteDerechoActivo = false; //atributo para controlar la segunda hbitacion

}

void Protagonista::siguienteFrame() {
    if (muerto) return; // No animar si está muerto
    int fila = 3;
    totalFrames = 8;
    if (enElAire) { fila = 4; totalFrames = 4; }
    else if (estado == Quieto) { fila = 0; totalFrames = 4; }

    yFrameFijo = fila * altoFrame;
    int x = frameActual * anchoFrame;
    QPixmap frame = hojaSprite.copy(x, yFrameFijo, anchoFrame, altoFrame);
    if (direccionSalto == -1) frame = frame.transformed(QTransform().scale(-1, 1));
    setPixmap(frame.scaled(100, 100));
    frameActual = (frameActual + 1) % totalFrames;
}

void Protagonista::mostrarAnimacionDanio()
{
    estaOcupado = true;  // Esto es para bloquear cualquier otra animación o acción
    timer->stop();       // Detener animación normal

    int fila = 13;
    int totalFramesDanio = 4;
    int duracionFrame = 100;
    int ancho = anchoFrame;
    int alto = altoFrame;

    QTimer* animTimer = new QTimer(this);
    int* frameIndex = new int(0); // usamos puntero para modificar dentro del lambda

    connect(animTimer, &QTimer::timeout, this, [=]() {
        if (*frameIndex >= totalFramesDanio) {
            animTimer->stop();
            delete frameIndex;
            animTimer->deleteLater();

            estaOcupado = false;     // Ahora sí puede volver a moverse o atacar
            timer->start(120);       // volver a animación normal
            return;
        }

        int x = (*frameIndex) * ancho;
        QPixmap frame = hojaSprite.copy(x, fila * alto, ancho, alto);
        if (direccionSalto == -1)
            frame = frame.transformed(QTransform().scale(-1, 1));
        setPixmap(frame.scaled(100, 100));
        (*frameIndex)++;
    });

    animTimer->start(duracionFrame); // cambia cada 100 ms
}

void Protagonista::moverDerecha() {
    int limite;

    if (limiteDerechoActivo && x() >= 800) {
        // Ya estamos en la segunda habitación
        limite = 1500;
    } else if (limiteDerechoActivo) {
        // Todavía estamos saliendo de la primera
        limite = 1600;
    } else {
        limite = 700;
    }

    if (x() + 5 <= limite) {
        setPos(x() + 5, y());
        direccionSalto = 1;
    }

    estado = Corriendo;
}

void Protagonista::moverIzquierda() {
    int limiteIzquierdo;

    if (limiteDerechoActivo && x() >= 800) {
        // Ya estamos en la segunda habitación
        limiteIzquierdo = 801;
    } else {
        limiteIzquierdo = 0;
    }

    if (x() - 5 >= limiteIzquierdo) {
        setPos(x() - 5, y());
        direccionSalto = -1;
    }

    estado = Corriendo;
}

void Protagonista::saltar() {
    if (enElAire) return;
    estado = Saltando;
    frameActual = 0;
    enElAire = true;
    tiempo = 0;
    yInicial = y();
    timerSalto->start(30);
}

void Protagonista::detenerse() {
    if (!enElAire) { estado = Quieto; frameActual = 0; }
}

void Protagonista::animarSalto() {
    tiempo += 0.25;
    double dy = -0.5 * g * tiempo * tiempo + v0 * tiempo;
    setY(yInicial - dy);

    double nuevaX = x() + velocidadHorizontal * direccionSalto;

    int limite = 1600;
    int limiteIzquierdo = 0;

    // Si Goku ya está en la segunda habitacion, se ajustan los límites
    if (limiteDerechoActivo && x() >= 800) {
        limite = 1500;
        limiteIzquierdo = 801;
    } else if (!limiteDerechoActivo) {
        limite = 700;
    }

    // Para limitar movimiento horizontal en el aire
    if (nuevaX >= limiteIzquierdo && nuevaX <= limite) {
        setX(nuevaX);
    }

    if (dy <= 0 && y() >= yInicial) {
        setY(yInicial);
        enElAire = false;
        timerSalto->stop();
        estado = Quieto;
        frameActual = 0;
    }
}

void Protagonista::mostrarMuerte()
{
    muerto = true;
    estaOcupado = true;


    int fila = 14;
    int totalFrames = 6;
    int duracion = 80;
    int frameActual = 0;

    QTimer* animador = new QTimer(this);
    connect(animador, &QTimer::timeout, this, [=]() mutable {
        if (frameActual >= totalFrames) {
            animador->stop();
            animador->deleteLater();
            estaOcupado = false;
            return;
        }

        QPixmap frame = hojaSprite.copy(frameActual * anchoFrame, fila * altoFrame, anchoFrame, altoFrame);
        if (direccionSalto == -1)
            frame = frame.transformed(QTransform().scale(-1, 1));

        setPixmap(frame.scaled(pixmap().width(), pixmap().height(), Qt::KeepAspectRatio));
        frameActual++;
    });
    animador->start(duracion);
}

void Protagonista::permitirAvanceCompleto(bool valor) {
    limiteDerechoActivo = valor;
}
