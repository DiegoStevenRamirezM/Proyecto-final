#include "nivel.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>

Nivel::Nivel(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    int anchoVista = 1600;
    int anchoVentana = 800;
    int altoVista = 600;

    scene->setSceneRect(0, 0, anchoVista, altoVista);
    this->setFixedSize(anchoVentana, altoVista);

    // Barra de vida
    widgetVida = new QWidget(this);
    widgetVida->setGeometry(10, 10, 230, 30);
    layoutVida = new QHBoxLayout(widgetVida);
    layoutVida->setContentsMargins(0, 0, 0, 0);
    layoutVida->setSpacing(5);

    iconoVida = new QLabel();
    iconoVida->setPixmap(QPixmap(":/imagenes/corazon.png").scaled(40, 40));
    layoutVida->addWidget(iconoVida);

    barraVida = new QProgressBar();
    barraVida->setRange(0, 100);
    barraVida->setValue(100);
    barraVida->setTextVisible(false);
    barraVida->setFixedHeight(20);
    barraVida->setStyleSheet(
        "QProgressBar { border: 2px solid black; border-radius: 5px; background-color: black; }"
        "QProgressBar::chunk { background-color: red; }"
        );
    layoutVida->addWidget(barraVida);

    textoVida = new QLabel("100/100");
    textoVida->setStyleSheet("color: white; font-weight: bold;");
    layoutVida->addWidget(textoVida);
}

void Nivel::inicializarEscena(const QString& fondoPath)
{
    QPixmap fondoOriginal(fondoPath);
    QPixmap fondoEscalado = fondoOriginal.scaled(1600, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondoEscalado);
    fondoItem->setZValue(-1);
    fondoItem->setPos(0, 0);
    scene->addItem(fondoItem);

    goku = new Protagonista();
    goku->setPos(100, 450);
    goku->setZValue(1);
    scene->addItem(goku);
}

void Nivel::actualizarBarraVida()
{
    int vidaActual = goku->getVida();
    barraVida->setValue(vidaActual);
    textoVida->setText(QString::number(vidaActual) + "/100");
}

void Nivel::mostrarVictoria()
{
    if (widgetVida) widgetVida->hide();

    QPixmap imagenWin(":/imagenes/win.png");
    QPixmap escalada = imagenWin.scaled(1000, 700, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* cartelWin = new QGraphicsPixmapItem(escalada);
    cartelWin->setZValue(200);
    qreal centroX = (scene->width() - escalada.width()) / 2;
    qreal centroY = (scene->height() - escalada.height()) / 2;
    cartelWin->setPos(centroX, centroY);
    scene->addItem(cartelWin);

    centerOn(centroX + escalada.width() / 2, centroY + escalada.height() / 2);

    QTimer::singleShot(3000, this, [=]() {
        emit nivelCompletado();
    });
}

void Nivel::mostrarDerrota()
{
    if (widgetVida) widgetVida->hide();
    if (barraProgreso) barraProgreso->hide();

    QPixmap imagenGameOver(":/imagenes/game_over.png");
    QPixmap escalada = imagenGameOver.scaled(1000, 800, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* cartel = new QGraphicsPixmapItem(escalada);
    cartel->setZValue(200);
    qreal centroX = (scene->width() - escalada.width()) / 2;
    qreal centroY = (scene->height() - escalada.height()) / 2;
    cartel->setPos(centroX, centroY);
    scene->addItem(cartel);

    centerOn(centroX + escalada.width() / 2, centroY + escalada.height() / 2);

    QTimer::singleShot(3000, this, [=]() {
        emit gokuMurio();
    });
}
