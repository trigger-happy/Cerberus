
#include <QtGui>
#include <QtGui/QApplication>
#include "deployer.h"
#include "ui_deployer.h"
#include "data_types.h"
#include <iostream>

Installer::Installer(QWidget *parent) : QDialog(parent), ui(new Ui::deployerUI)
{
    ui->setupUi(this);
    connect( ui->install, SIGNAL( pressed()), this, SLOT(install()) );
    ui->installList->insertItem( 0, QString("server" ),  QVariant() );

}

Installer::~Installer() {

    delete ui;
}

void Installer :: install() {

}



