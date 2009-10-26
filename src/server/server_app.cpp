/*
Copyright (C) 2009 Joshua Cheng

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
#include "server_app.h"
#include "ui_server_app.h"
#include "net/server_net.h"
#include <iostream>
#include <string>
#include "util/sql_util.h"
#include "util/xml_util.h"

using namespace std;

ServerApp::ServerApp ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::server_dlg )
{
		//Fills up the m_questions vector with the question data for each stage.
		for ( int i = 1; i <= 4; i++ ) {
				QFile file1 ( QString ( "resources/stage%1_q.xml" ).arg ( i ) );
				QFile file2 ( QString ( "resources/stage%1_a.xml" ).arg ( i ) );
				file1.open ( QIODevice::ReadOnly );
				file2.open ( QIODevice::ReadOnly );
				m_questions.push_back ( file1.readAll() );
				m_answers.push_back ( file2.readAll() );
		}

		QFile file ( QString ( "resources/server_config.xml" ));
		file.open ( QIODevice::ReadOnly );
		QString serverConfigXML = file.readAll();
		XmlUtil::getInstance().readServerConfig(serverConfigXML,m_config);

		m_dlg->setupUi ( this );
		connect(m_dlg->stopButton,SIGNAL(clicked()),this,SLOT(stop()));

		m_network = new ServerNetwork ( this );
		connect ( m_network, SIGNAL ( newContestant ( ContestantConnection* ) ), this, SLOT ( newContestant ( ContestantConnection* ) ) );
		connect ( m_network, SIGNAL ( badClient ( TempConnection* ) ), this, SLOT ( badClient ( TempConnection* ) ) );
		connect ( m_network, SIGNAL ( contestantDc ( ContestantConnection* ) ), this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
		m_network->listen ( 2652 );
		m_network->setRound ( 1 );
		m_network->setStatus ( CONTEST_STOPPED );
		m_network->setQData ( &m_questions );
		bool result = SqlUtil::getInstance().init ( "resources/test.db" );
		if ( !result ) {
				QMessageBox msg ( this );
				msg.setText ( "Failed to load db" );
				msg.exec();
		}
		num=0;
}

ServerApp::~ServerApp()
{
        delete m_network;
}

void ServerApp::newContestant( ContestantConnection* cc )
{
		//TODO: Stuff here for when a new connection is made
		m_dlg->textBrowser->append("New Contestant connection.");
}

void ServerApp::badClient ( TempConnection* tc )
{
		m_dlg->textBrowser->setText("Bad client.");
}

void ServerApp::contestantDisconnect( ContestantConnection* cc )
{
		m_dlg->textBrowser->append("Contestant disconnected.");
}

void ServerApp::stop()
{
		m_dlg->textBrowser->append("Stopped.");
		m_dlg->textBrowser->append(m_questions.at(num));
		num++;
}

int main ( int argc, char* argv[] )
{
		/*int x = 2 + 3;
		cout << x << endl;*/
        QApplication a ( argc, argv );
        ServerApp server;
		server.show();
		return a.exec();
}
