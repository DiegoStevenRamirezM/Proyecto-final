#include "trampa.h"

Trampa::Trampa(int dano)
{
    this->danio = dano;
    this->ancho = 80;
    this->alto = 80;
    this->activa = true;
    this->timer = nullptr;
    this->timerDanio = nullptr;
}

Trampa::~Trampa() {}

int Trampa::getDanio() const {
    return danio;
}

bool Trampa::estaActiva() const {
    return activa;
}

void Trampa::desactivar() {
    activa = false;
    detenerDanioContinuo();
}

QTimer* Trampa::getTimer() const {
    return timer;
}

void Trampa::iniciarDanioContinuo(QObject* receptor, std::function<void()> callback)
{
    if (!timerDanio) {
        timerDanio = new QTimer(receptor);
        QObject::connect(timerDanio, &QTimer::timeout, receptor, callback);
        timerDanio->start(800); // cada 800 ms
    }
}

void Trampa::detenerDanioContinuo()
{
    if (timerDanio) {
        timerDanio->stop();
        delete timerDanio;
        timerDanio = nullptr;
    }
}
