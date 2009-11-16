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
	Stop the current contest.
	*/
	void stopContest();

	/*!
	Start the contest with the current parameters.
	*/
	void startContest();

	/*!
	Pause the contest.
	*/
	void pauseContest();

	/*!
	View the submitted answers for a specific user in a specific round.
	\TODO: optimize this
	\param c_user The username
	\param round The round
	\return A QString containing the answer
	*/
	QString viewSubmittedAnswers( QString c_user, int round );

	/*!
	\TODO: remove this, manual answer checking will really be done manually.
	*/
	void checkAnswersManually ();

	/*!
	Drop the connection of a certain user.
	\param c_user User name of the connection to drop.
	*/
	void dropConnection( QString c_user );

	/*!
	Ge the score for a specific user.
	\param c_user The user name.
	\return The score in double.
	*/
	double getScore ( QString c_user );

	/*!
	Set the score for a certain user.
	\param c_user The username.
	\param score The new score.
	*/
	void setScore ( QString c_user, double score );

	/*!
	Set the current contest round to the new round.
	\param round The contest round between 1 to 4.
	*/
	void setRound( int round );

	/*!
	Send a command to projectors to show the remaining time.
	*/
	void showTimeLeft();

	/*!
	Send a command to projectors to show the current rankings.
	*/
	void showRankings();

	/*!
	Send a command to projectors to show the question time.
	*/
	void showQuestionTime();

	/*!
	Begin the question timer for num question.
	\param num The question number (0-based index).
	\param time The time in seconds.
	*/
	void startQuestionTime( int num, int time );

	/*!
	Stop the question timer for num question.
	\param num The question number.
	\param time The time in seconds.
	*/
	void stopQuestionTime( int num, int time );

	/*!
	Pause the question timer for num question.
	\param num The question number.
	\param time The time in seconds.
	*/
	void pauseQuestionTime( int num, int time );

	/*!
	Send a command to the projector to show the current question on screen.
	\TODO: determine if this should be removed/merged with showQuestionTime
	*/
	void showQuestion();

	/*!
	Send a command to the projector to show the answer to the current question.
	*/
	void showAnswer();

	/*!
	Send a command to the projector to hide the answer to the current question.
	*/
	void hideAnswer();

	/*!
	Get the contest time for a certain round.
	\param round The round to query.
	\return The contest time in seconds.
	*/
	int getRoundTime( int round );

	/*!
	Get the question time for a certain question.
	\param round The round to query [3-4].
	\param question The question to query (0-based index).
	\return The question time in seconds.
	*/
	int getQuestionTime( int round, int question );

	/*!
	Get the ranking data for the current round.
	\param out An std::vector of RankData to contain the data.
	*/
	void getRankData( vector<RankData>& out );

	/*!
	Update the rank data, used internally for enforcing consistency
	when scores changed. Other classes should only call this when
	absolutely necessary.
	*/
	void updateRankData();

	/*!
	Get the ranking model
	\return Pointer to QStandardItemModel containing the rank data.
	*/
	inline QStandardItemModel* getRankModel() {
		return m_rankmodel;
	}

	/*!
	Get the team model
	*/
	inline QStandardItemModel* getTeamModel() {
		return m_teammodel;
	}

	/*!
	Set the contest time for the projectors.
	\param time The contest time.
	*/
	void setContestTime ( ushort time );

	/*!
	Reset the scores
	*/
	void scoreReset();

	/*!
	Show the main screen on the projector.
	*/
	void showMainScreen();

	// public stuff
	vector<Question> questions3;
	vector<Question> questions4;

signals:
	/*!
	Signal emitted when there's a connection from an invalid client.
	*/
	void badC ( TempConnection* con );

	/*!
	Emitted when a contestant is authenticated.
	\param c_username The username of the contestant.
	*/
	void contestantC ( const QString& c_username );

	/*!
	Emitted when a contestant disconnects.
	\param c_user The username of the contestant that disconnected.
	*/
	void contestantDc ( QString c_user );

	/*!
	Emitted when a projector connects to the server.
	\param pc A pointer to the ProjectorConnection.
	*/
	void newProjector ( ProjectorConnection* pc );

	/*!
	Emitted when a connection requests for the contest time.
	\param contime A reference to a ushort to set the contest time.
	*/
	void onContestTimeRequest( ushort& contime );

	/*!
	Emitted when the model has been renewed. This is a work
	around for the sorting fail for score view.
	\param model The new QStandardItemModel
	*/
	void newRankModel( QStandardItemModel* model );

	/*!
	Emitted when the model has been renewed. Same work around
	as above.
	\param model The new QStandardItemModel
	*/
	void newTeamModel( QStandardItemModel* model );

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

	/*!
	Slot for managing authentication requests.
	\param cc The ContestantConnection that's authenticating.
	\param c_username The username passed by the contestant.
	*/
	void onAuthentication( ContestantConnection* cc, const QString& c_username );

	/*!
	Slot for managing answer submissions.
	\param cc The ContestantConnection where the answer data is coming from.
	\param round The round sent by the contestant.
	\param data A const reference to AnswerData containing the answers.
	*/
	void onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& data );

	//Projector slots
	/*!
	Slot for handling new projector connections.
	\param pc Pointer to the new ProjectorConnection.
	*/
	void projectorConnect( ProjectorConnection* pc );

	/*!
	Slot for handling disconnections by projectors.
	\param pc Pointer to the ProjectorConnection that disconnected.
	*/
	void projectorDisconnect( ProjectorConnection* pc );

	/*!
	Slot for the contest count down timer.
	*/
	void timerTick();

	void onPreciseTimerTick(unsigned int msec);
private:
	/*!
	Filter the score view by teams instead. Scores of users from the
	same team will be added together into a single entity.
	*/
	void filterTeamView();

	QString m_log, m_db_path;
	quint16 m_port;
	int m_round;
	ServerNetwork* m_network;
	ServerConfig m_config;
	vector<QString> m_questions;
	vector<Checker*> m_checkers;
	//Keeps track of each user's ContestantConnection
	QHash<QString, ContestantConnection*> m_hash;
	//Keeps track of each user's answers per round
	QHash<QString, QStringList> m_hash_answers;
	//Keeps track of the current question number of each user for rounds 3/4
	QHash<QString, int> m_hash_questions;
	//a switch that determines if the user can still submit or not
	QHash<QString, bool> m_cansubmit;
	// keep track of team connections.
	QHash<QString, bool> m_teamconnected;
	int m_selected_question_num;
	QStandardItemModel* m_rankmodel;
	QStandardItemModel* m_teammodel;
	// internal timer
	ushort m_timeleft;
	//A more precise time left.
	unsigned int m_preciseTimeLeft;
};

#endif //SERVER_H
