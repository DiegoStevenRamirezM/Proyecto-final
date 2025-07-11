#ifndef NIVEL1VIEW_H
#define NIVEL1VIEW_H

#include "nivel.h"
#include <QTimer>
#include <QList>
#include <QSet>
#include <QProgressBar>
#include "trampafija.h"
#include "flecha.h"
#include "pared.h"

class Nivel1View : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel1View(QWidget *parent = nullptr);


private slots:
    void verificarColisiones();

private:
    // Trampas
    QTimer *temporizadorTrampas;
    QTimer *verificadorColisiones;
    QList<Trampa*> trampasActivas;
    QSet<Trampa*> trampasEnContacto;

    // Desbloqueo de habitación
    int trampasGeneradas;
    bool habitacionDesbloqueada;
    QProgressBar* barraProgreso;
    QGraphicsTextItem* mensajeDesbloqueo;

    // Transición y desafíos
    QTimer *verificadorSalida;
    bool paredCreada;
    Pared* pared;

    // Desafío de resistencia
    QTimer* temporizadorResistencia;
    int segundosRestantes;
    QGraphicsTextItem* contadorTiempo;

    // Métodos internos
    void generarTrampa();
    void eliminarTrampa(Trampa* trampa);
    void iniciarDesafioPared();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // NIVEL1VIEW_H
