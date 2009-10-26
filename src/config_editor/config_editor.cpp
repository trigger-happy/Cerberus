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
    connect( ui->save, SIGNAL(pressed()), this, SLOT(save()) );
    connect( ui->serverPort1, SIGNAL( textChanged(QString)), this, SLOT(Port1Changed()) );
    connect( ui->serverPort2, SIGNAL( textChanged(QString)), this, SLOT(Port2Changed()) );
    //connect(ui->line1, SIGNAL(textChanged(QString)), this, SLOT(TextChange()) );
    //connect(ui->line2, SIGNAL(textChanged(QString)), this, SLOT(TextChange()) );
}

//void ConfigEditor::TextChange() {
    //ui->label->setText( ui->line1->text() + " " + ui->line2->text());
//}

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

bool ConfigEditor::showInfo() {
    QMessageBox msgBox;
     msgBox.setText("Some Files Doesn't Exist.");

     msgBox.setStandardButtons(QMessageBox::Ok);
     msgBox.setDefaultButton(QMessageBox::Ok);
     int ret = msgBox.exec();

}
void ConfigEditor::save() {
    //QString *error = new QString("");
    //bool isEmpty = true;
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

    QFile file ( server );
    if( file.exists() ) {
        QString temp("server");
        cont = askConfirmation(temp);

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
        //xu.writeClientConfig ( *cc, contestant );
        xu.writeNetConfig ( *ac, admin );
        xu.writeServerConfig ( *sc, server );
    }
    else {
        ui->error->setText( "Change the file paths to avoid overwriting" );
    }
    //ui->error->setText(*error);
}
void ConfigEditor::load() {

}
