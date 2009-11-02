#include <QtGui/QApplication>
#include "deployer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Installer w;
    w.show();
    return a.exec();

}
