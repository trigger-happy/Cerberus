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
#include "util/xml_util.h"
#include "util/sql_util.h"

using namespace std;

ServerDlg::ServerDlg ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::server_dlg() )
{
        m_dlg->setupUi ( this );
        connect ( m_dlg->quit_btn, SIGNAL ( clicked() ), this, SLOT ( onQuitBtn() ) );
        m_server = new ServerNetwork ( this );
        connect ( m_server, SIGNAL ( onNewConnection() ), this, SLOT ( onConnection() ) );
        QFile f("resources/stage1_q.xml");
	f.open(QIODevice::ReadOnly);
	QString xml = f.readAll();
        m_server->setR1QData ( xml );
        m_server->listen ( 2652 );
	bool result = SqlUtil::getInstance().init("resources/server.db");
	if(!result){
		QMessageBox msg(this);
		msg.setText("Failed to load db");
		msg.exec();
	}
}

ServerDlg::~ServerDlg()
{
        delete m_dlg;
}

void ServerDlg::onConnection()
{
        cout << "Connection from client" << endl;
}

void ServerDlg::onQuitBtn()
{
        this->close();
}

int main ( int argc, char* argv[] )
{
        QApplication app ( argc, argv );
        ServerDlg dlg;
        dlg.show();
        return app.exec();
}
