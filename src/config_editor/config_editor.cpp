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

#include <QFile>
#include <QString>
#include <QtTest/QtTest>
#include <QMessageBox>
#include "config_editor.h"
#include "ui_server_editor.h"
#include <iostream>
#include <data_types.h>
#include <util/xml_util.h>
//#include <QVERIFY>
using namespace std;

ConfigEditor::ConfigEditor(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::config_editor)
{
    ui->setupUi(this);
    connect( ui->save, SIGNAL(pressed()), this, SLOT(save()) );
    connect( ui->load, SIGNAL(pressed()), this, SLOT(load()) );
    connect( ui->serverPort1, SIGNAL( textChanged(QString)), this, SLOT(Port1Changed()) );
    connect( ui->serverPort2, SIGNAL( textChanged(QString)), this, SLOT(Port2Changed()) );

}

ConfigEditor::~ConfigEditor()
{
    delete ui;
}
void ConfigEditor:: Port1Changed() {
    ui->serverPort2->setText( ui->serverPort1->text() );

}

void ConfigEditor:: Port2Changed() {
    ui->serverPort1->setText( ui->serverPort2->text() );

}

bool ConfigEditor::askConfirmation( QString& s ) {
    QMessageBox msgBox;
    msgBox.setText(QString("The File for %1 Already Exists.").arg(s));
    msgBox.setInformativeText("Do you want to overwrite file?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No  );
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret) {
       case QMessageBox::Yes:
           return true;
           break;
       case QMessageBox::No:
           return false;
           break;

    }
}

bool ConfigEditor::showInfo( QString s ) {
    QMessageBox msgBox;
     msgBox.setText(s );

     msgBox.setStandardButtons(QMessageBox::Ok);
     msgBox.setDefaultButton(QMessageBox::Ok);
     int ret = msgBox.exec();

}
void ConfigEditor::save() {

    ui->error->setText("");
    ui->tabWidget_2->setTabText(0, "Round1");
    ui->tabWidget_2->setTabText(1, "Round2");
    ui->tabWidget_2->setTabText(2, "Round3");
    ui->tabWidget_2->setTabText(3, "Round4");
    ui->label_sql->setText("SQL File");
    ui->label_q1->setText("Question File");
    ui->label_q2->setText("Question File");
    ui->label_q3->setText("Question File");
    ui->label_q4->setText("Question File");



    QString port(ui->serverPort1->text());
    QString ip( ui->serverIP->text());
    QString server( ui->serverConf->text() );
    QString contestant ( ui->constConf->text() );
    QString admin ( ui->adminConf->text() );
    QString presenter ( ui->presenterConf->text() );
    QString sql ( ui->sql->text() );

    QString r1_q(ui->q_rnd1->text());
    QString r2_q(ui->q_rnd2->text());
    QString r3_q(ui->q_rnd3->text());
    QString r4_q(ui->q_rnd4->text());


    bool cont = true;

    QFile file ( server );
    if( file.exists() && server.compare(QString("")) != 0 ) {
        QString temp("server");
        cont = askConfirmation(temp);

    }
    else if( file.exists() && server.compare(QString("") ) == 0 ) {
        // give out eror
    }
    
    if( cont ) {
        
        QFile sqFile(sql);
        if( !sqFile.exists() ) {
            ui->label_sql->setText("SQL File*");
            cont = false;
        }

        QFile q1File(r1_q);
        if( !q1File.exists() ) {
            ui->tabWidget_2->setTabText(0, "Round1*");
            ui->label_q1->setText("Question File*");
            cout << r1_q.toStdString() << endl;
            cont = false;
        }      

        QFile q2File(r2_q);
        if( !q2File.exists() ) {
            ui->tabWidget_2->setTabText(1, "Round2*");
            ui->label_q2->setText("Question File*");
            cont = false;
        }

        QFile q3File(r3_q);
        if( !q3File.exists() ) {
            ui->tabWidget_2->setTabText(2, "Round3*");
            ui->label_q3->setText("Question File*");
            cont = false;
        }

        QFile q4File(r4_q);
        if( !q4File.exists() ) {
            ui->tabWidget_2->setTabText(3, "Round4*");
            ui->label_q4->setText("Question File*");
            cont = false;
        }
    }

    if( !cont) {
        showInfo( QString("Some Files Doesnt Exist") );
    }

    if( cont ) {
        QFile file2 ( contestant );
        if( file2.exists() ) {
            QString temp("contestant");
            cont = askConfirmation(temp);

        }
    }
    if( cont ) {
        QFile file3 ( admin );
        if( file3.exists() ) {
            QString temp("admin");
            cont = askConfirmation(temp);

        }
    }

    if( cont ) {
           QFile file4 ( presenter );
        if( file4.exists() ) {
            QString temp("presenter");
            cont = askConfirmation(temp);

        }
    }

    if( cont ) {
        ui->error->setText( "OK" );
        XmlUtil& xu = XmlUtil::getInstance();

        //Save everything...

        //ClientConfig *cc = new ClientConfig();
        ProjectorConfig *pc = new ProjectorConfig();
        ClientConfig *cc = new ClientConfig();
        AdminConfig *ac = new AdminConfig();

        cc->ip = ip;
        cc->port = port.toInt();
        pc->ip = ip;
        pc->port = port.toInt();
        ac->ip = ip;
        ac->port = port.toInt();

        ServerConfig *sc = new ServerConfig();
        sc->db_path = sql;
        sc->port = port.toInt();
        sc->stage_files.push_back(StageFile(r1_q));
        sc->stage_files.push_back(StageFile(r2_q));
        sc->stage_files.push_back(StageFile(r3_q));
        sc->stage_files.push_back(StageFile(r4_q));


        QString temp;

        xu.writeNetConfig ( *pc, temp );
        QFile f(presenter);
        f.open(QIODevice::WriteOnly);
        f.write(temp.toUtf8());
        temp.clear();

        //delete f;

        xu.writeClientConfig ( *cc, temp );
        QFile f1(contestant);
        f1.open(QIODevice::WriteOnly);
        f1.write(temp.toUtf8());
        temp.clear();

        xu.writeNetConfig ( *ac, temp );
        QFile f2(admin);
        f2.open(QIODevice::WriteOnly);
        f2.write(temp.toUtf8());
        temp.clear();

        xu.writeServerConfig ( *sc, temp );
        QFile f3(server);
        f3.open(QIODevice::WriteOnly);
        f3.write(temp.toUtf8());

    }
    showInfo( QString("Config Saved") );

    //ui->error->setText(*error);
}

