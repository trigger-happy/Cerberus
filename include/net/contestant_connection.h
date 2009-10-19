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
#include "protocol.h"
#include "error_defs.h"

using std::list;

/*!
\brief A class representing a connection with a contestant's client.
This class is created for each client connection that is made to the server.
*/
class ContestantConnection : public QObject
{
        Q_OBJECT;
public:
        /*!
        Constructor.
        \param parent Set the parent of this class.
        */
        ContestantConnection ( QObject* parent = 0, QTcpSocket* socket = 0 );

	inline void setRound(int r){
		m_round = r;
	}
	
	inline void setStatus(CONTEST_STATUS s){
		m_con_status = s;
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
        */
        void contestantDisconnect ( ContestantConnection* cc );
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
        Send the round 1 questin data.
        \param xml QString containing the xml data.
        */
        void sendR1QData ( const QString& xml );

        /*!
        Send a reply to the answer submission.
        \param res The result of the submission (not very relevant).
        */
        void sendR1AReply ( bool res );

        /*!
        Send a command to change the contest state.
        */
        void sendContestState ();

        //private fields
        QTcpSocket* m_socket;
        p_header* m_hdr;
        bool m_authenticated;
	CONTEST_STATUS m_con_status;
	int m_round;
};

#endif
