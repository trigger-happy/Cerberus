/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include <QApplication>
#include <iostream>
#include "ui_server.h"
#include "server.h"

using namespace std;

ServerDlg::ServerDlg(QWidget* parent): QDialog(parent), m_dlg(new Ui::server_dlg())
{
	m_dlg->setupUi(this);
	cout << "Constructor" << endl;
	connect(m_dlg->quit_btn, SIGNAL(clicked()), this, SLOT(onQuitBtn()));
	m_server = new QTcpServer(this);
	connect(m_server, SIGNAL(newConnection()), this, SLOT(onConnection()));
	m_server->listen(QHostAddress::Any, 3129);
}

ServerDlg::~ServerDlg()
{
	delete m_dlg;
}

void ServerDlg::onConnection()
{
	log += QString("Client Connected\n");
	m_dlg->log_tedt->setText(log);
	QTcpSocket* s = m_server->nextPendingConnection();
	log += QString("IP: ") + s->localAddress().toString();
	m_dlg->log_tedt->setText(log);
}

void ServerDlg::onQuitBtn()
{
	this->close();
}

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	ServerDlg dlg;
	dlg.show();
	return app.exec();
}