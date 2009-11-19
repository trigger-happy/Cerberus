/*
Copyright (C) 2009 Joshua Cheng
Copyright (C) 2009 James Choa

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
#include "net/projector_connection.h"
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

		if ( i < 3 ) {
			int size = questions.size();

			for ( int j = 0; j < size; j++ ) {
				checker->addQuestion( questions.at( j ) );
			}
		} else {
			if ( i == 3 ) questions3 = questions;
			else questions4 = questions;
		}

		m_checkers.push_back ( checker );

		m_questions.push_back ( fileString );
	}

	m_selected_question_num = 0;

	//Instantiates the ServerNetwork class, networking stuff go here
	m_network = new ServerNetwork ( this );
	connect ( m_network, SIGNAL ( newContestant ( ContestantConnection* ) ),
	          this, SLOT ( newContestant ( ContestantConnection* ) ) );
	connect ( m_network, SIGNAL ( badClient ( TempConnection* ) ),
	          this, SLOT ( badClient ( TempConnection* ) ) );
	connect ( m_network, SIGNAL ( contestantDc ( ContestantConnection* ) ),
	          this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
	connect ( m_network, SIGNAL ( newProjector( ProjectorConnection* ) ),
	          this, SLOT ( projectorConnect( ProjectorConnection* ) ) );
	connect ( m_network, SIGNAL ( projectorDc( ProjectorConnection* ) ),
	          this, SLOT ( projectorDisconnect( ProjectorConnection* ) ) );
	m_network->listen ( m_port );
	m_network->setRound ( 1 );
	m_network->setStatus ( CONTEST_STOPPED );
	m_network->setQData ( &m_questions );

	if ( testing ) cout << "Server has been established at port " << m_port << endl;

	//Opens the SQL Database with the specified db_path
	bool result;


	result = SqlUtil::getInstance().init ( m_db_path );

	//This will be deleted as soon as SqlUtil return types become void
	if ( !result ) {
		QMessageBox msg;
		msg.setText ( "Failed to load db" );
		msg.exec();
	}

	m_rankmodel = new QStandardItemModel( this );

	QStringList headers;
	headers.insert( 0, QString( "Rank" ) );
	headers.insert( 1, QString( "Name" ) );
	headers.insert( 2, QString( "Team" ) );
	headers.insert( 3, QString( "Score" ) );
	headers.insert( 4, QString( "Time" ) );
	m_rankmodel->setHorizontalHeaderLabels( headers );

	m_teammodel = new QStandardItemModel( this );
	headers.clear();
	headers.insert( 0, QString( "Rank" ) );
	headers.insert( 1, QString( "Team" ) );
	headers.insert( 2, QString( "Score" ) );
	headers.insert( 3, QString( "Time" ) );
	m_teammodel->setHorizontalHeaderLabels( headers );

	m_timeleft = 0;

	if ( testing ) cout << "Finished with setting up Server." << endl;
}

Server::~Server() {
	delete m_network;
}

//Contestant slots
void Server::newContestant( ContestantConnection* cc ) {
	if ( testing ) cout << "A new contestant has connected." << endl;

	connect ( cc, SIGNAL ( onAuthentication( ContestantConnection*, const QString& ) ),
	          this, SLOT ( onAuthentication( ContestantConnection*, const QString& ) ) );

	connect ( cc, SIGNAL( onContestTimeRequest( ushort& ) ),
	          this, SIGNAL( onContestTimeRequest( ushort& ) ) );
}

void Server::badClient ( TempConnection* tc ) {
	emit badC( tc );
}

void Server::onAuthentication( ContestantConnection* cc, const QString& c_username ) {
	if ( testing ) cout << c_username.toStdString() << " has authenticated." << endl;

	connect( cc, SIGNAL( onAnswerSubmission( ContestantConnection*, int, AnswerData ) ),
	         this, SLOT ( onAnswerSubmission( ContestantConnection*, int, AnswerData ) ) );

	if ( !m_cansubmit.contains( c_username ) ) {
		m_cansubmit[c_username] = true;
	} else {
		cc->enableAnswerSubmission( m_cansubmit[c_username] );
	}

	UserData ud;

	SqlUtil::getInstance().getSpecificUser( c_username, ud );

	if ( m_network->getRound() > 1 ) {
		if ( m_teamconnected.contains( ud.teamname ) ) {
			if ( m_teamconnected[ud.teamname] >= 1) {
				// this is a little work around to avoid a bug
				m_teamconnected[ud.teamname]++;
				emit contestantC( c_username );
				cc->dropClient();
				return;
			}
		}
	}

	if ( !m_hash.contains( c_username ) ) {
		// brand new connection
		m_hash_answers[c_username] << "" << "" << "" << "";

		// get information on the user and shove it into m_rankmodel
		QList<QStandardItem*> listing;
		// rank column
		listing.append( new QStandardItem( QString( "%1" ).arg( m_rankmodel->rowCount() + 1 ) ) );
		// full name
		listing.append( new QStandardItem( QString( "%1 %2" ).arg( ud.firstname ).arg( ud.lastname ) ) );
		// team name
		listing.append( new QStandardItem( ud.teamname ) );
		// score
		listing.append( new QStandardItem( QString( "0.0" ) ) );
		// time
		listing.append( new QStandardItem( QString( "0" ) ) );
		m_rankmodel->appendRow( listing );
		updateRankData();
	} else {
		// duplicate connection
		if ( m_hash[c_username] != NULL ) {
			m_hash[c_username]->dropClient();
		}
	}
	
	m_hash[c_username] = cc;
	if(m_teamconnected.contains(ud.teamname)){
		m_teamconnected[ud.teamname]++;
	}else{
		m_teamconnected[ud.teamname] = 1;
	}

	emit contestantC( c_username );
}

void Server::contestantDisconnect( ContestantConnection* cc ) {
	QString c_user = cc->getUserName();
	// remove the contestant from m_rankmodel.
	UserData ud;

	try {
		SqlUtil::getInstance().getSpecificUser( c_user, ud );
	} catch ( SqlUtil::SqlUtilException e ) {
		// unauthorized Contestant dced
		return;
	}

	m_teamconnected[ud.teamname]--;

	m_hash[c_user] = NULL;

	if ( testing ) cout << c_user.toStdString() << " has been disconnected.\n";

	if ( !c_user.isNull() ) emit contestantDc( c_user );

	/*
	if ( m_cansubmit[c_user] ) {
		// user has not submitted
		QString target = ud.firstname + " " + ud.lastname;
		QStandardItem* temp;

		for ( int i = 0; i < m_rankmodel->rowCount(); i++ ) {
			temp = m_rankmodel->item( i, 1 );

			if ( temp->text() == target ) {
				break;
			}
		}

		QList<QStandardItem*> listing = m_rankmodel->takeRow( temp->row() );

		for ( int i = 0; i < listing.size(); i++ ) {
			delete listing[i];
		}

		updateRankData();
	}
	*/
}