void ConfigEditor::load() {

    ui->error->setText( "" );
    XmlUtil& xu = XmlUtil::getInstance();
    bool check = false;

    if( ui->presenterConf->text().compare(QString( "" ) ) != 0 ) {
        ProjectorConfig *nc = new ProjectorConfig();
        QString network(ui->presenterConf->text());
        QFile file( network );


        if( file.exists() ) {
            check = true;
            QVERIFY(file.open(QIODevice::ReadOnly));
            QString xml = file.readAll();
            xu.readProjectorConfig ( xml , *nc );
            ui->serverIP->setText( nc->ip );
            ui->serverPort1->setText( QString("%1").arg(nc->port) );
            ui->serverPort2->setText( QString("%1").arg(nc->port) );

        }
        else {
            showInfo( QString("File path for projector config not found") );
        }
    }

    else if( ui->adminConf->text().compare(QString( "" ) ) != 0 ) {
        AdminConfig nc;
        QString network(ui->adminConf->text());
        QFile file (network);

        if( file.exists() ) {
            check = true;
            QVERIFY(file.open(QIODevice::ReadOnly));
            QString xml = file.readAll();
            
             try {
                   xu.readAdminConfig( xml, nc );

            } catch (XmlUtil::XmlException e) {
                    cout << e.what() << endl;
            }
            ui->serverIP->setText( nc.ip );
            ui->serverPort1->setText( QString("%1").arg(nc.port) );
            ui->serverPort2->setText( QString("%1").arg(nc.port) );
        }
        else {
            showInfo( QString("File path for admin config not found") );
        }
    }

    else if( ui->constConf->text().compare(QString( "" ) ) != 0 ) {
        ClientConfig *cc = new ClientConfig();
        QString network(ui->constConf->text());
        QFile file (network);


        if( file.exists() ) {
            check = true;
            QVERIFY(file.open(QIODevice::ReadOnly));
            QString xml = file.readAll();

            try {
                    xu.readClientConfig( xml, *cc );
            } catch (XmlUtil::XmlException e) {
                    cout << e.what() << endl;
            }

            ui->serverIP->setText( cc->ip );
            ui->serverPort1->setText( QString("%1").arg(cc->port) );
            ui->serverPort2->setText( QString("%1").arg(cc->port) );
        }
        else {
            showInfo( QString("File path for contestant config not found") );
        }


    }

    if( ui->serverConf->text() .compare(QString( "" ) ) != 0 ) {
        ServerConfig *sc = new ServerConfig();
        QString network( ui->serverConf->text() );
        QFile file (network);

        if( file.exists() ) {
            check = true;
            QVERIFY(file.open(QIODevice::ReadOnly));
            QString xml = file.readAll();

            try {
                    xu.readServerConfig( xml, *sc );
                    ui->sql->setText( sc->db_path );
                    ui->serverPort1->setText( QString("%1").arg(sc->port) );
                    ui->serverPort2->setText( QString("%1").arg(sc->port) );

                    ui->q_rnd1->setText(sc->stage_files.at(0));
                    ui->q_rnd2->setText(sc->stage_files.at(1));
                    ui->q_rnd3->setText(sc->stage_files.at(2));
                    ui->q_rnd4->setText(sc->stage_files.at(3));

            } catch (XmlUtil::XmlException e) {
                    cout << e.what() << endl;
            }


        }
        else {
            showInfo( QString("File path for server config not found") );
        }
    }


    if( check == false ) {
        ui->error->setText( "Please add at least one correct path to fill something" );
    }
}
