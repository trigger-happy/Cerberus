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
#ifndef CONTESTANT_NET_H
#define CONTESTANT_NET_H
#include <QtNetwork/QtNetwork>
#include "error_defs.h"
#include "protocol.h"

enum CCSTATE {
        CCS_DISCONNECTED = 0,
        CCS_AUTHENTICATING,
        CCS_R1QDATA_REQUEST,
        CCS_R1ADATA_SEND,
        CCS_STANDBY
};

/*!
\brief This class provides a simple communication layer with the server.
The ContestantNetwork class provides a networking layer for the contest client.
The intended design for this class is to hide away most of the core networking
stuff from the user. The class aims for a non-blocking approach through the
use of Qt's signals and slots.
*/
class ContestantNetwork : public QObject
{
        Q_OBJECT;
public:
        /*!
        Constructor
        \param parent A pointer to a QObject to be set at this class' parent.
        */
        ContestantNetwork ( QObject* parent = 0 );

        /*!
        Destructor
        */
        virtual ~ContestantNetwork();

        /*!
        Connect to the server using the specified connection info.
        \param ip QString containing the ip address.
        \param port integer containing the port.
        */
        void connectToHost ( const QString& ip, quint16 port );

        /*!
        Disconnect from the host.
        */
        void disconnectFromHost();

        /*!
        Returns the authentication status of the client.
        \return The authentication status.
        */
        inline bool isAuthenticated() {
                return m_authenticated;
        }

        /*!
        Attempt to authenticate (use the onAuthenticate signal to get the response).
        \param user_name QString containing the user name.
        \param pw QString containing the password.
        \return true if the data was sent, false otherwise.
        */
        bool authenticate ( const QString& user_name, const QString& pw );

        /*!
        Requet for the round 1 question data from the server.
        \return true if the data was sent, false otherwise.
        */
        bool r1QDataRequest();

        /*!
        Send the answer data for 1st round to the server.
        \param xml QString containing the xml answer data to be sent.
        \return true if the data was sent, false otherwise.
        */
        bool r1ADataSend ( const QString& xml );

        /*!
        Get the current state of the connection class.
        */
        inline CCSTATE getState() const {
                return m_state;
        }
protected slots:

        /*!
        Called internally when the socket connects successfuly.
        */
        void connected();

        /*!
        Called internally when the socket disconnects
        */
        void disconnected();

        /*!
        Called internally when there's an error.
        \param err The socket error
        */
        void error ( const QAbstractSocket::SocketError& err );

        /*!
        Called internally when there's data to be read in the socket.
        */
        void ready();
signals:

        /*!
        Signal for disconnections.
        */
        void onDisconnect();

        /*!
        Signal for when the server states that the contest state has changed.
        \param state The state that the contest is in now.
        */
        void onContestStateChange ( int state );

        /*!
        Emitted when there's an error with the connection.
        \param err SocketError
        */
        void onError ( const QAbstractSocket::SocketError& err );

        /*!
        Emitted when there's a contest error.
        \param err ERROR_MESSAGES indicating the error.
        */
        void onContestError ( quint16 err );

        /*!
        Signal emitted when the connection to the server has been established.
        */
        void onConnect();

        /*!
        Emitted when there's an authentication reply from the server.
        \param result true if we're in, false if not
        */
        void onAuthenticate ( bool result );

        /*!
        Emitted when the question data has arrived.
        \param xml Round 1 question data in xml format.
        */
        void onR1QData ( const QString& xml );

        /*!
        Emitted when server replies to our submission.
        \param result true on success, false on failure
        */
        void onR1AData ( bool result );
protected:
        QTcpSocket* m_socket;
        bool m_authenticated;
        CCSTATE m_state;
        quint16 m_blocksize;
private:
};

#endif //CONTESTANT_NET_H
