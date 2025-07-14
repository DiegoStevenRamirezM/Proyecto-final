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
    temporizadorTrampasExtra = nullptr;

    // Crear barra de progreso
    barraProgreso = new QProgressBar(this);
    barraProgreso->setRange(0, 100);
    barraProgreso->setValue(0);
    barraProgreso->setGeometry(this->width() - 210, 10, 200, 20);
    barraProgreso->setTextVisible(true);
    barraProgreso->setStyleSheet("QProgressBar { border: 2px solid black; border-radius: 5px; background-color: white; }"
                                 "QProgressBar::chunk { background-color: green; }");

    // Temporizadores
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
    temporizadorTrampas->start(4500);

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

    //  Detectar si estamos en la habitación 2
    bool enHabitacion2 = habitacionDesbloqueada && paredCreada;
    int offsetX = enHabitacion2 ? 800 : 0;

    if (tipoAleatorio == "flecha") {
        bool haciaDerecha = QRandomGenerator::global()->bounded(0, 2);
        Flecha* f = new Flecha(haciaDerecha);
        int x = haciaDerecha ? offsetX : offsetX + 800;
        f->setPos(x, 460);
        f->setZValue(1);
        scene->addItem(f);
    } else {
        TrampaFija* trampa = new TrampaFija(tipoAleatorio);
        int x = offsetX + QRandomGenerator::global()->bounded(100, 700);
        int y = (tipoAleatorio == "acido") ? 460 : 480;
        trampa->setPos(x, y);
        trampa->setZValue(1);
        scene->addItem(trampa);
        trampasActivas.append(trampa);

        QTimer::singleShot(3000, [=]() {
            trampasActivas.removeOne(trampa);
            trampasEnContacto.remove(trampa);
            trampa->detenerDanioContinuo();

            if (trampa->scene()) {
                trampa->scene()->removeItem(trampa);
            }

            delete trampa;
        });



        trampa->actualizar();
    }

    trampasGeneradas++;
    int totalTrampasParaDesbloqueo = 20;
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

    segundosRestantes = 40;  // Cambiado de 60 a 40
    contadorTiempo = new QGraphicsTextItem(QString::number(segundosRestantes));
    contadorTiempo->setDefaultTextColor(Qt::white);
    contadorTiempo->setFont(QFont("Arial", 24, QFont::Bold));
    contadorTiempo->setZValue(100);
    contadorTiempo->setPos(1200, 15);
    scene->addItem(contadorTiempo);

    // Temporizador del contador de segundos
    temporizadorResistencia = new QTimer(this);
    connect(temporizadorResistencia, &QTimer::timeout, this, [=]() mutable {
        segundosRestantes--;
        contadorTiempo->setPlainText(QString::number(segundosRestantes));

        if (segundosRestantes <= 0 && pared) {
            temporizadorResistencia->stop();

            if (temporizadorTrampasExtra) {
                temporizadorTrampasExtra->stop();
                temporizadorTrampasExtra->deleteLater();
                temporizadorTrampasExtra = nullptr;
            }

            // Verificar que pared exista y esté en la escena antes de eliminarla
            if (pared->scene()) {
                scene->removeItem(pared);
            }
            delete pared;
            pared = nullptr;

            // Verificar que contadorTiempo esté en escena antes de eliminarlo
            if (contadorTiempo && contadorTiempo->scene()) {
                scene->removeItem(contadorTiempo);
            }
            delete contadorTiempo;
            contadorTiempo = nullptr;

            // Llamar a mostrarVictoria después de limpiar todo
            QTimer::singleShot(100, this, [=]() {
                mostrarVictoria();
            });
        }
    });
    temporizadorResistencia->start(1000);  // Cada 1 segundo

    // 👇 NUEVO: Generación de trampas normales durante esos 40 segundos
    temporizadorTrampasExtra = new QTimer(this);
    connect(temporizadorTrampasExtra, &QTimer::timeout, this, [=]() {
        generarTrampa();  // Se reutiliza tu función existente
    });
    temporizadorTrampasExtra->start(1500);  // Cada 1.5 segundos
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

                goku->recibirDanio(trampa->getDanio());
                actualizarBarraVida();

                trampa->iniciarDanioContinuo(this, [=]() {
                    goku->recibirDanio(trampa->getDanio());
                    actualizarBarraVida();

                    if (goku->getVida() <= 0) {
                        for (Trampa* t : trampasEnContacto) {
                            t->detenerDanioContinuo();
                            if (t->scene()) {
                                t->scene()->removeItem(t);
                            }
                            delete t;
                        }

                        trampasEnContacto.clear();

                        verificadorColisiones->stop();
                        temporizadorTrampas->stop();

                        // 🟡 NUEVO: detener los temporizadores adicionales si existen
                        if (temporizadorResistencia) {
                            temporizadorResistencia->stop();
                        }
                        if (temporizadorTrampasExtra) {
                            temporizadorTrampasExtra->stop();
                            temporizadorTrampasExtra->deleteLater();
                            temporizadorTrampasExtra = nullptr;
                        }

                        goku->mostrarMuerte();


                        QTimer::singleShot(900, this, [=]() {
                            mostrarDerrota();  // ✅ Llamada directa
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
void Nivel1View::detenerNivel()
{
    if (verificadorSalida) verificadorSalida->stop();
    if (temporizadorTrampas) temporizadorTrampas->stop();
    if (temporizadorTrampasExtra) temporizadorTrampasExtra->stop();
    if (verificadorColisiones) verificadorColisiones->stop();
    if (temporizadorResistencia) temporizadorResistencia->stop();
}
