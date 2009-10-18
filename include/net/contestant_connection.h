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

        /*!
        Set the round 1 question data that should be sent.
        \TODO: remove this later in place of a better system
        \param xml QString for the data.
        */
        void setR1QData ( const QString* xml );
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
        \param state The state (or contest round).
        */
        void sendContestState ( quint16 state );

        //private fields
        const QString* m_r1qdata;
        QTcpSocket* m_socket;
        quint16 m_blocksize;
        bool m_authenticated;
};

#endif
