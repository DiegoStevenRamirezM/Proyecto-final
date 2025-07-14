#ifndef NIVEL2VIEW_H
#define NIVEL2VIEW_H

#include "nivel.h"
#include "enemigo.h"
#include <QProgressBar>
#include <QTimer>

class Nivel2View : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel2View(QWidget *parent = nullptr);
    void actualizarBarraEnemigo();
    void detenerNivel() override;  //importante para evitar crashes al volver al menú

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Enemigo *pilaf;
    QProgressBar *barraEnemigo;
    QTimer *temporizadorAtaque;
};

#endif // NIVEL2VIEW_H
