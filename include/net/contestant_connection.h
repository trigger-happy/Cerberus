/*
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
#ifndef CONTESTANT_CONNECTION_H
#define CONTESTANT_CONNECTION_H
#include <QtNetwork>
#include <vector>
#include "protocol.h"
#include "error_defs.h"
#include "data_types.h"
#include "server_net.h"

using std::vector;

/*!
\brief A class representing a connection with a contestant's client.
This class is created for each client connection that is made to the server.
*/

class ContestantConnection : public QObject {
	Q_OBJECT;

public:
	/*!
	Constructor.
	\param parent Set the parent of this class.
	*/
	ContestantConnection ( ServerNetwork* sn = 0, QTcpSocket* socket = 0 );

	/*!
	Set the current round of the contest.
	\param r The round number [1-4]
	*/
	void setRound ( int r );

	/*!
	Disconnect the client for this connection.
	*/
	void dropClient();

	/*!
	Get the username associated with this contestant connection
	\return const reference to a QString containing the username.
	*/
	inline const QString& getUserName() const {
		return m_username;
	}

	/*!
	Set the current status of the contest.
	\param s CONTEST_STATUS
	*/
	void setStatus ( CONTEST_STATUS s );

	/*!
	*/
	inline void setContestTime( ushort time ) {
		m_contime = time;
	}

	/*!
	Set the contest time. Note that this will immediately send the information
	to the client.
	*/
	void sendContestTime ();

	/*!
	Set the question state for round 3/4 (for contest pacing).
	\param qnum The question number.
	\param time The time left for this question.
	\param state The state of this question.
	*/
	void setQuestionState ( ushort qnum, ushort time, QUESTION_STATUS state );

	/*!
	Set the pointer to the question data. This is to be used by ServerNet.
	\param qdata pointer to a vector of QString containing the xml data.
	*/
	inline void setQData ( const vector<QString>* qdata ) {
		m_qdata = qdata;
	}

	/*!
	Enable/disable this client in submitting answers.
	\param b true if this client may submit, false otherwise.
	*/
	inline void enableAnswerSubmission( bool b ) {
		m_answer_capable = b;
	}

	/*!
	Check if this contestant can still submit.
	\return true if it can, false otherwise.
	*/
	inline bool canSubmitAnswers() {
		return m_answer_capable;
	}

public slots:
	/*!
	Called when there's a socket error.
	\param err The error
	*/
	void error ( const QAbstractSocket::SocketError& err );

	/*!
	Called when there's data ready in the socket for reading.
	*/
	void ready();

	/*!
	Called when the connection is disconnected by the client.
	*/
	void disconnected();

	/*!
	Send a command to change the contest state.
	*/
	void sendContestState ();

signals:
	/*!
	Emitted when a contestant disconnects
	\param cc Pointer to a ContestantConnection with the contestant that disconnected.
	*/
	void contestantDisconnect ( ContestantConnection* cc );

	/*!
	Emitted when the contestant requests for the contest time.
	\param contime Reference to a ushort that will contain the contest time.
	*/
	void onContestTimeRequest( ushort& contime );

	/*!
	Emitted when the authentication is successful for this client.
	\param cc Pointer to the ContesantConnection that made the request.
	\param username const reference to a QString containing the username.
	*/
	void onAuthentication( ContestantConnection* cc, const QString& username );

	/*!
	Emitted when the client has sent its answer data.
	\param cc Pointer to the ContesantConnection that made the request.
	\param round The round that the answer data is suppose to come from.
	\param ans The answer data.
	*/
	void onAnswerSubmission( ContestantConnection* cc, int round, const AnswerData& ans );

	/*!
	Emitted when a socket error occurs
	\param error A const reference to a QString containing the error message
	*/
	void onError( const QString& error );

private:
	//some private functions
	/*!
	Send a reply to the authentication request.
	\param res The result of the authentication.
	*/
	void authenticationReply ( bool res );

	/*!
	Send a reply that an error has occurred.
	\param err The error message.
	*/
	void errorReply ( ERROR_MESSAGES err );

	/*!
	Send the question data
	\param xml QString containing the xml data.
	*/
	void sendQData ( const QString& xml );

	/*!
	Send a reply to the answer submission.
	\param res The result of the submission (not very relevant).
	*/
	void sendAReply ( bool res );

	//private fields
	ushort m_qnum;
	ushort m_qtime;
	QUESTION_STATUS m_qstatus;
	bool m_answer_capable;
	QTcpSocket* m_socket;
	QString m_username;
	p_header* m_hdr;
	bool m_authenticated;
	CONTEST_STATUS m_con_status;
	int m_round;
	ushort m_contime;
	const vector<QString>* m_qdata;
	ServerNetwork* m_snet;
};

#endif
