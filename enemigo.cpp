#include "enemigo.h"
#include "protagonista.h"
#include "nivel2view.h"
#include "nivel.h"
#include <QTransform>
#include <QTimer>


Enemigo::Enemigo(QGraphicsItem* parent)
    : Personaje(100)
{
    hojaSprite = QPixmap(":/imagenes/enemigo.png");

    if (hojaSprite.isNull()) {
        return;
    }

    anchoFrame = 300;
    altoFrame = 300;
    totalFrames = 2;
    frameActual = 0;

    connect(timer, &QTimer::timeout, this, &Enemigo::siguienteFrame);
    timer->start(300);

    siguienteFrame();  // mostrar el primer frame de inmediato
}

void Enemigo::setObjetivos(Protagonista* goku, Nivel* nivel)
{
    this->objetivoGoku = goku;
    this->nivelPadre = nivel;
}

void Enemigo::siguienteFrame()
{
    if (muerto) return;

    frameActual = (frameActual + 1) % totalFrames;
    int x = frameActual * anchoFrame;
    int y = 0;

    QPixmap cuadro = hojaSprite.copy(x, y, anchoFrame, altoFrame);
    QPixmap volteado = cuadro.transformed(QTransform().scale(-1, 1));
    setPixmap(volteado);
    setScale(1.6);
    setPos(240, 170);
}

void Enemigo::recibirDanio(int cantidad)
{
    Personaje::recibirDanio(cantidad);

    //  Solo Nivel2View tiene la barra del enemigo
    Nivel2View* nivel2 = dynamic_cast<Nivel2View*>(nivelPadre);
    if (nivel2) {
        nivel2->actualizarBarraEnemigo();
    }

    if (muerto) {
        timer->stop();
        scene()->removeItem(this);
        deleteLater();
    }
}

void Enemigo::lanzarMisiles(QGraphicsScene* escena, QGraphicsItem* objetivo)
{
    for (int i = 0; i < 3; ++i) {
        QTimer::singleShot(i * 300, this, [=]() {
            QGraphicsPixmapItem* misil = new QGraphicsPixmapItem();
            QPixmap img(":/imagenes/misil.png");
            QPixmap reflejado = img.transformed(QTransform().scale(-1, 1));  // Voltearlo horizontalmente
            misil->setPixmap(reflejado.scaled(60, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

            misil->setPos(this->x() + 90, this->y() + 160);
            escena->addItem(misil);

            QTimer* animador = new QTimer();

            float t = 0;
            float vx = -5.0;
            float vyInicial = -8.0;
            float gravedad = 0.5;
            float y0 = misil->y();
            float x0 = misil->x();

            QObject::connect(animador, &QTimer::timeout, [=]() mutable {
                t += 1.0;

                float x = x0 + vx * t;
                float y = y0 + vyInicial * t + 0.5 * gravedad * t * t;
                misil->setPos(x, y);

                // Colisión con Goku
                if (misil->collidesWithItem(objetivo)) {
                    animador->stop();
                    escena->removeItem(misil);
                    delete misil;

                    Protagonista* goku = dynamic_cast<Protagonista*>(const_cast<QGraphicsItem*>(objetivo));
                    if (goku && !goku->estaMuerto()) {
                        goku->recibirDanio(10);

                        if (nivelPadre) nivelPadre->actualizarBarraVida();

                        if (goku->getVida() <= 0) {
                            goku->mostrarMuerte();

                            QTimer::singleShot(800, nivelPadre, [=]() {
                                nivelPadre->mostrarDerrota();
                            });
                        }
                    }

                }

                // Eliminar si sale de la escena
                if (x < 0 || y > escena->height()) {
                    animador->stop();
                    escena->removeItem(misil);
                    delete misil;
                }
            });

            animador->start(20);
        });
    }
}

bool Enemigo::estaMuerto() const {
    return muerto;
}
