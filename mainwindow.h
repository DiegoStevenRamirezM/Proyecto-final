#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include "nivel1view.h"
#include "nivel2view.h"
#include <QSoundEffect>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarNivel1();
    void iniciarNivel2();
    void volverAlMenu();

private:
    QStackedWidget *stackedWidget;
    QWidget *menuWidget;
    Nivel1View *nivel1;
    Nivel2View *nivel2;
    QSoundEffect *musicaMenu;
};

#endif // MAINWINDOW_H
