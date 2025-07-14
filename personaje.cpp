#include "personaje.h"
#include <QSoundEffect>

Personaje::Personaje(int vidaInicial)
    : vida(vidaInicial), frameActual(0), anchoFrame(40), altoFrame(40), totalFrames(1)
{
    timer = new QTimer(this);
    muerto = false;

}

Personaje::~Personaje() {}

void Personaje::recibirDanio(int cantidad)
{
    if (muerto) return;  // No seguir recibiendo daño si ya está muerto

    vida -= cantidad;
    if (vida <= 0) {
        vida = 0;
        muerto = true;  //  marcar que está muerto
        return;
    }
    mostrarAnimacionDanio();
}


int Personaje::getVida() const { return vida; }

void Personaje::mostrarAnimacionDanio() {}

#include <QSoundEffect>

void Personaje::reproducirSonido(const QString& ruta) {
    QSoundEffect* efecto = new QSoundEffect(this);
    efecto->setSource(QUrl("qrc:/sonidos/" + ruta));
    efecto->setVolume(0.5f);  // Volumen entre 0.0 y 1.0
    efecto->play();

    // Eliminar automáticamente después de reproducir
    connect(efecto, &QSoundEffect::playingChanged, [=]() {
        if (!efecto->isPlaying()) {
            delete efecto;
        }
    });
}