void Server::onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& data ) {
	AnswerData new_data = data;
	const QString& user = cc->getUserName();

	if ( !m_cansubmit[user] ) {
		return;
	}

	QString allAnswers, answer;

	cout << ( QString( "%1 has submitted answer data for round %2" ).arg( user ).arg( round ) ).toStdString() << endl;

	cc->enableAnswerSubmission( false );
	m_cansubmit[user] = false;

	for ( int i = 0; i < data.size(); i++ ) {
		int questionNum;
		if ( round >= 3 ) questionNum = m_selected_question_num;
		else questionNum = i + 1;
		if ( data[i].ans_type == Question::IDENTIFICATION ) {
			answer = QString( "%1 is %2\n" ).arg( questionNum ).arg( data[i].id_answer );
			cout << answer.toStdString();
			allAnswers.append( answer );
		} else {
			QString buffer;

			for ( int j = 0; j < data[i].multi_choice.size(); j++ ) {
				buffer.append( QString( "%1 " ).arg( data[i].multi_choice[j] ) );
			}

			answer = QString( "%1 is %2\n" ).arg( questionNum ).arg( buffer );

			cout << answer.toStdString();
			allAnswers.append( answer );
		}
	}

	if ( round >= 3 ) {
		QString tempAnswers = m_hash_answers[user].at( round - 1 );
		tempAnswers.append( allAnswers );
		m_hash_answers[user].replace( round - 1, tempAnswers );
	}
	else
		m_hash_answers[user].replace( round - 1, allAnswers );

	Checker* checker = m_checkers.at( round - 1 );

	if ( round >= 3 ) {
		vector<Question> questions;

		if ( round == 3 )
			questions = questions3;
		else
			questions = questions4;

		checker->resetQuestionSet();

		cout << "Checking at question number " << m_selected_question_num << endl;

		cout << "The question is " << questions.at( m_selected_question_num ).question.toStdString() << endl;

		checker->addQuestion( questions.at( m_selected_question_num ) );
	}

	double points = checker->score( new_data );

	if ( testing ) cout << user.toStdString() << "'s got " << points << " points.\n";

	double new_score = this->getScore( user ) + points;

	this->setScore( user, new_score );
}

//Presenter slots
void Server::projectorConnect( ProjectorConnection* pc ) {
	if ( testing ) cout << "Projector has connected.\n";

	connect( pc, SIGNAL( onContestTimeRequest( ushort& ) ), this, SIGNAL( onContestTimeRequest( ushort& ) ) );
}

