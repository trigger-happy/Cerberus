/*
Copyright (C) 2009 Janssen Go

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <QtGui>
#include <QtGui/QApplication>
#include "deployer.h"
#include "ui_deployer.h"
#include "data_types.h"
#include <iostream>
#include <QMessageBox>
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

}

Installer::~Installer() {

    delete ui;
}

bool Installer::showInfo( QString s ) {
    QMessageBox msgBox;
     msgBox.setText(s );

     msgBox.setStandardButtons(QMessageBox::Ok);
     msgBox.setDefaultButton(QMessageBox::Ok);
     int ret = msgBox.exec();

}

void Installer::deploy( QString& source, QString& component ) {
    QFile f(source);
    if( !f.exists() ) {
        showInfo( QString( "File %1 not found.").arg(source) );
        return;
    }

    if( !f.open( QIODevice::ReadOnly) ) {
        showInfo( QString( "An unknown error has occured" ) );
        return;
    }

    QStringList path = list[QString(component)].split("/");
    QString filler( "" );
    QDir dir( "" );

    for( int i = 0; i < path.size(); i++ ) {
        filler.append( QString( "%1/").arg(path.at(i) ) );
        dir.mkdir(filler);
    }

    QTextStream in(&f);

    bool isSuccessful = true;
    bool hasExe = false;
    int round = 0;
    while (!in.atEnd()) {
        round++;
        QString line = in.readLine();
        QStringList strlist = line.split(" ");
        QFile temp(strlist.at(0));

        if( temp.exists() && round == 1 ) {
            hasExe = true;
        }
        else if( !temp.exists() && round == 1 ) {
            continue;
        }
        else if( !temp.exists() && round == 2 && hasExe == true ) {
            continue;
        }

        QString dest = QString("%1%2").arg(list[QString(component)]).arg(QString("/%1").arg(strlist.at(1)) );
        if( QFile::exists( dest ) ) {
            isSuccessful = false;
            showInfo( QString( "File %1 already exists." ).arg( dest ) );
        }
        else {
            bool checker = temp.copy( dest );
            if( checker == false ) {
                isSuccessful = false;
                showInfo( QString( "File %1 was not copied." ).arg(temp.fileName()) );
            }
        }
     }
    f.close();
    if( isSuccessful ) showInfo( QString("The component '%1' was successfully deployed").arg(component) );
}

void Installer :: install() {
    cout << "INSTALL" << endl;
    switch( ui->installList->currentIndex()) {
        case 0 :
        {
            cout << list[QString("server")].toStdString() << endl;
            QString source("resources/server_deploy.txt");
            QString comp("server");
            deploy( source, comp  );
            break;
        }
        case 1:
        {
            cout << list[QString("projector")].toStdString() << endl;
            QString source("resources/projector_deploy.txt");
            QString comp("projector");
            deploy( source, comp  );
            break;
        }

        case 2:
        {
            cout << list[QString("client")].toStdString() << endl;
            QString source("resources/client_deploy.txt");
            QString comp("client");
            deploy( source, comp  );
            break;
        }

        case 3:
        {
            cout << list[QString("registration")].toStdString() << endl;
            QString source("resources/registration_deploy.txt");
            QString comp("registration");
            deploy( source, comp  );
            break;
        }

        case 4:
        {
            cout << list[QString("qeditor")].toStdString() << endl;
            QString source("resources/qeditor_deploy.txt");
            QString comp("qeditor");
            deploy( source, comp  );
        }
        default:
            break;
    }
}



