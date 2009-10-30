#include <QFile>
#include <QString>
#include <QMessageBox>
#include "config_editor.h"
#include "ui_server_editor.h"
#include <iostream>
#include <data_types.h>
#include <util/xml_util.h>
using namespace std;

ConfigEditor::ConfigEditor(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::config_editor)
{
    ui->setupUi(this);

    //connects signal to slot
    connect( ui->save, SIGNAL(pressed()), this, SLOT(save()) );
    connect( ui->serverPort1, SIGNAL( textChanged(QString)), this, SLOT(Port1Changed()) );
    connect( ui->serverPort2, SIGNAL( textChanged(QString)), this, SLOT(Port2Changed()) );
}

ConfigEditor::~ConfigEditor()
{
    delete ui;
}

//line edit1 is changed, change line edit2 as well. And vice versa
void ConfigEditor:: Port1Changed() {
    ui->serverPort2->setText( ui->serverPort1->text() );

}

void ConfigEditor:: Port2Changed() {
    ui->serverPort1->setText( ui->serverPort2->text() );

}

//shows a msg box asking for confirmation to overwrite file
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

//shows a msg box
bool ConfigEditor::showInfo() {
    QMessageBox msgBox;
     msgBox.setText("Some Files Doesn't Exist.");

     msgBox.setStandardButtons(QMessageBox::Ok);
     msgBox.setDefaultButton(QMessageBox::Ok);
     int ret = msgBox.exec();

}

//Saves to the specific paths given by the user.

void ConfigEditor::save() {

    /*Resets Labels*/
    ui->error->setText("");
    ui->tabWidget_2->setTabText(0, "Round1");
    ui->tabWidget_2->setTabText(1, "Round2");
    ui->tabWidget_2->setTabText(2, "Round3");
    ui->tabWidget_2->setTabText(3, "Round4");
    ui->sql->setText("SQL File");
    ui->label_q1->setText("Question File");
    ui->label_q2->setText("Question File");
    ui->label_q3->setText("Question File");
    ui->label_q4->setText("Question File");
    ui->label_a1->setText("Answer File");
    ui->label_a2->setText("Answer File");
    ui->label_a3->setText("Answer File");
    ui->label_a4->setText("Answer File");


    /*Gets user inputted data from Line Edit*/
    QString port(ui->serverPort1->text());
    QString ip( ui->serverIP->text());
    QString server( ui->serverConf->text() );
    QString contestant ( ui->constConf->text() );
    QString admin ( ui->adminConf->text() );
    QString presenter ( ui->presenterConf->text() );
    QString sql ( ui->sql->text() );

    QString r1_q(ui->q_rnd1->text());
    QString r1_a(ui->a_rnd1->text());
    QString r2_q(ui->q_rnd2->text());
    QString r2_a(ui->a_rnd2->text());
    QString r3_q(ui->q_rnd3->text());
    QString r3_a(ui->a_rnd3->text());
    QString r4_q(ui->q_rnd4->text());
    QString r4_a(ui->a_rnd4->text());


    bool cont = true;

    //checks if server config path exists
    QFile file ( server );
    if( file.exists() ) {
        QString temp("server");
        cont = askConfirmation(temp);

    }
    
    //checks if sql and question file paths exist
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
            cont = false;
        }

        QFile a1File( r1_a) ;
        if( !a1File.exists() ) {
            ui->tabWidget_2->setTabText(0, "Round1*");
            ui->label_a1->setText("Answer File*");
            cont = false;
        }

        QFile q2File(r2_q);
        if( !q2File.exists() ) {
            ui->tabWidget_2->setTabText(1, "Round2*");
            ui->label_q2->setText("Question File*");
            cont = false;
        }

        QFile a2File( r2_a) ;
        if( !a2File.exists() ) {
            ui->tabWidget_2->setTabText(1, "Round2*");
            ui->label_a2->setText("Answer File*");
            cont = false;
        }

        QFile q3File(r3_q);
        if( !q3File.exists() ) {
            ui->tabWidget_2->setTabText(2, "Round3*");
            ui->label_q3->setText("Question File*");
            cont = false;
        }

        QFile a3File( r3_a) ;
        if( !a3File.exists() ) {
            ui->tabWidget_2->setTabText(2, "Round3*");
            ui->label_a3->setText("Answer File*");
            cont = false;
        }

        QFile q4File(r4_q);
        if( !q4File.exists() ) {
            ui->tabWidget_2->setTabText(3, "Round4*");
            ui->label_q4->setText("Question File*");
            cont = false;
        }

        QFile a4File( r4_a) ;
        if( !a4File.exists() ) {
            ui->tabWidget_2->setTabText(3, "Round4*");
            ui->label_a4->setText("Answer File*");
            cont = false;
        }

    }

    if( !cont) {
        showInfo();
    }

    //check if contestant config path exists
    if( cont ) {
        QFile file2 ( contestant );
        if( file2.exists() ) {
            QString temp("contestant");
            cont = askConfirmation(temp);

        }
    }

    //check if admin config path exists
    if( cont ) {
        QFile file3 ( admin );
        if( file3.exists() ) {
            QString temp("admin");
            cont = askConfirmation(temp);

        }
    }

    //check if presenter config path exists
    if( cont ) {
           QFile file4 ( presenter );
        if( file4.exists() ) {
            QString temp("presenter");
            cont = askConfirmation(temp);

        }
    }

    /*Creates the config structures for specific paths.
     *ClientConfig, PresenterConfig, AdminConfig.
     *StageDatas and ServerConfig
     *Finally saves in the end using xml_util methods
     */
    if( cont ) {
        ui->error->setText( "OK" );
        XmlUtil& xu = XmlUtil::getInstance();

        //Save everything...

        //ClientConfig *cc = new ClientConfig();
        PresenterConfig *pc = new PresenterConfig();
        ClientConfig *cc = new ClientConfig();
        AdminConfig *ac = new AdminConfig();

        cc->ip = ip;
        cc->port = port.toInt();
        pc->ip = ip;
        pc->port = port.toInt();
        ac->ip = ip;
        ac->port = port.toInt();



        StageData *s1 = new StageData();
        StageData *s2 = new StageData();
        StageData *s3 = new StageData();
        StageData *s4 = new StageData();

        s1->answer_file = r1_a;
        s1->question_file = r1_q;
        s2->answer_file = r2_a;
        s2->question_file = r2_q;
        s3->answer_file = r3_a;
        s3->question_file = r3_q;
        s4->answer_file = r4_a;
        s4->question_file = r4_q;

        ServerConfig *sc = new ServerConfig();
        sc->db_path = sql;
        sc->port = port.toInt();
        sc->stage_data.push_back(*s1);
        sc->stage_data.push_back(*s2);
        sc->stage_data.push_back(*s3);
        sc->stage_data.push_back(*s4);

        xu.writeNetConfig ( *pc, presenter );
        xu.writeClientConfig ( *cc, contestant );
        xu.writeNetConfig ( *ac, admin );
        xu.writeServerConfig ( *sc, server );
    }
    else {
        ui->error->setText( "Change the file paths to avoid overwriting" );
    }
    //ui->error->setText(*error);
}

