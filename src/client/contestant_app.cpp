#include "contestant_app.h"
#include "net/contestant_net.h"
#include "error_defs.h"
#include "ui_login.h"

ContestantApp::ContestantApp ( QWidget* parent )
                : QDialog ( parent ), m_login_dlg ( new Ui::login_dlg )
{
        m_login_dlg->setupUi ( this );
        m_network = new ContestantNetwork ( this );

        connect ( m_network, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( netAuthenticate ( bool ) ) );
        connect ( m_network, SIGNAL ( onConnect() ), this, SLOT ( netConnect() ) );
        connect ( m_network, SIGNAL ( onContestStateChange ( int ) ), this, SLOT ( netContestStateChange ( int ) ) );
        connect ( m_network, SIGNAL ( onContestError ( quint16 ) ), this, SLOT ( netContestError ( quint16 ) ) );
        connect ( m_network, SIGNAL ( onError ( QAbstractSocket::SocketError ) ), this, SLOT ( netError ( QAbstractSocket::SocketError ) ) );
        connect ( m_network, SIGNAL ( onR1AData ( bool ) ), this, SLOT ( netR1AData ( bool ) ) );
        connect ( m_network, SIGNAL ( onR1QData ( QString ) ), this, SLOT ( netR1QData ( QString ) ) );

        connect ( m_login_dlg->login_btn, SIGNAL ( clicked() ), this, SLOT ( loginLogin() ) );
        connect ( m_login_dlg->exit_btn, SIGNAL ( clicked() ), this, SLOT ( loginExit() ) );

}

ContestantApp::~ContestantApp()
{
        delete m_network;
        delete m_login_dlg;
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

int main ( int argc, char* argv[] )
{
        //TODO: implement the app stuff here

        QApplication app ( argc, argv );

        ContestantApp c_app;
        c_app.show();

        return app.exec();
}
