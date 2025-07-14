#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDebug>
#include <QSoundEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 600);

    // Crear el stackedWidget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Crear menú principal
    menuWidget = new QWidget();
    QLabel *fondo = new QLabel(menuWidget);
    fondo->setPixmap(QPixmap(":/imagenes/Fondo_menu.png").scaled(800, 600));
    fondo->setFixedSize(800, 600);
    fondo->lower();

    QVBoxLayout *layout = new QVBoxLayout(menuWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(250, 180, 250, 180);

    QPushButton *btnNivel1 = new QPushButton("Desafío en la Fortaleza");
    QPushButton *btnNivel2 = new QPushButton("Batalla Final contra Pilaf");
    QPushButton *btnSalir = new QPushButton("Salir");

    layout->addWidget(btnNivel1);
    layout->addWidget(btnNivel2);
    layout->addWidget(btnSalir);
    menuWidget->setLayout(layout);

    QString estilo = "font-size: 18px; padding: 10px; background-color: orange; border-radius: 10px;";
    btnNivel1->setStyleSheet(estilo);
    btnNivel2->setStyleSheet(estilo);
    btnSalir->setStyleSheet("font-size: 18px; padding: 10px; background-color: red; color: white; border-radius: 10px;");

    // Agregar solo el menú inicialmente
    stackedWidget->addWidget(menuWidget);
    stackedWidget->setCurrentWidget(menuWidget);

    // Inicializar punteros a nullptr
    nivel1 = nullptr;
    nivel2 = nullptr;

    // Conectar botones del menú
    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
    connect(btnSalir, &QPushButton::clicked, this, &QApplication::quit);

    // Música de fondo del menú
    musicaMenu = new QSoundEffect(this);
    musicaMenu->setSource(QUrl("qrc:/sonidos/musica_fondo.wav"));
    musicaMenu->setLoopCount(QSoundEffect::Infinite);
    musicaMenu->setVolume(0.4f);  // Puedes ajustar el volumen si es muy fuerte
    musicaMenu->play();
}

MainWindow::~MainWindow()
{
    delete nivel1;
    delete nivel2;
}

void MainWindow::iniciarNivel1()
{
    if (!nivel1) {
        nivel1 = new Nivel1View();
        stackedWidget->addWidget(nivel1);

        connect(nivel1, &Nivel1View::nivelCompletado, this, &MainWindow::volverAlMenu);
        connect(nivel1, &Nivel1View::gokuMurio, this, &MainWindow::volverAlMenu);
    }

    stackedWidget->setCurrentWidget(nivel1);
}

void MainWindow::iniciarNivel2()
{
    if (!nivel2) {
        nivel2 = new Nivel2View();
        stackedWidget->addWidget(nivel2);

        connect(nivel2, &Nivel2View::nivelCompletado, this, &MainWindow::volverAlMenu);
        connect(nivel2, &Nivel2View::gokuMurio, this, &MainWindow::volverAlMenu);
    }

    stackedWidget->setCurrentWidget(nivel2);
}

void MainWindow::volverAlMenu()
{
    if (stackedWidget->currentWidget() == nivel1 && nivel1)
        nivel1->detenerNivel();

    if (stackedWidget->currentWidget() == nivel2 && nivel2)
        nivel2->detenerNivel();

    stackedWidget->setCurrentWidget(menuWidget);
}