/*Load IP, Port and question paths according to input paths*/
void ConfigEditor::load() {
    ui->error->setText( "" );
    XmlUtil& xu = XmlUtil::getInstance();
    bool check = false;

    if( ui->presenterConf->text() != "" ) {
        NetworkConfig *nc = new NetworkConfig();
        QString network(ui->presenterConf->text());
        QFile file( network );

        if( file.exists() ) {
            check = true;
            xu.readNetConfig ( network , *nc );
            ui->serverIP->setText( nc->ip );
            ui->serverPort1->setText( QString("%1").arg(nc->port) );
        }
    }

    else if( ui->adminConf->text() != "" ) {
        NetworkConfig *nc = new NetworkConfig();
        QString network(ui->presenterConf->text());
        QFile file (network);

        if( file.exists() ) {
            check = true;
            xu.readNetConfig( network, *nc );
            ui->serverIP->setText( nc->ip );
            ui->serverPort1->setText( QString("%1").arg(nc->port) );
        }
    }

    else if( ui->constConf->text() != "" ) {
        ClientConfig *nc = new ClientConfig();
        QString network(ui->presenterConf->text());
        QFile file (network);

        if( file.exists() ) {
            check = true;
            xu.readClientConfig( network, *nc );
            ui->serverIP->setText( nc->ip );
            ui->serverPort1->setText( QString("%1").arg(nc->port) );
            ui->serverPort2->setText( QString("%1").arg(nc->port) );
        }
    }

    if( ui->serverConf->text() != "" ) {
        ServerConfig *sc = new ServerConfig();
        QString network( ui->serverConf->text() );
        QFile file (network);

        if( file.exists() ) {
            check = true;
            xu.readServerConfig( network, *sc );
            ui->sql->setText( sc->db_path );
            ui->serverPort1->setText( QString("%1").arg(sc->port) );
            ui->serverPort2->setText( QString("%1").arg(sc->port) );

            StageData s1 = sc->stage_data.at(0);
            StageData s2 = sc->stage_data.at(1);
            StageData s3 = sc->stage_data.at(2);
            StageData s4 = sc->stage_data.at(3);

            ui->q_rnd1->setText(s1.question_file);
            ui->q_rnd2->setText(s2.question_file);
            ui->q_rnd3->setText(s3.question_file);
            ui->q_rnd4->setText(s4.question_file);

            ui->a_rnd1->setText(s1.answer_file);
            ui->a_rnd2->setText(s2.answer_file);
            ui->a_rnd3->setText(s3.answer_file);
            ui->a_rnd4->setText(s4.answer_file);
        }
    }

    if( check == false ) {

        ui->error->setText( "Please add at least one correct path to fill something" );
    }
}
