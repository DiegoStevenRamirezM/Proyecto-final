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
    estaOcupado = true;  // Esto es para bloquear cualquier otra animacion o accion
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

            estaOcupado = false;     // Ahora si puede volver a moverse o atacar
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

     reproducirSonido("Salto.wav"); // 🎵 Sonido justo al iniciar salto

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

void Protagonista::lanzarKamehameha() {
    QGraphicsPixmapItem* rayo = new QGraphicsPixmapItem();

    int direccionRayo = direccionSalto;
    int yOffset = 20;
    int xOffset = (direccionRayo == -1) ? -100 : 50;
    rayo->setPos(x() + xOffset, y() + yOffset);
    rayo->setZValue(2);

    QPixmap frame = spriteKamehameha.copy(0, 0, 200, 42);
    if (direccionRayo == -1)
        frame = frame.transformed(QTransform().scale(-1, 1));
    rayo->setPixmap(frame);

    scene()->addItem(rayo);

    QTimer* timerRayo = new QTimer(this);
    connect(timerRayo, &QTimer::timeout, this, [=]() {
        int dx = direccionRayo * 15;  // ya no cambia aunque Goku se mueva
        rayo->moveBy(dx, 0);

        QList<QGraphicsItem*> colisiones = rayo->collidingItems();
        for (auto* item : colisiones) {
            if (dynamic_cast<Pared*>(item)) {
                static_cast<Pared*>(item)->recibirImpacto();
                scene()->removeItem(rayo);
                delete rayo;
                timerRayo->stop();
                timerRayo->deleteLater();
                return;
            }
        }

        if (rayo->x() < 0 || rayo->x() > 1600) {
            scene()->removeItem(rayo);
            delete rayo;
            timerRayo->stop();
            timerRayo->deleteLater();
        }
    });

    timerRayo->start(80);
}

void Protagonista::animarKamehameha() {
    timer->stop(); // Detener animación normal

    int fila = 10;
    int totalFrames = 12;
    int duracionFrame = 70;  // más rápido para que el rayo no se retrase
    int ancho = 40;
    int alto = 40;

    QTimer* animTimer = new QTimer(this);
    int* frameIndex = new int(0);

    connect(animTimer, &QTimer::timeout, this, [=]() {
        if (*frameIndex >= totalFrames) {
            animTimer->stop();
            delete frameIndex;
            animTimer->deleteLater();
            timer->start(120);
            lanzarKamehameha();  // sale justo al terminar
            return;
        }
        int x = (*frameIndex) * ancho;
        QPixmap frame = hojaSprite.copy(x, fila * alto, ancho, alto);
        if (direccionSalto == -1)
            frame = frame.transformed(QTransform().scale(-1, 1));

        setPixmap(frame.scaled(96, 96, Qt::KeepAspectRatio));
        (*frameIndex)++;
    });

    animTimer->start(duracionFrame);
}

void Protagonista::setEnemigo(Enemigo* enemigo)
{
    enemigoActual = enemigo;
}

void Protagonista::curarse()
{
    if (estaOcupado) return;
    estaOcupado = true;

    int frames = 4;
    int fila = 5;
    int duracion = 100;

    timer->stop();
    frameActual = 0;

    QPointF posicionInicial = this->pos();
    QPixmap spriteOriginal = hojaSprite.copy(0, 0 * altoFrame, anchoFrame, altoFrame);

    QTimer* animador = new QTimer();
    connect(animador, &QTimer::timeout, [=]() mutable {
        if (frameActual < frames) {
            int x = frameActual * anchoFrame;
            int y = fila * altoFrame;

            int factor = 3;
            QPixmap cuadro = hojaSprite.copy(x, y, anchoFrame, altoFrame);
            QPixmap escalado = cuadro.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(escalado);
            setPos(posicionInicial.x() + 20, posicionInicial.y() - 25);

            frameActual++;
        } else {
            animador->stop();
            delete animador;

            int factor = 3;
            QPixmap restaurado = spriteOriginal.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(restaurado);
            setPos(posicionInicial);

            // ✅ Solo se cura una vez
            vida += 8;
            if (vida > 100) vida = 100;

            estaOcupado = false;
        }
    });

    animador->start(duracion);
}

