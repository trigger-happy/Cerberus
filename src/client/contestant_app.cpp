#include "contestant_app.h"
#include "net/contestant_net.h"
#include "error_defs.h"
#include "ui_login.h"
#include "ui_welcome.h"
#include "ui_reconnect.h"
#include "ui_round1.h"
#include "ui_summary.h"

ContestantApp::ContestantApp ( QWidget* parent )
                : QDialog ( parent ),
                DISCONNECT_INFORMATION ( tr ( "There will be a penalty for disconnecting." ) ),
                DISCONNECT_QUESTION ( tr ( "Are you sure you want to exit the program?" ) )
{
	m_login_dlg = new Ui::login_dlg;
	m_welcome_dlg = new Ui::welcome_dlg;
	m_reconnect_dlg = new Ui::reconnect_dlg;
        m_round1_dlg = new Ui::round1_dlg;
        m_summary_dlg = new Ui::summary_dlg;
	
        this->hide();
        m_welcome_w = new QDialog ( this );
        m_welcome_dlg->setupUi ( m_welcome_w );
        m_welcome_w->hide();

        m_reconnect_w = new QDialog ( this );
        m_reconnect_dlg->setupUi ( m_reconnect_w );
        m_reconnect_w->hide();

        m_round1_w = new QDialog ( this );
        m_round1_dlg->setupUi ( m_round1_w );
        m_round1_w->hide();

        m_summary_w = new QDialog ( this );
        m_summary_dlg->setupUi ( m_summary_w );
        m_summary_w->hide();

        m_login_w = new QDialog ( this );
        m_login_dlg->setupUi ( m_login_w );
        m_login_w->show();

        m_network = new ContestantNetwork ( this );

        connect ( m_network, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( netAuthenticate ( bool ) ) );
        connect ( m_network, SIGNAL ( onConnect() ), this, SLOT ( netConnect() ) );
        connect ( m_network, SIGNAL ( onContestStateChange ( int ) ), this, SLOT ( netContestStateChange ( int ) ) );
        connect ( m_network, SIGNAL ( onContestError ( quint16 ) ), this, SLOT ( netContestError ( quint16 ) ) );
        connect ( m_network, SIGNAL ( onError ( QAbstractSocket::SocketError ) ), this, SLOT ( netError ( QAbstractSocket::SocketError ) ) );
        connect ( m_network, SIGNAL ( onR1AData ( bool ) ), this, SLOT ( netR1AData ( bool ) ) );
        connect ( m_network, SIGNAL ( onR1QData ( QString ) ), this, SLOT ( netR1QData ( QString ) ) );

        // connections for the login dialog
        connect ( m_login_dlg->login_btn, SIGNAL ( clicked() ), this, SLOT ( loginLogin() ) );
        connect ( m_login_dlg->exit_btn, SIGNAL ( clicked() ), this, SLOT ( loginExit() ) );

        // connections for the welcome dialog
        connect ( m_welcome_dlg->start_btn, SIGNAL ( clicked() ), this, SLOT ( welcomeStart() ) );

        // connections for the reconnect dialog
        connect ( m_reconnect_dlg->try_btn, SIGNAL ( clicked() ), this, SLOT ( reconnectTry() ) );
        connect ( m_reconnect_dlg->cancel_btn, SIGNAL ( clicked() ), this, SLOT ( reconnectCancel() ) );

        // connections for the round 1 dialog
        connect ( m_round1_dlg->previous_btn, SIGNAL( clicked() ), this, SLOT ( round1Previous() ) );
        connect ( m_round1_dlg->next_btn, SIGNAL( clicked() ), this, SLOT ( round1Next() ) );

        // TODO: get the client configuration from XmlUtil

        // TODO: connect to the server here

}

ContestantApp::~ContestantApp()
{
        delete m_network;
        delete m_login_w;
        delete m_welcome_w;
        delete m_reconnect_w;
        delete m_round1_w;
}

void ContestantApp::netContestStateChange ( int state )
{
        //TODO: do something here for when the contest state changes
}

void ContestantApp::netError ( const QAbstractSocket::SocketError& err )
{
        //TODO: do something here when there's a network error.
}

void ContestantApp::netContestError ( quint16 err )
{
        //TODO: do something here for contest errors.
}

void ContestantApp::netConnect()
{
        //TODO: implement actions for when we have a connection
}

void ContestantApp::netAuthenticate ( bool result )
{
        //TODO: do something here for authorization replies.
}

void ContestantApp::netR1QData ( const QString& xml )
{
        //TODO: do something here for question data arriving.
}

void ContestantApp::netR1AData ( bool result )
{
        //TODO: do something here for replies to answer uploads.
}

void ContestantApp::loginLogin()
{
        m_network->authenticate ( m_login_dlg->username_line->text(), m_login_dlg->password_line->text() );
}

void ContestantApp::loginExit()
{
        this->close();
}

void ContestantApp::welcomeStart()
{
        // to do later
}

void ContestantApp::reconnectTry()
{
}

void ContestantApp::reconnectCancel()
{
        QMessageBox msgBox;
        msgBox.setWindowTitle ( "Confirm Disconnection" );
        msgBox.setText ( DISCONNECT_QUESTION );
        msgBox.setInformativeText ( DISCONNECT_INFORMATION );
        msgBox.setStandardButtons ( QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton ( QMessageBox::No );
        msgBox.setIcon ( QMessageBox::Question );
        int disconnect = msgBox.exec();

        switch ( disconnect ) {
        case QMessageBox::Yes:
                // "close the whole friggin program" - jim

        case QMessageBox::No:
                break;
        }
}

void ContestantApp::round1Next()
{
}

void ContestantApp::round1Previous()
{
}

int main ( int argc, char* argv[] )
{
        //TODO: implement the app stuff here

        QApplication app ( argc, argv );

        ContestantApp c_app;
        //c_app.show();

        return app.exec();
}
