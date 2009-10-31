#include <QtGui/QApplication>
#include "config_editor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigEditor w;
    w.show();
    return a.exec();

}

 /*int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     QWidget *widget = new QWidget;
     Ui::server_editor ui;
     ui.setupUi(widget);

     widget->show();
     return app.exec();
 }*/
