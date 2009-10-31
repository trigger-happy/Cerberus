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
    class semifinals_dlg;
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

	//Below: slots for network

	/*!
	Called when the client is able to connect.
	*/
	void onConnect();

	/*!
	Called when the client is disconnected.
	*/
	void onDisconnect();

	/*!
	Slot for when the authentication reply has arrived.
	\param result true if we're in, false if not
	*/
	void onAuthenticate ( bool result );


	/*!
	Slot for when the server says the contest state changes.
	*/
	void onContestStateChange ( int round, CONTEST_STATUS s );

	/*!
	Slot for when question in round 3/4 changes in status.
	This is either when the question changes, the timer changes
	or when the timer is started/stopped/paused.
	\param qnum The question number.
	\param time The time for this question.
	\param status The current status for this question.
	*/
	void onQuestionStateChange ( ushort qnum, ushort time, QUESTION_STATUS status );

	/*!
	Slot for receiving current contest time (in seconds)
	*/
	void onContestTime( ushort time );

	/*!
	When question data arrives, this gets called
	\param xml Round 1 question data in xml format.
	*/
	void onQData ( const QString& xml );

	/*!
	Slot for when the server replies to our sent answers.
	\param result true on success, false on failure
	*/
	void onAData ( bool result );

	/*!
	Emitted when there's a contest error.
	\param err ERROR_MESSAGES indicating the error.
	*/
	void onContestError ( ERROR_MESSAGES err );

	/*!
	Emitted when there's an error with the connection.
	\param err SocketError
	*/
	void onError ( const QAbstractSocket::SocketError& err );

	//Below: slots for widgets

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
    Slot for the Previous button on the elims & semis dialog.
	*/
    void elimSemiPrev();

	/*!
    Slot for the Next button on the elims & semis dialog.
	*/
    void elimSemiNext();

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
    Ui::semifinals_dlg* m_semifinals_dlg;
	Ui::summary_dlg* m_summary_dlg;

	QDialog* m_login_w;
	QDialog* m_welcome_w;
	QDialog* m_reconnect_w;
	QDialog* m_elims_w;
    QDialog* m_semifinals_w;
	QDialog* m_summary_w;

	const QString DISCONNECT_QUESTION;
	const QString DISCONNECT_INFORMATION;
	const QString CON_TEXT;
	const QString UNAUTH_TEXT;
	const QString UNAUTH_INFORMATION;

	void displayQuestionAndChoices();
    void recordAnswer();
    void displayAnswer();
    void initializeAnswerData();

	StageData sd;
    AnswerData ad;
	int round;
	int qCount;
};

#endif //CONTESTANT_APP_H
