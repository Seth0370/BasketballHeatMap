#include "shootingmap.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShootingMap w;
    w.show();
    return a.exec();
}
