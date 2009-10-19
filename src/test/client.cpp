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
#include <QtGui/QApplication>
#include "client.h"
#include "ui_client.h"

ClientDlg::ClientDlg ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::client_dlg )
{
        m_dlg->setupUi ( this );
        connect ( m_dlg->connect_btn, SIGNAL ( clicked() ), this, SLOT ( onConnectBtn() ) );

        m_net = new ContestantNetwork ( this );
        connect ( m_net, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( onAuthReply ( bool ) ) );
        connect ( m_net, SIGNAL ( onQData ( QString ) ), this, SLOT ( onQData ( QString ) ) );
        connect ( m_net, SIGNAL ( onAData ( bool ) ), this, SLOT ( onAData ( bool ) ) );
        connect ( m_net, SIGNAL ( onConnect() ), this, SLOT ( onConnect() ) );
        connect ( m_net, SIGNAL ( onDisconnect() ), this, SLOT ( onDisconnect() ) );
        connect ( m_net, SIGNAL ( onContestStateChange ( int,CONTEST_STATUS ) ),
                  this, SLOT ( onContestStateChange ( int, CONTEST_STATUS ) ) );
}

ClientDlg::~ClientDlg()
{
        delete m_dlg;
}

void ClientDlg::onContestStateChange ( int round, CONTEST_STATUS s )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Status: " + QString ( "%1 %2\n" ).arg ( round ).arg ( s );
        m_dlg->log_tedt->setText ( buffer );
}


void ClientDlg::onQData ( const QString& xml )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Q: " + xml + "\n";
        m_dlg->log_tedt->setText ( buffer );
}

void ClientDlg::onAData ( bool result )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "R1: ";
        if ( result ) {
                buffer += "true\n";
        } else {
                buffer += "false\n";
        }
        m_dlg->log_tedt->setText ( buffer );;
}

void ClientDlg::onAuthReply ( bool result )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        if ( !result ) {
                buffer += "Failed to authenticate\n";
        } else {
                buffer += "Authenticated!\n";
        }
        if ( result ) {
		// get the contest state next
		buffer += "Getting contest state...\n";
                m_net->getContestState();
	}
	m_dlg->log_tedt->setText ( buffer );
}

void ClientDlg::onConnect()
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Connected\n";
        // TODO: begin test here
        // do an authentication test
        buffer += "Authenticating...\n";
        m_dlg->log_tedt->setText ( buffer );
        m_net->authenticate ( "user", "pass" );
}

void ClientDlg::onDisconnect()
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Disconnected\n";
        m_dlg->log_tedt->setText ( buffer );
}

void ClientDlg::onConnectBtn()
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Connecting...\n";
        m_dlg->log_tedt->setText ( buffer );
        m_net->connectToHost ( "localhost" , 2652 );
}

void ClientDlg::onError ( QAbstractSocket::SocketError error )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Error\n";
        m_dlg->log_tedt->setText ( buffer );
        QMessageBox msg ( this );
        msg.setText ( QString ( "Socket error: %1" ).arg ( error ) );
}

int main ( int argc, char* argv[] )
{
        QApplication app ( argc, argv );
        ClientDlg c_dlg;
        c_dlg.show();
        return app.exec();
}
