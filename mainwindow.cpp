#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 600);

    menuWidget = new QWidget(this);

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
    setCentralWidget(menuWidget);

    QString estilo = "font-size: 18px; padding: 10px; background-color: orange; border-radius: 10px;";
    btnNivel1->setStyleSheet(estilo);
    btnNivel2->setStyleSheet(estilo);
    btnSalir->setStyleSheet("font-size: 18px; padding: 10px; background-color: red; color: white; border-radius: 10px;");

    connect(btnNivel1, &QPushButton::clicked, this, &MainWindow::iniciarNivel1);
    connect(btnNivel2, &QPushButton::clicked, this, &MainWindow::iniciarNivel2);
    connect(btnSalir, &QPushButton::clicked, this, &QApplication::quit);
}

MainWindow::~MainWindow() {}
void MainWindow::iniciarNivel1()
{
    this->hide();  // Oculta el menú

    Nivel1View *nivel = new Nivel1View();
    nivel->setAttribute(Qt::WA_DeleteOnClose); // Libera memoria al cerrarse
    nivel->show();

    connect(nivel, &Nivel1View::nivelCompletado, this, [=]() {
        nivel->close();    // Cierra el nivel
        this->show();      // Muestra de nuevo el menú
    });

    connect(nivel, &Nivel1View::gokuMurio, this, [=]() {
        nivel->close();    // Cierra el nivel
        this->show();      // Vuelve al menú
    });
}

void MainWindow::iniciarNivel2()
{
    this->hide();

    Nivel2View *nivel = new Nivel2View();
    nivel->setAttribute(Qt::WA_DeleteOnClose);
    nivel->show();

    connect(nivel, &Nivel2View::nivelCompletado, this, [=]() {
        nivel->close();
        this->show();
    });

    connect(nivel, &Nivel2View::gokuMurio, this, [=]() {
        nivel->close();
        this->show();
    });
}
