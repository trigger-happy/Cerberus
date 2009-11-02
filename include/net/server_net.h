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
#ifndef SERVER_NET_H
#define SERVER_NET_H
#include <QtNetwork/QtNetwork>
#include <list>
#include <vector>
#include "protocol.h"
#include "patterns/singleton.h"
#include "util/sql_util.h"

using std::list;
using std::vector;

class ContestantConnection;

class ProjectorConnection;

class AdminConnection;

class TempConnection;

typedef list<ContestantConnection*> contestant_list;
typedef list<ProjectorConnection*> projector_list;
typedef list<AdminConnection*> admin_list;
typedef list<TempConnection*> tmpcon_list;

/*!
\brief Networking class to be used by the server.
This class handles the communication with various clients.
This includes the contestants, admin, projectors and user registration clients.
*/

class ServerNetwork : public QObject, public Singleton<ServerNetwork> {
	Q_OBJECT;

public:
	/*!
	Constructor
	\param parent The parent of this object.
	*/
	ServerNetwork ( QObject* parent = 0 );

	/*!
	Destructor
	*/
	~ServerNetwork();

	/*!
	Listen for connections at the specified port.
	\param port The port to listen to.
	*/
	void listen ( quint16 port );

	/*!
	Get the current round from ServerNet.
	\return the current round.
	*/
	inline int getRound() {
		return m_round;
	}

	/*!
	Set the current round of the contest.
	\param round The current round.
	*/
	void setRound ( int round );

	/*!\
	Get the current contest status.
	\return The contest status.
	*/
	CONTEST_STATUS getStatus() {
		return m_con_status;
	}

	/*!
	Set the current contest status.
	\param s The contest status.
	*/
	void setStatus ( CONTEST_STATUS s );

	/*!
	Set the current contest time. Note that this will broadcast to all
	clients.
	\param time The time in an unsigned short.
	*/
	void setContestTime( ushort time );

	/*!
	Get the list of ContestantConnections.
	\return list\<ContestantConnection\>
	*/
	inline const contestant_list& getContestantList() {
		return m_contestants;
	}

	/*!
	Get the list of AdminConnections.
	\return list\<AdminConnection\>
	*/
	inline const admin_list& getAdminList() {
		return m_admins;
	}

	/*!
	Get the list of PresenterConnections.
	\return list\<PresenterConnection\>
	*/
	inline const projector_list& getPresenters() {
		return m_projectors;
	}

	/*!
	Set the question data to be sent to contestants.
	\param qdata A pointer to a vector of QStrings.
	*/
	void setQData ( const vector<QString>* qdata );

	/*!
	Change the round 3/4 question status.
	\param qnum The question number.
	\param time The time left for this question.
	\param state The status of this question.
	*/
	void setQuestionState( ushort qnum, ushort time, QUESTION_STATUS state );

	// projector specific stuff
	/*!
	Show the contest time on projectors.
	*/
	void showContestTime();

	/*!
	Show the current contesant rankings on the projectors.
	\note Parameters are subject to change.
	*/
	void showContestRanks();

	/*!
	Show the question time on screen.
	*/
	void showQuestionTime();

	/*!
	Show the answer to the projector.
	*/
	void showAnswer();

public slots:

	/*!
	Called when there's a new pending connection.
	*/
	void newConnection();

	/*!
	Called when a contestant disconnects.
	\param c A pointer to the ContestantConnection class that disconnected.
	*/
	void contestantDisconnect ( ContestantConnection* c );

	/*!
	Called when a presenter disconnects.
	\param p A pointer to a ProjectorConnection class that disconnected.
	*/
	void projectorDisconnect( ProjectorConnection* p );

	/*!
	Called when a new client is identified in a connection.
	\param con Pointer to a TempConnection.
	\param id The type of client.
	*/
	void newClient ( TempConnection* con, CLIENT_ID id );

	/*!
	Called when a client is found to be invalid.
	\param con Pointer to a TempConnection.
	*/
	void invalidClient ( TempConnection* con );

	/*!
	*/
	void contestTimeResponse( ushort& contime );

signals:
	void badClient ( TempConnection* con );
	void newContestant ( ContestantConnection* cc );
	void contestantDc ( ContestantConnection* cc );
	void newAdmin ( AdminConnection* ac );
	void newProjector ( ProjectorConnection* pc );
	void projectorDc( ProjectorConnection* pc );

protected:
	//contest states
	int m_round;
	ushort m_contime;
	CONTEST_STATUS m_con_status;
	//server socket
	QTcpServer* m_server;
	//connections
	contestant_list m_contestants;
	admin_list m_admins;
	projector_list m_projectors;
	tmpcon_list m_tempconnections;
	const vector<QString>* m_questiondata;
};

#endif //SERVER_NET_H
