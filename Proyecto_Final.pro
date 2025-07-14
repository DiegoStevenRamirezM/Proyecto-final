QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    enemigo.cpp \
    flecha.cpp \
    main.cpp \
    mainwindow.cpp \
    nivel.cpp \
    nivel1view.cpp \
    nivel2view.cpp \
    pared.cpp \
    personaje.cpp \
    protagonista.cpp \
    trampa.cpp \
    trampafija.cpp

HEADERS += \
    enemigo.h \
    flecha.h \
    mainwindow.h \
    nivel.h \
    nivel1view.h \
    nivel2view.h \
    pared.h \
    personaje.h \
    protagonista.h \
    trampa.h \
    trampafija.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

