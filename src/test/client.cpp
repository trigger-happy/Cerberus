#include <QtGui/QApplication>
#include <iostream>
#include "client.h"
#include "ui_client.h"

using namespace std;

ClientDlg::ClientDlg(QWidget* parent): QDialog(parent), m_dlg(new Ui::client_dlg){
	m_dlg->setupUi(this);
	cout << "Constructor" << endl;
	connect(m_dlg->connect_btn, SIGNAL(clicked()), this, SLOT(onConnectBtn()));
	connect(m_dlg->quit_btn, SIGNAL(clicked()), this, SLOT(onQuitBtn()));
	
	m_socket = new QSslSocket();
	connect(m_socket, SIGNAL(connected()), this, SLOT(onConnect()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(onError(QAbstractSocket::SocketError)));
}

ClientDlg::~ClientDlg(){
	delete m_dlg;
}

void ClientDlg::onConnect(){
	cout << "Connected" << endl;
}

void ClientDlg::onConnectBtn(){
	cout << "Connect" << endl;
	m_socket->connectToHost(m_dlg->ip_ledit->text(), m_dlg->port_ledit->text().toInt());
}

void ClientDlg::onQuitBtn(){
	cout << "Quit" << endl;
	this->close();
}

void ClientDlg::onError(QAbstractSocket::SocketError error){
	cout << "Error" << endl;
}

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	ClientDlg c_dlg;
	c_dlg.show();
	return app.exec();
}
