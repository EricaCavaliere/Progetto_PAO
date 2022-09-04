#include "view/mainwindow.h"
#include "control/controller.h"
#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller c;
    return a.exec();
}
