
#include <QtGui>
#include <QtGui/QApplication>
#include "deployer.h"
#include "ui_deployer.h"
#include "data_types.h"
#include <iostream>
#include <map>
#include <vector>
#include <QFile>
using namespace std;
typedef map<QString, QString> mapType;

Installer::Installer(QWidget *parent) : QMainWindow(parent), ui(new Ui::deployerUI)
{

    ui->setupUi(this);
    connect( ui->install, SIGNAL( pressed()), this, SLOT(install()) );

     QFile file("resources/deployer_config.cfg");
     if (!file.open(QIODevice::ReadOnly )){
        return;
     }
     QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = in.readLine();
         QStringList strlist = line.split(" ");
         list.insert( pair<QString,QString> (strlist.at(0), strlist.at(1)) );
     }

    /*mapType::iterator iter;
    iter = list.find(QString("server"));
    cout << list[QString("server")].toStdString() << endl;*/

}

Installer::~Installer() {

    delete ui;
}
void Installer::installServer() {
    QFile f("resources/server_deploy.txt");
    if( !f.open( QIODevice::ReadOnly) ) {
        return;
    }

    QDir dir( "");
    dir.mkdir( list[QString("server")] );
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList strlist = line.split(" ");
        QFile temp(strlist.at(0));
        QString dest = QString("%1%2").arg(list[QString("server")]).arg(QString("/%1").arg(strlist.at(1)) );

        temp.copy( dest );
     }

}
void Installer :: install() {
    cout << "INSTALL" << endl;
    switch( ui->installList->currentIndex()) {
        case 0 :
        {
            cout << list[QString("server")].toStdString() << endl;
            installServer();
            break;
        }
        case 1:
        {
            cout << list[QString("projector")].toStdString() << endl;
            break;
        }

        case 2:
        {
            cout << list[QString("client")].toStdString() << endl;
            break;
        }

        case 3:
        {
            cout << list[QString("registration")].toStdString() << endl;
            cout << list[QString("qeditor")].toStdString() << endl;
            break;
        }

        default:
            break;
    }
}



