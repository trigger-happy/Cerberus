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
bool testing = true;

Server::Server ( QWidget* parent ) : QObject ( parent ) {
	/*Fills up the m_questions vector with the question data in XML.
	*/
	for ( int i = 1; i <= 4; i++ ) {
		QFile file ( QString ( "resources/stage%1.xml" ).arg ( i ) );
		file.open ( QIODevice::ReadOnly );
		m_questions.push_back ( file.readAll() );
	}

	//reads the server config from the XML file
	QFile file ( QString ( "resources/server_config.xml" ) );
	file.open ( QIODevice::ReadOnly );
	QString serverConfigXml = file.readAll();
	XmlUtil::getInstance().readServerConfig( serverConfigXml, m_config );
	m_port = m_config.port;
	QString m_db_path = m_config.db_path;

	//Instantiates the ServerNetwork class, networking stuff go here
	m_network = new ServerNetwork ( this );
	connect ( m_network, SIGNAL ( newContestant ( ContestantConnection* ) ),
			  this, SLOT ( newContestant ( ContestantConnection* ) ) );
	connect ( m_network, SIGNAL ( badClient ( TempConnection* ) ),
			  this, SLOT ( badClient ( TempConnection* ) ) );
	connect ( m_network, SIGNAL ( contestantDc ( ContestantConnection* ) ),
			  this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
	m_network->listen ( m_port );
	m_network->setRound ( 1 );
	m_network->setStatus ( CONTEST_STOPPED );
	m_network->setQData ( &m_questions );
	if ( testing ) cout << "Server has been established at port " << m_port << endl;

	//Opens the SQL Database with the specified db_path
	bool result;
	if ( testing ) result = SqlUtil::getInstance().init ( "resources/test.db" );
	else result = SqlUtil::getInstance().init ( m_db_path );

	if ( !result ) {
		QMessageBox msg;
		msg.setText ( "Failed to load db" );
		msg.exec();
	}

	if ( testing ) cout << "Finished with setting up Server." << endl;
}

Server::~Server() {
	delete m_network;
}

void Server::newContestant( ContestantConnection* cc ) {
	//TODO: Stuff here for when a new connection is made
	if( testing ) cout << "A new contestant has connected." << endl;
	connect ( cc, SIGNAL ( onAuthentication( ContestantConnection*, const QString& ) ),
			  this, SLOT ( onAuthentication( ContestantConnection*, const QString& ) ) );
}

void Server::badClient ( TempConnection* tc ) {
	emit badC( tc );
}

void Server::contestantDisconnect( ContestantConnection* cc ) {
	if( testing ) cout << "A contestant has been disconnected." << endl;
	emit contestantDc( cc );
}

void Server::onAuthentication( ContestantConnection* cc, const QString& c_username ) {
	if ( testing ) cout << c_username.toStdString() << " has connected." << endl;
	connect( cc, SIGNAL( onAnswerSubmission( ContestantConnection*, int, AnswerData ) ),
			 this, SLOT( onAnswerSubmission( ContestantConnection*, int, AnswerData ) ) );
	emit contestantC( cc, c_username );
}

void Server::onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& data )
{

}

void Server::stopContest() {
	//m_dlg->textBrowser->append("Stopped.");
	m_network->setStatus( CONTEST_STOPPED );
}

void Server::startContest() {
	m_network->setStatus( CONTEST_RUNNING );
}

void Server::pauseContest() {
	m_network->setStatus( CONTEST_PAUSED );
}

void Server::viewSubmittedAnswers() {

}

void Server::checkAnswersManually() {

}

void Server::dropConnection( ContestantConnection* cc ) {

}
