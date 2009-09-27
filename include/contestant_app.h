#ifndef CONTESTANT_APP_H
#define CONTESTANT_APP_H
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>

namespace Ui
{
class login_dlg;
class welcome_dlg;
class reconnect_dlg;
class round1_dlg;
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
        void netContestStateChange ( int state );

        /*!
        Emitted when there's an error with the connection.
        \param err SocketError
        */
        void netError ( const QAbstractSocket::SocketError& err );

        /*!
        Emitted when there's a contest error.
        \param err ERROR_MESSAGES indicating the error.
        */
        void netContestError ( quint16 err );

        /*!
        Called when the client is able to connect.
        */
        void netConnect();

        /*!
        Slot for when the authentication reply has arrived.
        \param result true if we're in, false if not
        */
        void netAuthenticate ( bool result );

        /*!
        When round 1 questions arrive, this gets called.
        \param xml Round 1 question data in xml format.
        */
        void netR1QData ( const QString& xml );

        /*!
        Slot for when the server replies to our sent answers.
        \param result true on success, false on failure
        */
        void netR1AData ( bool result );

        /*!
        Slot for when the user clicks the login button on the log-in screen.
        \param the username in the username field
        \param the password in the password field
        */
        void loginLogin();

        /*!
        Slot for when the user exits from the login screen.
        */
        void loginExit();

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
	*/

private:
        ContestantNetwork* m_network;
        Ui::login_dlg* m_login_dlg;
        Ui::welcome_dlg* m_welcome_dlg;
        Ui::reconnect_dlg* m_reconnect_dlg;
        Ui::round1_dlg* round1_dlg;
        Ui::summary_dlg* summary_dlg;
	
	QDialog* m_login_w;
	QDialog* m_welcome_w;
	QDialog* m_reconnect_w;
	QDialog* m_round1_w;
	QDialog* m_summary_w;

        const QString DISCONNECT_QUESTION;
        const QString DISCONNECT_INFORMATION;
};

#endif //CONTESTANT_APP_H
