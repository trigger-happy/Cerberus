#include <QtGui/QApplication>
#include <iostream>
#include "client.h"
#include "ui_client.h"

using namespace std;

ClientDlg::ClientDlg(QWidget* parent): QDialog(parent), m_dlg(new Ui::client_dlg){
	m_dlg->setupUi(this);
	cout << "Constructor" << endl;
	connect(m_dlg->connect_btn, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(m_dlg->quit_btn, SIGNAL(clicked()), this, SLOT(onQuit()));
}

ClientDlg::~ClientDlg(){
	delete m_dlg;
}

void ClientDlg::onConnect(){
	cout << "Connect" << endl;
}

void ClientDlg::onQuit(){
	cout << "Quit" << endl;
}


int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	ClientDlg c_dlg;
	c_dlg.show();
	return app.exec();
}