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
#include <cassert>
#include "client.h"
#include "ui_client.h"

using namespace std;

ClientDlg::ClientDlg ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::client_dlg )
{
        m_dlg->setupUi ( this );
        connect ( m_dlg->connect_btn, SIGNAL ( clicked() ), this, SLOT ( onConnectBtn() ) );
        connect ( m_dlg->clear_btn, SIGNAL ( clicked() ), this, SLOT ( onClearBtn() ) );

        m_net = new ContestantNetwork ( this );
        connect ( m_net, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( onAuthReply ( bool ) ) );
        connect ( m_net, SIGNAL ( onQData ( QString ) ), this, SLOT ( onQData ( QString ) ) );
        connect ( m_net, SIGNAL ( onAData ( bool ) ), this, SLOT ( onAData ( bool ) ) );
        connect ( m_net, SIGNAL ( onConnect() ), this, SLOT ( onConnect() ) );
        connect ( m_net, SIGNAL ( onDisconnect() ), this, SLOT ( onDisconnect() ) );
        connect ( m_net, SIGNAL ( onContestStateChange ( int,CONTEST_STATUS ) ),
                  this, SLOT ( onContestStateChange ( int, CONTEST_STATUS ) ) );
        connect ( m_net, SIGNAL ( onContestError ( ERROR_MESSAGES ) ),
                  this, SLOT ( onContestError ( ERROR_MESSAGES ) ) );
        connect ( m_net, SIGNAL ( onContestTime ( ushort ) ), this, SLOT ( onContestTimeChange ( ushort ) ) );
        connect ( m_net, SIGNAL ( onQuestionStateChange ( ushort, ushort, QUESTION_STATUS ) ), 
				  this, SLOT ( onQuestionChange ( ushort, ushort, QUESTION_STATUS ) ) );
}

ClientDlg::~ClientDlg()
{
        delete m_dlg;
}

void ClientDlg::onContestStateChange ( int round, CONTEST_STATUS s )
{
        writeLog ( QString ( "Current round is %1 and state is %2" ).arg ( round ).arg ( s ) );
        //get the question data
        m_net->qDataRequest ( round );
}


void ClientDlg::onQData ( const QString& xml )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += "Q: " + xml + "\n";
        m_dlg->log_tedt->setText ( buffer );
        // try submitting
        QFile file ( "resources/stage1_a.xml" );
        file.open ( QIODevice::ReadOnly );
        QString ans = file.readAll();
        m_net->aDataSend ( ans );
}

void ClientDlg::onAData ( bool result )
{
        if ( result ) {
                writeLog ( "Answer submission returned true" );
        } else {
                writeLog ( "Answer submission returned false" );
        }
}

void ClientDlg::onAuthReply ( bool result )
{
        if ( !result ) {
                writeLog ( "Failed to authenticate" );
        } else {
                writeLog ( "Authenticated" );
        }
        if ( result ) {
                // get the contest state next
                writeLog ( "Getting contest state..." );
                m_net->getContestState();
        }
}

void ClientDlg::onConnect()
{
        writeLog ( "Connected" );
        writeLog ( "Authenticating.." );
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
        writeLog ( "Connecting.." );
        m_net->connectToHost ( "localhost" , 2652 );
}

void ClientDlg::onError ( QAbstractSocket::SocketError error )
{
        writeLog ( QString ( "Error: %1" ).arg ( error ) );
        QMessageBox msg ( this );
        msg.setText ( QString ( "Socket error: %1" ).arg ( error ) );
}

void ClientDlg::writeLog ( const QString& s )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += s +"\n";
        m_dlg->log_tedt->setText ( buffer );
}

void ClientDlg::onClearBtn()
{
        m_dlg->log_tedt->clear();
}

void ClientDlg::onContestError ( ERROR_MESSAGES err )
{
        switch ( err ) {
        case ERR_NOTAUTHORIZED:
                writeLog ( "Server returned that we're not authorized" );
                break;
        case ERR_BADCOMMAND:
                writeLog ( "Server returned bad command" );
                break;
        case ERR_CONTEST_STOPPED:
                writeLog ( "Contest is stopped" );
                break;
        case ERR_UNKNOWN:
                writeLog ( "Server returned unknown error" );
                break;
        default:
                assert ( false );
        }
}

void ClientDlg::onContestTimeChange ( ushort time )
{
        writeLog ( QString ( "Contest time now is: %1" ).arg ( time ) );
}

void ClientDlg::onQuestionChange ( ushort q, ushort time, QUESTION_STATUS status )
{
        writeLog ( QString ( "Question changed to: %1 %2 %3" ).arg ( q ).arg(time).arg((ushort) status) );
}

int main ( int argc, char* argv[] )
{
        QApplication app ( argc, argv );
        ClientDlg c_dlg;
        c_dlg.show();
        return app.exec();
}
