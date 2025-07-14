#include "nivel2view.h"
#include "enemigo.h"
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>


Nivel2View::Nivel2View(QWidget *parent) : Nivel(parent)
{
    // Fondo
    inicializarEscena(":/imagenes/Fondo_nivel2.png");

    // Ajustar sprite de Goku
    goku->setScale(2);
    goku->setY(goku->y() - 110);

    // Crear a Pilaf
    pilaf = new Enemigo();
    pilaf->setPos(0, 450);
    pilaf->setZValue(1);
    scene->addItem(pilaf);
    pilaf->setObjetivos(goku, this);

    goku->setEnemigo(pilaf);

    // Barra de vida de Pilaf
    barraEnemigo = new QProgressBar(this);
    barraEnemigo->setRange(0, 100);
    barraEnemigo->setValue(100);
    barraEnemigo->setFormat("Pilaf: %p%");
    barraEnemigo->setGeometry(580, 10, 200, 25);
    barraEnemigo->setStyleSheet(
        "QProgressBar { border: 2px solid black; border-radius: 5px; background-color: white; }"
        "QProgressBar::chunk { background-color: blue; }"
        );
    barraEnemigo->raise();

    // Temporizador de ataque de Pilaf
    temporizadorAtaque = new QTimer(this);
    connect(temporizadorAtaque, &QTimer::timeout, this, [=]() {
        if (goku->getVida() <= 0) {
            temporizadorAtaque->stop();
            goku->mostrarMuerte();

            QTimer::singleShot(800, this, [=]() {
                mostrarDerrota();
            });
            return;
        }

        if (!pilaf->estaMuerto()) {
            pilaf->lanzarMisiles(scene, goku);
        } else {
            temporizadorAtaque->stop();
            mostrarVictoria();
        }
    });

    temporizadorAtaque->start(5000);
}

void Nivel2View::keyPressEvent(QKeyEvent *event)
{
    if (goku->estaMuerto() || goku->getEstaOcupado()) return;

    switch (event->key()) {
    case Qt::Key_C:
        goku->curarse();
        actualizarBarraVida();
        break;
    case Qt::Key_F:
        goku->animarPunio();
        break;
    case Qt::Key_G:
        goku->animarPatada();
        break;
    case Qt::Key_H:
        goku->animarBolaEnergia();
        break;
    default:
        break;
    }
}

void Nivel2View::actualizarBarraEnemigo()
{
    if (!pilaf) return;
    barraEnemigo->setValue(pilaf->getVida());
}

void Nivel2View::detenerNivel()
{
    if (temporizadorAtaque) {
        temporizadorAtaque->stop();
    }

    if (barraEnemigo) {
        barraEnemigo->hide();
    }

}
