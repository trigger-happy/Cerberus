/*
Copyright (C) 2009 James Choa
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
#ifndef SERVER_H
#define SERVER_H
#include <QtGui/QtGui>
#include <QtNetwork>
#include "net/server_net.h"
#include "checker.h"

class ServerNetwork;

class Server : public QObject {
	Q_OBJECT;

public:
	Server ( QWidget* parent = 0 );
	~Server();

	//Manual methods to be used by admin.
	/*!
	*/
	void stopContest();

	/*!
	*/
	void startContest();

	/*!
	*/
	void pauseContest();

	/*!
	*/
	QString viewSubmittedAnswers( QString c_user, int round );

	/*!
	*/
	void checkAnswersManually ();

	/*!
	*/
	void dropConnection( QString c_user );

	double getScore ( QString c_user );

	void setScore ( QString c_user, double score );

	void setRound( int round );

	void showTimeLeft();

	void showRankings();

	void showQuestionTime();

	void startQuestionTime( int num, int time );

	void stopQuestionTime( int num, int time );

	void pauseQuestionTime( int num, int time );

	void showQuestion();

	void showAnswer();

	vector<Question> questions3;

	vector<Question> questions4;

	int getRoundTime( int round );

	int getQuestionTime( int round, int question );

	void getRankData( vector<RankData>& out );

	void updateRankData();

	inline QStandardItemModel* getRankModel() {
		return m_rankmodel;
	}

signals:
	void badC ( TempConnection* con );
	void contestantC ( const QString& c_username );
	void contestantDc ( QString c_user );
	void newAdmin ( AdminConnection* ac );
	void newProjector ( ProjectorConnection* pc );
	void onContestTimeRequest( ushort& contime );

private slots:
	//Contestant slots
	/*!
	Called when there is a new connection
	*/
	void newContestant( ContestantConnection* cc );

	/*!
	Called when a client connects but is not a Cerberus client.
	  */
	void badClient ( TempConnection* tc );

	/*!
	Called when there is a disconnection
	*/
	void contestantDisconnect( ContestantConnection* cc );

	void onAuthentication( ContestantConnection* cc, const QString& c_username );

	void onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& data );

	//Projector slots
	void projectorConnect( ProjectorConnection* pc );

	void projectorDisconnect( ProjectorConnection* pc );

private:
	QString log, m_db_path;
	quint16 m_port;
	ServerNetwork* m_network;
	ServerConfig m_config;
	vector<QString> m_questions;
	vector<Checker*> m_checkers;
	//Keeps track of each user's ContestantConnection
	QHash<QString, ContestantConnection*> hash;
	//Keeps track of each user's answers per round
	QHash<QString, QStringList> hash_answers;
	//Keeps track of the current question number of each user for rounds 3/4
	QHash<QString, int> hash_questions;
	int selected_question_num;
	QStandardItemModel* m_rankmodel;
};

#endif //SERVER_H
