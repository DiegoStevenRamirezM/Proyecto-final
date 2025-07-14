#ifndef NIVEL1VIEW_H
#define NIVEL1VIEW_H

#include "nivel.h"
#include "trampafija.h"
#include "flecha.h"
#include "pared.h"
#include <QTimer>
#include <QSet>

class Nivel1View : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel1View(QWidget *parent = nullptr);
    void detenerNivel() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void verificarColisiones();

private:
    void generarTrampa();
    void iniciarDesafioPared();

    QTimer *temporizadorTrampas;
    QTimer *verificadorColisiones;
    QTimer *temporizadorResistencia;
    QTimer *temporizadorTrampasExtra;
    QTimer *verificadorSalida;

    QList<Trampa*> trampasActivas;
    QSet<Trampa*> trampasEnContacto;

    QGraphicsTextItem* mensajeDesbloqueo;
    QGraphicsTextItem* contadorTiempo;
    Pared* pared;

    int trampasGeneradas;
    int segundosRestantes;
    bool habitacionDesbloqueada;
    bool paredCreada;
};

#endif // NIVEL1VIEW_H