void Server::projectorDisconnect( ProjectorConnection* pc ) {
	if ( testing ) cout << "Projector has been disconnected.\n";
}



//Admin functions
void Server::stopContest() {
	if ( testing ) cout << "Contest stopped. \n";

	m_network->setStatus( CONTEST_STOPPED );
}

void Server::startContest() {
	if ( testing ) cout << "Contest running. \n";

	m_network->setStatus( CONTEST_RUNNING );
}

void Server::pauseContest() {
	if ( testing ) cout << "Contest paused. \n";

	m_network->setStatus( CONTEST_PAUSED );
}

QString Server::viewSubmittedAnswers( QString c_user, int round ) {
	if ( testing ) cout << "Attempting to view answers by " << c_user.toStdString() << " at round " << round + 1 << endl;

	QString answers = m_hash_answers[c_user].at( round );

	return answers;
}

void Server::checkAnswersManually() {

}

void Server::dropConnection( QString c_user ) {
	ContestantConnection* cc = m_hash[c_user];
	cc->dropClient();
}

double Server::getScore( QString c_user ) {
	return SqlUtil::getInstance().getScore( c_user );
}

static const unsigned int SCORE_TIME_PRECISION = 100; //100ms precision

void Server::setScore( QString c_user, double score ) {
	SqlUtil::getInstance().setScore( c_user, score );
	// update m_rankmodel's score info
	UserData ud;
	SqlUtil::getInstance().getSpecificUser( c_user, ud );
	QString target = ud.firstname + " " + ud.lastname;
	QStandardItem* temp;

	for ( int i = 0; i < m_rankmodel->rowCount(); i++ ) {
		temp = m_rankmodel->item( i, 1 );

		if ( temp->text() == target ) {
			m_rankmodel->item( i, 3 )->setText( QString( "%1" ).arg( score ) );
			m_rankmodel->item( i, 4 )->setText( QString( "%1" ).arg(
					(getRoundTime( m_round ) * 1000 - //In seconds, convert to milliseconds
					 m_preciseTimeLeft +
					 SCORE_TIME_PRECISION/2) //for rounding off to the nearest SCORE_TIME_PRECISON
					/SCORE_TIME_PRECISION //truncate the rest of the score off
					) );
			updateRankData();
			break;
		}
	}

	if ( testing ) cout << c_user.toStdString() << "'s score has been set to " << score << endl;
}

void Server::setRound( int round ) {
	m_round = round;
	m_network->setRound( round );

	QMutableHashIterator<QString, bool> i( m_cansubmit );

	while ( i.hasNext() ) {
		i.next();
		i.setValue( true );
	}

	m_network->enableContestSubmission( true );

	if ( testing ) cout << "Contest set to Round " << round << ".\n";
}

void Server::showTimeLeft() {
	m_network->showContestTime();
}

void Server::showRankings() {
	vector<RankData> rd;
	getRankData( rd );
	m_network->showContestRanks( rd );
}

void Server::showQuestionTime() {
	m_network->showQuestionTime();
}

void Server::showAnswer() {
	m_network->showAnswer();
}

void Server::hideAnswer() {
	m_network->hideAnswer();
}

void Server::startQuestionTime( int num, int time ) {
	m_network->setQuestionState( num, time, QUESTION_RUNNING );
	m_selected_question_num = num;

	QMutableHashIterator<QString, bool> i( m_cansubmit );

	while ( i.hasNext() ) {
		i.next();
		i.setValue( true );
	}

	m_network->enableContestSubmission( true );
}

void Server::stopQuestionTime( int num, int time ) {
	m_network->setQuestionState( num, time, QUESTION_STOPPED );
}

void Server::pauseQuestionTime( int num, int time ) {
	m_network->setQuestionState( num, time, QUESTION_PAUSED );
}

int Server::getRoundTime( int round ) {
	if ( round > 0 && round < 3 ) {
		StageData temp;
		XmlUtil::getInstance().readStageData( m_questions[round-1], temp );
		return temp.contest_time;
	}

	return -1;
}

int Server::getQuestionTime( int round, int question ) {
	if ( round > 2 ) {
		StageData temp;
		XmlUtil::getInstance().readStageData( m_questions[round-1], temp );
		return temp.questions[question].time_limit;
	}

	return -1;
}

void Server::getRankData( vector<RankData>& out ) {
	// get stuff from the item model and then shove it into out.
	out.clear();

	for ( int i = 0; i < m_rankmodel->rowCount(); i++ ) {
		RankData temp;
		temp.rank = m_rankmodel->item( i, 0 )->text().toUShort();
		temp.fullname = m_rankmodel->item( i, 1 )->text();
		temp.teamname = m_rankmodel->item( i, 2 )->text();
		temp.score = m_rankmodel->item( i, 3 )->text().toDouble();
		temp.time = m_rankmodel->item( i, 4 )->text().toUInt();
		out.push_back( temp );
	}
}

