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
	//Fills up the m_questions vector with the question data in XML.
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

	//This will be deleted as soon as SqlUtil return types become void
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

//Contestant slots
void Server::newContestant( ContestantConnection* cc ) {
	if( testing ) cout << "A new contestant has connected." << endl;
	connect ( cc, SIGNAL ( onAuthentication( ContestantConnection*, const QString& ) ),
			  this, SLOT ( onAuthentication( ContestantConnection*, const QString& ) ) );
}

void Server::badClient ( TempConnection* tc ) {
	emit badC( tc );
}

void Server::onAuthentication( ContestantConnection* cc, const QString& c_username ) {
	if ( testing ) cout << c_username.toStdString() << " has authenticated." << endl;
	connect( cc, SIGNAL( onAnswerSubmission( ContestantConnection*, int, AnswerData ) ),
			 this, SLOT( onAnswerSubmission( ContestantConnection*, int, AnswerData ) ) );
	m_network->getContestantList();
	hash[c_username] = cc;
	emit contestantC( c_username );
}

void Server::contestantDisconnect( ContestantConnection* cc ) {
	QString c_user = cc->getUserName();
	if( testing ) cout << c_user.toStdString() << " has been disconnected.\n";
	if(!c_user.isNull()) emit contestantDc( c_user );
}

void Server::onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& data )
{
	//return here should be a vector of QStrings to be submitted to admin. Or something.
	const QString& user = cc->getUserName();
	QString allAnswers, answer;
	cout << ( QString( "%1 has submitted answer data for round %2" ).arg( user ).arg( round ) ).toStdString() << endl;

	for ( int i = 0; i < data.size(); i++ ) {
		if ( data[i].ans_type == Question::IDENTIFICATION ) {
			answer = QString( "%1 is %2\n" ).arg( i + 1 ).arg( data[i].id_answer );
			cout << answer.toStdString();
			allAnswers.append( answer );
		} else {
			QString buffer;

			for ( int j = 0; j < data[i].multi_choice.size(); j++ ) {
				buffer.append( QString( "%1 " ).arg( data[i].multi_choice[j] ) );
			}
			answer = QString( "%1 is %2\n" ).arg( i + 1 ).arg( buffer );
			cout << answer.toStdString();
			allAnswers.append( answer );
		}
	}
	hash_answers[user].insert( round, allAnswers );

}

//Presenter slots
void Server::projectorConnect( ProjectorConnection* pc){
	if( testing ) cout << "Projector has connected.\n";
}

void Server::projectorDisconnect( ProjectorConnection* pc){
	if( testing ) cout << "Projector has been disconnected.\n";
}



//Admin functions
void Server::stopContest() {
	if( testing ) cout << "Contest stopped. \n";
	m_network->setStatus( CONTEST_STOPPED );
}

void Server::startContest() {
	if( testing ) cout << "Contest running. \n";
	m_network->setStatus( CONTEST_RUNNING );
}

void Server::pauseContest() {
	if( testing ) cout << "Contest paused. \n";
	m_network->setStatus( CONTEST_PAUSED );
}

QString Server::viewSubmittedAnswers( QString c_user, int round ) {
	QString answers = hash_answers[c_user].at( round );
	return answers;
}

void Server::checkAnswersManually() {

}

void Server::dropConnection( QString c_user ) {
	ContestantConnection* cc = hash[c_user];
	cc->dropClient();
}

double Server::getScore( QString c_user ){
	return SqlUtil::getInstance().getScore( c_user );
}

void Server::setScore( QString c_user, double score ){
	SqlUtil::getInstance().setScore( c_user, score );
	if( testing ) cout << c_user.toStdString() << "'s score has been set to " << score << endl;
}

void Server::setRound( int round ){
	m_network->setRound(round);
	if( testing ) cout << "Contest set to Round " << round << ".\n";
}

void Server::showTimeLeft(){
}

void Server::showRankings(){
	// TODO: change this
	// m_network->showContestRanks();
}

void Server::showQuestionTime(){
	m_network->showQuestionTime();
}


