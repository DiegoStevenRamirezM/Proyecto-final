#ifndef TRAMPA_H
#define TRAMPA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Trampa : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Trampa(int dano = 5);
    virtual ~Trampa();

    virtual void actualizar() = 0;

    int getDanio() const;
    bool estaActiva() const;
    void desactivar();

    QTimer* getTimer() const;

    void iniciarDanioContinuo(QObject* receptor, std::function<void()> callback);
    void detenerDanioContinuo();

protected:
    QTimer* timer;         // para animaciones (uso en hijas)
    QTimer* timerDanio;    // para daño repetido
    int ancho;
    int alto;
    int danio;
    bool activa;
};

#endif // TRAMPA_H