void Server::updateRankData() {
	// for those curious why QStandardItemModel::sort is not used
	// just remember that it sorts it ALPHABETICALLY, not NUMERICALLY.
	vector<pair<RankData, QStandardItem*> > temp;

	for ( int i = 0; i < m_rankmodel->rowCount(); i++ ) {
		QStandardItem* ranking = m_rankmodel->item( i, 0 );
		RankData rd;
		rd.score = m_rankmodel->item( i, 3 )->text().toInt();
		rd.time = m_rankmodel->item( i, 4 )->text().toUInt();
		temp.push_back( pair<RankData, QStandardItem*>( rd, ranking ) );
	}

	sort( temp.begin(), temp.end() );

	QStandardItemModel* tempmodel = new QStandardItemModel( this );

	QStringList headers;
	headers.insert( 0, QString( "Rank" ) );
	headers.insert( 1, QString( "Name" ) );
	headers.insert( 2, QString( "Team" ) );
	headers.insert( 3, QString( "Score" ) );
	headers.insert( 4, QString( "Time" ) );
	tempmodel->setHorizontalHeaderLabels( headers );

	for ( int i = 0; i < temp.size(); i++ ) {
		int row = temp[i].second->row();
		QList<QStandardItem*> listing;
		// rank column
		listing.append( new QStandardItem( QString( "%1" ).arg( i + 1 ) ) );
		// full name
		listing.append( new QStandardItem( m_rankmodel->item( row, 1 )->text() ) );
		// team name
		listing.append( new QStandardItem( m_rankmodel->item( row, 2 )->text() ) );
		// score
		listing.append( new QStandardItem( m_rankmodel->item( row, 3 )->text() ) );
		// time
		listing.append( new QStandardItem( m_rankmodel->item( row, 4 )->text() ) );
		tempmodel->appendRow( listing );
	}

	emit newRankModel( tempmodel );

	delete m_rankmodel;
	m_rankmodel = tempmodel;
	filterTeamView();
}

void Server::filterTeamView() {
	vector<RankData> rd;
	this->getRankData( rd );
	map<QString, RankData> td;

	for ( int i = 0; i < rd.size(); i++ ) {
		if ( td.find( rd[i].teamname ) == td.end() ) {
			td[rd[i].teamname] = rd[i];
		} else {
			td[rd[i].teamname].score += rd[i].score;
			td[rd[i].teamname].time += rd[i].time;
		}
	}

	rd.clear();

	map<QString, RankData>::iterator iter = td.begin();

	while ( iter != td.end() ) {
		rd.push_back( iter->second );
		iter++;
	}

	sort( rd.begin(), rd.end() );

	// shove the team data into the view
	QStandardItemModel* tempmodel = new QStandardItemModel( this );

	QStringList headers;
	headers.insert( 0, QString( "Rank" ) );
	headers.insert( 1, QString( "Team" ) );
	headers.insert( 2, QString( "Score" ) );
	headers.insert( 3, QString( "Time" ) );
	tempmodel->setHorizontalHeaderLabels( headers );

	for ( int i = 0; i < rd.size(); i++ ) {
		QList<QStandardItem*> listing;
		// rank column
		listing.append( new QStandardItem( QString( "%1" ).arg( i + 1 ) ) );
		// team name
		listing.append( new QStandardItem( rd[i].teamname ) );
		// score
		listing.append( new QStandardItem( QString( "%1" ).arg( rd[i].score ) ) );
		// time
		listing.append( new QStandardItem( QString( "%1" ).arg( rd[i].time ) ) );
		tempmodel->appendRow( listing );
	}

	emit newTeamModel( tempmodel );

	delete m_teammodel;
	m_teammodel = tempmodel;
}

void Server::setContestTime( ushort time ) {
	m_network->setContestTime( time );
	m_timeleft = time;
}

void Server::scoreReset() {
	SqlUtil::getInstance().scoreReset();

	for ( int i = 0; i < m_rankmodel->rowCount(); i++ ) {
		m_rankmodel->item( i, 3 )->setText( QString( "0" ) );
		m_rankmodel->item( i, 4 )->setText( QString( "0" ) );
	}
}

void Server::showMainScreen() {
	m_network->showMainScreen();
}

void Server::timerTick() {
	m_timeleft--;
}

void Server::onPreciseTimerTick(unsigned int msec) {
	m_preciseTimeLeft = msec;
}