// PUÑO - fila 7, 7 frames
void Protagonista::animarPunio()
{
    if (estaOcupado) return;
    estaOcupado = true;

    int frames = 6;
    int fila = 7;
    int duracion = 100;

    timer->stop();
    frameActual = 0;
    bool golpeado = false;  // solo una colisión

    QPointF posicionInicial = this->pos();
    QPixmap spriteOriginal = hojaSprite.copy(0, 0 * altoFrame, anchoFrame, altoFrame);

    QTimer* animador = new QTimer();
    connect(animador, &QTimer::timeout, [=]() mutable {
        if (frameActual < frames) {
            int x = frameActual * anchoFrame;
            int y = fila * altoFrame;

            int factor = 3;
            QPixmap cuadro = hojaSprite.copy(x, y, anchoFrame, altoFrame);
            QPixmap escalado = cuadro.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(escalado);
            setPos(posicionInicial.x() + 20, posicionInicial.y() - 25);

            frameActual++;

            if (!golpeado && enemigoActual && this->collidesWithItem(enemigoActual)) {
                enemigoActual->recibirDanio(2);
                reproducirSonido("Golpe.wav");
                golpeado = true;
            }
        } else {
            animador->stop();
            delete animador;

            int factor = 3;
            QPixmap restaurado = spriteOriginal.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(restaurado);
            setPos(posicionInicial);

            estaOcupado = false;
        }
    });

    animador->start(duracion);
}

// PATADA - fila 17, 12 frames
void Protagonista::animarPatada()
{
    if (estaOcupado) return;
    estaOcupado = true;

    int frames = 12;
    int fila = 17;
    int duracion = 45;

    timer->stop();
    frameActual = 0;
    bool golpeado = false;  // solo una colisión

    QPointF posicionInicial = this->pos();
    QPixmap spriteOriginal = hojaSprite.copy(0, 0 * altoFrame, anchoFrame, altoFrame);

    QTimer* animador = new QTimer();
    connect(animador, &QTimer::timeout, [=]() mutable {
        if (frameActual < frames) {
            int x = frameActual * anchoFrame;
            int y = fila * altoFrame;

            int factor = 3;
            QPixmap cuadro = hojaSprite.copy(x, y, anchoFrame, altoFrame);
            QPixmap escalado = cuadro.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(escalado);
            setPos(posicionInicial.x() + 20, posicionInicial.y() - 25);

            frameActual++;

            if (!golpeado && enemigoActual && this->collidesWithItem(enemigoActual)) {
                enemigoActual->recibirDanio(3);
                reproducirSonido("Golpe.wav");
                golpeado = true;
            }

        } else {
            animador->stop();
            delete animador;

            int factor = 3;
            QPixmap restaurado = spriteOriginal.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(restaurado);
            setPos(posicionInicial);

            estaOcupado = false;
        }
    });

    animador->start(duracion);
}

// BOLA DE ENERGÍA - fila 2, 3 frames + ataque
void Protagonista::animarBolaEnergia()
{
    if (estaOcupado) return;
    estaOcupado = true;

    int frames = 3;
    int fila = 1;
    int duracion = 100;

    timer->stop();
    frameActual = 0;

    QPointF posicionInicial = this->pos();
    QPixmap spriteOriginal = hojaSprite.copy(0, 0 * altoFrame, anchoFrame, altoFrame);

    QTimer* animador = new QTimer();
    connect(animador, &QTimer::timeout, [=]() mutable {
        if (frameActual < frames) {
            int x = frameActual * anchoFrame;
            int y = fila * altoFrame;

            int factor = 3;
            QPixmap cuadro = hojaSprite.copy(x, y, anchoFrame, altoFrame);
            QPixmap escalado = cuadro.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(escalado);
            setPos(posicionInicial.x() + 20, posicionInicial.y() - 25);

            frameActual++;
        } else {
            animador->stop();
            delete animador;

            // Crear bola de energía
            QGraphicsEllipseItem* bola = new QGraphicsEllipseItem(0, 0, 30, 30);
            bola->setBrush(Qt::blue);
            bola->setPen(Qt::NoPen);
            bola->setPos(this->x() + 100, this->y() + 30);
            scene()->addItem(bola);

            QTimer* mover = new QTimer();
            connect(mover, &QTimer::timeout, [=]() mutable {
                bola->moveBy(7, 0);

                if (enemigoActual && bola->collidesWithItem(enemigoActual)) {
                    enemigoActual->recibirDanio(4);
                    scene()->removeItem(bola);
                    delete bola;
                    mover->stop();
                    delete mover;
                }

                if (bola->x() > scene()->width()) {
                    scene()->removeItem(bola);
                    delete bola;
                    mover->stop();
                    delete mover;
                }
            });
            mover->start(20);

            // Restaurar sprite
            int factor = 3;
            QPixmap restaurado = spriteOriginal.scaled(anchoFrame * factor, altoFrame * factor);
            setPixmap(restaurado);
            setPos(posicionInicial);
            estaOcupado = false;
        }
    });

    animador->start(duracion);
}
