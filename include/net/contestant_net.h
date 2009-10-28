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
        Request for the current contest state.
        */
        void getContestState();

        /*!
        Request for the question data from the server. Use onQdata signal
        for the xml data.
        \return true if the data was sent, false otherwise.
        */
        bool qDataRequest ( int round );

        /*!
        Send the answer data to the server. Use onAData signal for the
        server's response.
        \param xml QString containing the xml answer data to be sent.
        \return true if the data was sent, false otherwise.
        */
        bool aDataSend ( const QString& xml );
		
		/*!
		Request for the contest time from the server. Use onContestTime
		signal for the server's response
		\return true if the data was sent, false otherwise.
		*/
		bool getContestTime();

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
        Emitted when there's a change to the contest state or as a reply
        from the server to our state request.
        \param round The current round
        \param status The current contest status
        */
        void onContestStateChange ( int round, CONTEST_STATUS status );

        /*!
        Emitted when there's an error with the connection.
        \param err SocketError
        */
        void onError ( const QAbstractSocket::SocketError& err );

        /*!
        Emitted when there's a contest error.
        \param err ERROR_MESSAGES indicating the error.
        */
        void onContestError ( ERROR_MESSAGES err );

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
        \param xml Question data in xml format
        */
        void onQData ( const QString& xml );

        /*!
        Emitted when server replies to our submission.
        \param result true on success, false on failure
        */
        void onAData ( bool result );
		
		/*!
		Emitted when server replies to our request for the current
		contest time.
		\param time unsigned int containing the contest time.
		*/
		void onContestTime(unsigned int time);
		
		/*!
		Emitted when the question for round 3/4 is changed. The contest
		pacing for this round is controlled and this basically determines
		when it's time to move to the next question.
        \param qnum The question number.
		*/
		void onQuestionChange(int qnum);
protected:
        QTcpSocket* m_socket;
        p_header* m_hdr;
        bool m_authenticated;
private:
};

#endif //CONTESTANT_NET_H
