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
#include "server.h"
#include "ui_server.h"
#include "net/server_net.h"
#include "net/contestant_connection.h"
#include <iostream>
#include <string>
#include <QtGui>
#include "util/sql_util.h"
#include "util/xml_util.h"

using namespace std;

Server::Server ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::server_dlg )
{
		/*Fills up the m_questions and m_answers vector with the
		question data and answer data respectively.
		*/
		for ( int i = 1; i <= 4; i++ ) {
				QFile file1 ( QString ( "resources/stage%1_q.xml" ).arg ( i ) );
				QFile file2 ( QString ( "resources/stage%1_a.xml" ).arg ( i ) );
				file1.open ( QIODevice::ReadOnly );
				file2.open ( QIODevice::ReadOnly );
				m_questions.push_back ( file1.readAll() );
				m_answers.push_back ( file2.readAll() );
		}

		//reads the server config from the XML file
		QFile file ( QString ( "resources/server_config.xml" ));
		file.open ( QIODevice::ReadOnly );
		QString serverConfigXml = file.readAll();
		XmlUtil::getInstance().readServerConfig(serverConfigXml,m_config);
		int port = m_config.port;
		QString db_path = m_config.db_path;

		//Opens the SQL Database with the db_path
		bool result = SqlUtil::getInstance().init ( db_path );
		if ( !result ) {
				QMessageBox msg ( this );
				msg.setText ( "Failed to load db" );
				msg.exec();
		}

		//Instantiates the ServerNetwork class, networking stuff go here
		m_network = new ServerNetwork ( this );
		connect ( m_network, SIGNAL ( newContestant ( ContestantConnection* ) ), this, SLOT ( newContestant ( ContestantConnection* ) ) );
		connect ( m_network, SIGNAL ( badClient ( TempConnection* ) ), this, SLOT ( badClient ( TempConnection* ) ) );
		connect ( m_network, SIGNAL ( contestantDc ( ContestantConnection* ) ), this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
		
		m_network->listen ( port );
		m_network->setRound ( 1 );
		m_network->setStatus ( CONTEST_STOPPED );
		m_network->setQData ( &m_questions );
}

Server::~Server()
{
        delete m_network;
}

void Server::newContestant( ContestantConnection* cc )
{
		//TODO: Stuff here for when a new connection is made
		cout << "Connected." << endl; 
		connect ( cc, SIGNAL (onAuthentication( ContestantConnection*, const QString&)), this, SLOT (onAuthentication(ContestantConnection*, const QString&)));

}

void Server::badClient ( TempConnection* tc )
{
		//m_dlg->textBrowser->setText("Bad client.");
}

void Server::contestantDisconnect( ContestantConnection* cc )
{
		//m_dlg->textBrowser->append("Contestant disconnected.");

}

void Server::onAuthentication(ContestantConnection* cc, const QString& c_username)
{
		//m_dlg->textBrowser->append("Contestant has logged in.");
		cout << c_username.toStdString() << " has connected." << endl;
}

void Server::stopContest()
{
		//m_dlg->textBrowser->append("Stopped.");
}

void Server::startContest()
{

}

void Server::pauseContest()
{

}

void Server::viewSubmittedAnswers()
{

}

void Server::checkAnswersManually()
{

}

void Server::dropConnection( ContestantConnection* cc )
{

}

int main ( int argc, char* argv[] )
{
		cout << "hello" << endl;
		QApplication a ( argc, argv );
		return a.exec();
}
