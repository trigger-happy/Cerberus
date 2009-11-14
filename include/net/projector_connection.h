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
#ifndef PROJECTOR_CONNECTION_H
#define PROJECTOR_CONNECTION_H
#include <QtNetwork>
#include <vector>
#include "net/protocol.h"
#include "data_types.h"

using std::vector;

class ProjectorConnection : public QObject {
	Q_OBJECT;

public:
	ProjectorConnection( QObject* parent = 0, QTcpSocket* socket = 0,
	                     CONTEST_STATUS cstatus = CONTEST_STOPPED,
	                     int round = 1,
	                     ushort ctime = 0 );

	/*!
	Send the contest time to the projector.
	\param time The contest time.
	*/
	inline void setContestTime( ushort time ) {
		m_contime = time;
		sendContestTime();
	}

	/*!
	Send the contest round to the projector.
	\param round The contest round
	*/
	inline void setRound( int round ) {
		m_round = round;
	}

	/*!
	Send the contest state to the projector.
	*/
	void sendContestState();

	/*!
	Set the contest status.
	\param s The contest status.
	*/
	inline void setStatus( CONTEST_STATUS s ) {
		m_con_status = s;
	}

	/*!
	Change the round 3/4 question status.
	\param qnum The question number.
	\param time The time left for this question.
	\param state The status of this question.
	*/
	void setQuestionState( ushort qnum, ushort time, QUESTION_STATUS state );

	/*!
	Show the contest time on projectors.
	*/
	void showContestTime();

	/*!
	Show the question time on screen.
	*/
	void showQuestionTime();

	/*!
	Show the answer to the projector.
	*/
	void showAnswer();

	/*!
	Hide the answer.
	*/
	void hideAnswer();

	/*!
	Send the contest time to the projector.
	*/
	void sendContestTime();

	/*!
	Show the current contesant rankings on the projectors.
	*/
	void showContestRanks( const vector<RankData>& rd );

	/*!
	Show the main screen on the projector.
	*/
	void showMainScreen();

	/*!
	Set the pointer to the stage data. This is to be used by ServerNet.
	\param qdata pointer to a vector of QString containing the xml data.
	*/
	inline void setStageData ( const vector<QString>* qdata ) {
		m_qdata = qdata;
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

signals:
	/*!
	Emitted when the projector disconnects
	\param p Pointer to the ProjectorConnection that disconnected.
	*/
	void projectorDisconnect( ProjectorConnection* p );

	/*!
	Emitted when the projector requests for the contest time.
	\param contime Reference to a ushort that will contain the contest time.
	*/
	void onContestTimeRequest( ushort& contime );

	/*!
	Emitted when the projector is ready to receive commands.
	\param p Pointer to the ProjectorConnection that signaled it's ready.
	*/
	void projectorReady( ProjectorConnection* p );

	/*!
	Emitted when a socket error occurs
	\param error A const reference to a QString containing the error message
	*/
	void onError( const QString& error );

private:
	void sendQData( const QString& xml );
	const vector<QString>* m_qdata;
	bool m_ready;
	ushort m_round;
	ushort m_contime;
	CONTEST_STATUS m_con_status;
	QTcpSocket* m_socket;
	p_header* m_hdr;
};

#endif