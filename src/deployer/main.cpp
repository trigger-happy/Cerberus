#include <QtGui/QApplication>
#include "deployer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigEditor w;
    w.show();
    return a.exec();

}
