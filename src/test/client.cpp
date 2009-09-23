#include <QtGui/QApplication>
#include <iostream>
#include "client.h"
#include "ui_client.h"

using namespace std;

ClientDlg::ClientDlg ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::client_dlg )
{
        m_dlg->setupUi ( this );
        cout << "Constructor" << endl;
        connect ( m_dlg->connect_btn, SIGNAL ( clicked() ), this, SLOT ( onConnectBtn() ) );
        connect ( m_dlg->quit_btn, SIGNAL ( clicked() ), this, SLOT ( onQuitBtn() ) );
        connect ( m_dlg->auth_btn, SIGNAL ( clicked() ), this, SLOT ( onAuthBtn() ) );
        connect ( m_dlg->ads_btn, SIGNAL ( clicked() ), this, SLOT ( onADS() ) );
        connect ( m_dlg->qdr_btn, SIGNAL ( clicked() ), this, SLOT ( onQDR() ) );

        m_net = new ContestantNetwork ( this );
        connect ( m_net, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( onAuthReply ( bool ) ) );
        connect ( m_net, SIGNAL ( onR1QData ( QString ) ), this, SLOT ( onR1QData ( QString ) ) );
        connect ( m_net, SIGNAL ( onR1AData ( bool ) ), this, SLOT ( onR1AData ( bool ) ) );
}

ClientDlg::~ClientDlg()
{
        delete m_dlg;
}

void ClientDlg::onQDR()
{
        m_net->r1QDataRequest();
}

void ClientDlg::onADS()
{
        QString xml = "<?xml version=\"1.0\"?>";
        m_net->r1ADataSend ( xml );
}


void ClientDlg::onR1QData ( const QString& xml )
{
        cout << "Q: " << xml.toStdString() << endl;
}

void ClientDlg::onR1AData ( bool result )
{
        cout << "R1: " << result << endl;
}

void ClientDlg::onAuthReply ( bool result )
{
        cout << "Reply result: " << result << endl;
}

void ClientDlg::onAuthBtn()
{
        m_net->authenticate ( m_dlg->user_ledit->text(), m_dlg->pass_ledit->text() );
}

void ClientDlg::onConnect()
{
        cout << "Connected" << endl;
}

void ClientDlg::onConnectBtn()
{
        cout << "Connect" << endl;
        m_net->connectToHost ( m_dlg->ip_ledit->text(), m_dlg->port_ledit->text().toInt() );
}

void ClientDlg::onQuitBtn()
{
        cout << "Quit" << endl;
        this->close();
}

void ClientDlg::onError ( QAbstractSocket::SocketError error )
{
        cout << "Error" << endl;
}

int main ( int argc, char* argv[] )
{
        QApplication app ( argc, argv );
        ClientDlg c_dlg;
        c_dlg.show();
        return app.exec();
}
