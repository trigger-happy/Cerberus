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
	ProjectorConnection( QObject* parent = 0, QTcpSocket* socket = 0 );

	/*!
	*/
	inline void setContestTime( ushort time ) {
		m_contime = time;
		sendContestTime();
	}

	/*!
	*/
	inline void setRound( int round ) {
		m_round = round;
	}

	/*!
	*/
	void sendContestState();

	/*!
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
	*/
	void showQuestion();

	/*!
	*/
	void sendContestTime();

	/*!
	Show the current contesant rankings on the projectors.
	*/
	void showContestRanks( const vector<RankData>& rd );

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
	*/
	void contestTimeRequest( ushort& contime );

	/*!
	*/
	void projectorReady( ProjectorConnection* p );

private:
	bool m_ready;
	ushort m_round;
	ushort m_contime;
	CONTEST_STATUS m_con_status;
	QTcpSocket* m_socket;
	p_header* m_hdr;
};

#endif