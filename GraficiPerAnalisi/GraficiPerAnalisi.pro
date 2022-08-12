QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    view/mainwindow.cpp \
    model/campione.cpp \
    model/sostanza.cpp \
    model/elemento.cpp \
    model/composto.cpp \
    model/miscela.cpp

HEADERS += \
    view/mainwindow.h \
    model/campione.h \
    model/sostanza.h \
    model/elemento.h \
    model/composto.h \
    model/miscela.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
