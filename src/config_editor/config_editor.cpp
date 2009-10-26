#include <QFile>
#include <QString>
#include "config_editor.h"
#include "ui_server_editor.h"
#include <iostream>
using namespace std;

ConfigEditor::ConfigEditor(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::config_editor)
{
    ui->setupUi(this);
    connect( ui->pushButton, SIGNAL(pressed()), this, SLOT(ButtonPressed()) );
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

void ConfigEditor::ButtonPressed() {
    QString *error = new QString("");
    bool isEmpty = true;
    QString port(ui->serverPort->text());
    QString ip( ui->serverIP->text());
    QString server( ui->serverConf->text() );
    QString contestant ( ui->constConf->text() );
    QString admin ( ui->adminConf->text() );
    QString presenter ( ui->presenterConf->text() );

    cout << port.toStdString() << endl;
    cout << ip.toStdString() << endl;
    cout << server.toStdString() << endl;
    cout << contestant.toStdString() << endl;
    cout << admin.toStdString() << endl;
    cout << presenter.toStdString() << endl;

    QFile file ( server );
    if( !file.exists() ) {
        isEmpty = false;
        error->append( "server file" );
    }
    QFile file2 ( contestant );
    if( !file2.exists() ) {
        isEmpty = false;
        error->append(":: contestant file") ;
    }
    QFile file3 ( admin );
    if( !file3.exists() ) {
        isEmpty = false;
        error->append( ":: admin file" );
    }
    QFile file4 ( presenter );
    if( !file4.exists() ) {
        isEmpty = false;
        error->append(":: presenter file");
    }

    if( isEmpty == true ) error->append( " ERROR" );
    ui->error->setText(*error);
}
