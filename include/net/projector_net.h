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
#ifndef PROJECTOR_NET_H
#define PROJECTOR_NET_H
#include <QtNetwork>
#include <vector>
#include "net/protocol.h"
#include "data_types.h"

using std::vector;

/*!
\brief Networking layer for the projector.
ProjectorNet class handles the networking portion for the projector
application.
*/

class ProjectorNet : public QObject {
	Q_OBJECT;

public:
	/*!
	Constructor
	\param parent A QObject that will serve as this class' parent.
	*/
	ProjectorNet( QObject* parent = 0 );

	/*!
	Destructor
	*/
	~ProjectorNet();

	/*!
	Connect to the Cerberus contest server.
	\param ip The ip address of the server.
	\param port The port that the server is listening to.
	*/
	void connectToHost( const QString& ip, quint16 port );

	/*!
	Disconnect from the server.
	*/
	void disconnectFromHost();

	/*!
	Get the current contest state from the server.
	*/
	void getContestState();

	/*!
	*/
	void getContestTime();

	/*!
	*/
	void sendReadyState();

	/*!
	Request for the question data from the server. Use onQdata signal
	for the xml data.
	\return true if the data was sent, false otherwise.
	*/
	bool qDataRequest ( int round );

public slots:
	/*!
	Called when the app is able to connect to the server.
	*/
	void connected();

	/*!
	Called when the app is disconnected from the server.
	*/
	void disconnected();

	/*!
	Called when there's a socket error.
	\param err The error
	*/
	void error ( const QAbstractSocket::SocketError& err );

	/*!
	Called when there's data ready in the socket for reading.
	*/
	void ready();

signals:
	/*!
	Emitted when the question data has arrived.
	\param xml Question data in xml format
	*/
	void onQData ( const QString& xml );

	void onConnect();
	void onDisconnect();
	void onError( const QString& error );

	void onContestState( ushort round, CONTEST_STATUS status );
	void onContestTime( ushort time );
	void onShowContestTime();
	void onShowContestRanks( const vector<RankData>& rd );
	void onQuestionTime();
	void onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status );
	void onShowAnswer();
	void onShowQuestion();

private:
	QTcpSocket* m_socket;
	p_header* m_hdr;
};

#endif //PROJECTOR_NET_H