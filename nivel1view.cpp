#include "nivel1view.h"
#include <QRandomGenerator>
#include <QMessageBox>

Nivel1View::Nivel1View(QWidget *parent) : Nivel(parent)
{
    inicializarEscena(":/imagenes/Fondo_nivel1.png");

    goku->permitirAvanceCompleto(false);
    trampasGeneradas = 0;
    habitacionDesbloqueada = false;
    paredCreada = false;

    barraProgreso = new QProgressBar(this);
    barraProgreso->setRange(0, 100);
    barraProgreso->setValue(0);
    barraProgreso->setGeometry(this->width() - 210, 10, 200, 20);
    barraProgreso->setTextVisible(true);
    barraProgreso->setStyleSheet(
        "QProgressBar { border: 2px solid black; border-radius: 5px; background-color: white; }"
        "QProgressBar::chunk { background-color: green; }"
        );

    verificadorSalida = new QTimer(this);
    connect(verificadorSalida, &QTimer::timeout, this, [=]() {
        if (habitacionDesbloqueada && goku->x() >= 800 && !paredCreada) {
            centerOn(1200, 300);
            paredCreada = true;
            iniciarDesafioPared();
        }
    });
    verificadorSalida->start(50);

    temporizadorTrampas = new QTimer(this);
    connect(temporizadorTrampas, &QTimer::timeout, this, &Nivel1View::generarTrampa);
    temporizadorTrampas->start(2500);

    verificadorColisiones = new QTimer(this);
    connect(verificadorColisiones, &QTimer::timeout, this, &Nivel1View::verificarColisiones);
    verificadorColisiones->start(50);

    setFocusPolicy(Qt::StrongFocus);
    setFocus(Qt::OtherFocusReason);
}

void Nivel1View::keyPressEvent(QKeyEvent *event)
{
    if (goku->estaMuerto()) return;

    switch (event->key()) {
    case Qt::Key_D:
        goku->moverDerecha();
        break;
    case Qt::Key_A:
        goku->moverIzquierda();
        break;
    case Qt::Key_W:
        goku->saltar();
        break;
    case Qt::Key_Space:
        if (goku->x() >= 800) {
            goku->animarKamehameha();
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void Nivel1View::keyReleaseEvent(QKeyEvent *event)
{
    if (goku->estaMuerto()) return;

    if (event->key() == Qt::Key_D && !event->isAutoRepeat()) {
        goku->detenerse();
    }
    if (event->key() == Qt::Key_A && !event->isAutoRepeat()) {
        goku->detenerse();
    }

    QGraphicsView::keyReleaseEvent(event);
}

void Nivel1View::generarTrampa()
{
    QString tipos[] = { "estacas1", "estacas2", "pisoTrampa", "acido", "flecha" };
    int indice = QRandomGenerator::global()->bounded(0, 5);
    QString tipoAleatorio = tipos[indice];

    if (tipoAleatorio == "flecha") {
        bool haciaDerecha = QRandomGenerator::global()->bounded(0, 2);
        Flecha* f = new Flecha(haciaDerecha);
        int x = haciaDerecha ? 0 : 800;
        f->setPos(x, 460);
        f->setZValue(1);
        scene->addItem(f);
    } else {
        TrampaFija* trampa = new TrampaFija(tipoAleatorio);
        int x = QRandomGenerator::global()->bounded(100, 700);
        int y = (tipoAleatorio == "acido") ? 460 : 480;
        trampa->setPos(x, y);
        trampa->setZValue(1);
        scene->addItem(trampa);
        trampasActivas.append(trampa);

        QTimer::singleShot(6000, [=]() {
            trampasActivas.removeOne(trampa);
            trampasEnContacto.remove(trampa);
            trampa->detenerDanioContinuo();
            scene->removeItem(trampa);
            delete trampa;
        });

        trampa->actualizar();
    }

    trampasGeneradas++;
    int totalTrampasParaDesbloqueo = 30;
    barraProgreso->setValue((trampasGeneradas * 100) / totalTrampasParaDesbloqueo);

    if (trampasGeneradas == totalTrampasParaDesbloqueo && !habitacionDesbloqueada) {
        habitacionDesbloqueada = true;
        goku->permitirAvanceCompleto(true);

        barraProgreso->hide();
        temporizadorTrampas->stop();

        mensajeDesbloqueo = new QGraphicsTextItem("Habitación desbloqueada ...");
        mensajeDesbloqueo->setDefaultTextColor(Qt::white);
        mensajeDesbloqueo->setFont(QFont("Impact", 18));
        mensajeDesbloqueo->setZValue(100);
        mensajeDesbloqueo->setPos(380, 5);
        scene->addItem(mensajeDesbloqueo);
    }
}

void Nivel1View::iniciarDesafioPared()
{
    pared = new Pared();
    scene->addItem(pared);

    segundosRestantes = 60;
    contadorTiempo = new QGraphicsTextItem(QString::number(segundosRestantes));
    contadorTiempo->setDefaultTextColor(Qt::white);
    contadorTiempo->setFont(QFont("Arial", 24, QFont::Bold));
    contadorTiempo->setZValue(100);
    contadorTiempo->setPos(1200, 15);
    scene->addItem(contadorTiempo);

    temporizadorResistencia = new QTimer(this);
    connect(temporizadorResistencia, &QTimer::timeout, this, [=]() mutable {
        segundosRestantes--;
        contadorTiempo->setPlainText(QString::number(segundosRestantes));

        if (segundosRestantes <= 0 && pared) {
            scene->removeItem(pared);
            delete pared;
            pared = nullptr;
            temporizadorResistencia->stop();

            scene->removeItem(contadorTiempo);
            delete contadorTiempo;

            mostrarVictoria();
        }
    });

    temporizadorResistencia->start(700);
}

void Nivel1View::verificarColisiones()
{
    QList<QGraphicsItem*> colisiones = goku->collidingItems();
    QSet<Trampa*> trampasActuales;

    for (QGraphicsItem* item : colisiones) {
        Trampa* trampa = dynamic_cast<Trampa*>(item);
        if (trampa && trampa->estaActiva()) {
            trampasActuales.insert(trampa);
            if (!trampasEnContacto.contains(trampa)) {
                trampasEnContacto.insert(trampa);

                goku->recibirDanio(10);
                actualizarBarraVida();

                trampa->iniciarDanioContinuo(this, [=]() {
                    goku->recibirDanio(10);
                    actualizarBarraVida();

                    if (goku->getVida() <= 0) {
                        for (Trampa* t : trampasEnContacto) {
                            t->detenerDanioContinuo();
                            scene->removeItem(t);
                            delete t;
                        }
                        trampasEnContacto.clear();
                        verificadorColisiones->stop();
                        temporizadorTrampas->stop();

                        goku->mostrarMuerte();

                        QTimer::singleShot(900, this, [=]() {
                            mostrarDerrota();
                        });
                    }
                });
            }
        }
    }

    for (Trampa* t : trampasEnContacto) {
        if (!trampasActuales.contains(t)) {
            t->detenerDanioContinuo();
        }
    }

    trampasEnContacto = trampasActuales;
}
