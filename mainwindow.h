#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "nivel1view.h"
#include "nivel2view.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void iniciarNivel1();
    void iniciarNivel2();

private:
    QWidget *menuWidget;
};

#endif // MAINWINDOW_H
