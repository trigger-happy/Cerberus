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
#include "checker.h"
#include <iostream>
#include <string>
#include <QtGui>
#include "util/sql_util.h"
#include "util/xml_util.h"

using namespace std;
bool testing = true;

Server::Server ( QWidget* parent ) : QObject ( parent ) {
	
	//reads the server config from the XML file
	QFile file ( QString ( "resources/server_config.xml" ) );
	file.open ( QIODevice::ReadOnly );
	QString serverConfigXml = file.readAll();
	XmlUtil::getInstance().readServerConfig( serverConfigXml, m_config );
	m_port = m_config.port;
	QString m_db_path = m_config.db_path;
	
	//Fills up the m_questions vector with the question data in XML.
	for ( int i = 1; i <= 4; i++ ) {
		QFile file ( m_config.stage_files.at( i - 1 ) );
		file.open ( QIODevice::ReadOnly );
		QString fileString = file.readAll();
		StageData sd;
		XmlUtil::getInstance().readStageData( fileString, sd );
		vector<Question> questions = sd.questions;
		Checker* checker = new Checker();
		if( i < 3 ){
			int size = questions.size();
			for( int j=0; j<size; j++ ){
				checker->addQuestion(questions.at(j));
			}
		}
		else{
			if(i == 3) questions3 = questions;
			else questions4 = questions;
		}
		m_checkers.push_back ( checker );
		m_questions.push_back ( fileString );
	}
	selected_question_num = 0;

	//Instantiates the ServerNetwork class, networking stuff go here
	m_network = new ServerNetwork ( this );
	connect ( m_network, SIGNAL ( newContestant ( ContestantConnection* ) ),
			  this, SLOT ( newContestant ( ContestantConnection* ) ) );
	connect ( m_network, SIGNAL ( badClient ( TempConnection* ) ),
			  this, SLOT ( badClient ( TempConnection* ) ) );
	connect ( m_network, SIGNAL ( contestantDc ( ContestantConnection* ) ),
			  this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
	connect ( m_network, SIGNAL (newProjector(ProjectorConnection*)),
			  this, SLOT (projectorConnect(ProjectorConnection*)));
	connect ( m_network, SIGNAL (projectorDc(ProjectorConnection*)),
			  this, SLOT (projectorDisconnect(ProjectorConnection*)));
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
				 this, SLOT ( onAnswerSubmission(ContestantConnection*,int,AnswerData ) ) );
	if(!hash.contains(c_username)){
		hash[c_username] = cc;
		hash_answers[c_username] << "Not submitted.\n" << "Not submitted.\n" << "Not submitted.\n" << "Not submitted.\n";
	}
	emit contestantC( c_username );
}

void Server::contestantDisconnect( ContestantConnection* cc ) {
	QString c_user = cc->getUserName();
	if( testing ) cout << c_user.toStdString() << " has been disconnected.\n";
	if(!c_user.isNull()) emit contestantDc( c_user );
}

void Server::onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& data )
{
	AnswerData new_data = data;
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
	hash_answers[user].replace( round-1, allAnswers );
	Checker* checker = m_checkers.at(round-1);

	if(round>=3){
		vector<Question> questions;
		if(round == 3)
			questions = questions3;
		else
			questions = questions4;
		checker->resetQuestionSet();
		cout << "Checking at question number " << selected_question_num << endl;
		cout << "The question is " << questions.at(selected_question_num).question.toStdString() << endl;
		checker->addQuestion(questions.at(selected_question_num));
	}

	double points = checker->score( new_data );
	if ( testing ) cout << user.toStdString() << "'s got " << points << " points.\n";
	double new_score = this->getScore(user) + points;
	this->setScore( user, new_score );
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
	if ( testing ) cout << "Attempting to view answers by " << c_user.toStdString() << " at round " << round+1 << endl;
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
	m_network->showContestTime();
}

void Server::showRankings(){
	// TODO: change this
	// m_network->showContestRanks();
}

void Server::showQuestionTime(){
	m_network->showQuestionTime();
}

void Server::showQuestion(){
	m_network->showQuestion();
}

void Server::showAnswer(){
	m_network->showAnswer();
}

void Server::startQuestionTime(int num, int time){
	m_network->setQuestionState(num, time, QUESTION_RUNNING);
	selected_question_num = num;
}

void Server::stopQuestionTime(int num, int time){
	m_network->setQuestionState(num, time, QUESTION_STOPPED);
}

void Server::pauseQuestionTime(int num, int time){
	m_network->setQuestionState(num, time, QUESTION_PAUSED);
}

int Server::getRoundTime(int round){
	if(round > 0 && round < 3){
		StageData temp;
		XmlUtil::getInstance().readStageData(m_questions[round-1], temp);
		return temp.contest_time;
	}
	return -1;
}

int Server::getQuestionTime(int round, int question){
	if(round > 2){
		StageData temp;
		XmlUtil::getInstance().readStageData(m_questions[round-1], temp);
		return temp.questions[question].time_limit;
	}
	return -1;
}

