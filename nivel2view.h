#ifndef NIVEL2VIEW_H
#define NIVEL2VIEW_H

#include "nivel.h"
#include "enemigo.h"
#include <QProgressBar>

class Nivel2View : public Nivel
{
    Q_OBJECT

public:
    explicit Nivel2View(QWidget *parent = nullptr);
    void actualizarBarraEnemigo();

private:
    Enemigo *pilaf;
    QProgressBar *barraEnemigo;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // NIVEL2VIEW_H
