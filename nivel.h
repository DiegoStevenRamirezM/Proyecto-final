#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "protagonista.h"

class Nivel : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Nivel(QWidget *parent = nullptr);
    void actualizarBarraVida();
    void mostrarVictoria();
    void mostrarDerrota();
    virtual void detenerNivel(); // se redefine en cada nivel

signals:
    void nivelCompletado();
    void gokuMurio();

protected:
    QGraphicsScene* scene;
    Protagonista* goku;
    QProgressBar* barraVida;
    QLabel* textoVida;
    QLabel* iconoVida;
    QWidget* widgetVida;
    QHBoxLayout* layoutVida;
    QProgressBar* barraProgreso = nullptr;

    void inicializarEscena(const QString& fondoPath);
};

#endif // NIVEL_H
