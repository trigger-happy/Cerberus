/*
Copyright (C) 2009 Michael Ybanez

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
#ifndef CONTESTANT_APP_H
#define CONTESTANT_APP_H
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include "net/contestant_net.h"
#include "data_types.h"
#include "util/xml_util.h"

namespace Ui
{
class login_dlg;
class welcome_dlg;
class reconnect_dlg;
class elims_dlg;
class summary_dlg;
}

class ContestantNetwork;

class ContestantApp : public QDialog
{
        Q_OBJECT;
public:
        ContestantApp ( QWidget* parent = 0 );
        ~ContestantApp();
private slots:

        /*!
        Slot for when the server says the contest state changes.
        \param state The state that the contest is in now.
        */
        void onContestStateChange ( int round, CONTEST_STATUS s );

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
        Called when the client is able to connect.
        */
        void onConnect();

        /*!
        Slot for when the authentication reply has arrived.
        \param result true if we're in, false if not
        */
        void onAuthenticate ( bool result );

        /*!
        When round 1 questions arrive, this gets called.
        \param xml Round 1 question data in xml format.
        */
        void onQData ( const QString& xml );

        /*!
        Slot for when the server replies to our sent answers.
        \param result true on success, false on failure
        */
        void onAData ( bool result );

        /*!
        Slot for when the user clicks the login button on the log-in screen.
        \param the username in the username field
        \param the password in the password field
        */
        void login();

        /*!
        Slot for when the user exits from the login screen.
        */
        void exit();

        /*!
        Slot for when the user clicks the Start button on the welcome screen.
        */
        void welcomeStart();

        /*!
        Slot for the Reconnect button on the dialog which appears upon connection problems.
        */
        void reconnectTry();

        /*!
        Slot for the Cancel button on the dialog which appears upon connection problems.
        */
        void reconnectCancel();

        /*!
        Slot for the Previous button on the round 1 dialog.
        */
		void elimsPrevious();

        /*!
        Slot for the Next button on the round 1 dialog.
        */
		void elimsNext();

		/*!
		Slot for the review button button of the summary dialog
		*/
		void review();

        /*!
        Slot for the submit button button of the summary dialog
        */
        void submit();

private:
        ContestantNetwork* m_network;
        Ui::login_dlg* m_login_dlg;
        Ui::welcome_dlg* m_welcome_dlg;
        Ui::reconnect_dlg* m_reconnect_dlg;
		Ui::elims_dlg* m_elims_dlg;
        Ui::summary_dlg* m_summary_dlg;

        QDialog* m_login_w;
        QDialog* m_welcome_w;
        QDialog* m_reconnect_w;
		QDialog* m_elims_w;
        QDialog* m_summary_w;

        const QString DISCONNECT_QUESTION;
        const QString DISCONNECT_INFORMATION;
		const QString CON_TEXT;
        const QString UNAUTH_TEXT;
        const QString UNAUTH_INFORMATION;

        void displayQuestionAndChoices();

        StageData sd;
		int round;
        int qCount;        
};

#endif //CONTESTANT_APP_H
