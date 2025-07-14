#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

#include "personaje.h"
#include "enemigo.h"
#include <QTimer>

class Protagonista : public Personaje
{
    Q_OBJECT

public:
    Protagonista();
    void moverDerecha();
    void moverIzquierda();
    void saltar();
    void detenerse();
    void mostrarMuerte();
    void permitirAvanceCompleto(bool valor);
    void lanzarKamehameha();
    void animarKamehameha();
    void setEnemigo(Enemigo* enemigo); // para enlazar a Pilaf
    void curarse();
    void animarPunio();
    void animarPatada();
    void animarBolaEnergia();
    bool getEstaOcupado() const { return estaOcupado; }

protected:
    void mostrarAnimacionDanio() override;

private slots:
    void siguienteFrame() override;
    void animarSalto();

private:
    QTimer *timerSalto;
    int yFrameFijo;
    double tiempo, g, v0, velocidadHorizontal;
    int yInicial, direccionSalto;
    bool enElAire;
    bool limiteDerechoActivo;

    enum Estado { Corriendo, Saltando, Quieto };
    Estado estado;

    // Atributo necesario para el sprite del kamehameha
    QPixmap spriteKamehameha;

    Enemigo* enemigoActual = nullptr;  // Para nivel 2

    //variable para que no se hagan multiples golpes a la vez
    bool estaOcupado = false;
};

#endif // PROTAGONISTA_H
