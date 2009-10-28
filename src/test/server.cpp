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
#include <QApplication>
#include <iostream>
#include "ui_server.h"
#include "server.h"
#include "util/xml_util.h"
#include "util/sql_util.h"

using namespace std;

ServerDlg::ServerDlg ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::server_dlg() )
{
        //load up the question data
        for ( int i = 1; i <= 4; i++ ) {
                QFile file ( QString ( "resources/stage%1_q.xml" ).arg ( i ) );
                file.open ( QIODevice::ReadOnly );
                m_questions.push_back ( file.readAll() );
        }
        m_dlg->setupUi ( this );
        connect ( m_dlg->quit_btn, SIGNAL ( clicked() ), this, SLOT ( onQuitBtn() ) );
        m_server = new ServerNetwork ( this );
        connect ( m_dlg->stop_btn, SIGNAL ( clicked() ), this, SLOT ( onStopBtn() ) );
        connect ( m_dlg->change_btn, SIGNAL ( clicked() ), this, SLOT ( onRoundChangeBtn() ) );
        connect ( m_dlg->pause_btn, SIGNAL ( clicked() ), this, SLOT ( onPauseBtn() ) );
        connect ( m_dlg->start_btn, SIGNAL ( clicked() ), this, SLOT ( onStartBtn() ) );
        connect ( m_dlg->time_btn, SIGNAL ( clicked() ), this, SLOT ( onTimeBtn() ) );
        connect ( m_dlg->question_btn, SIGNAL ( clicked() ), this, SLOT ( onQuestionBtn() ) );

        connect ( m_server, SIGNAL ( newContestant ( ContestantConnection* ) ),
                  this, SLOT ( newContestant ( ContestantConnection* ) ) );
        connect ( m_server, SIGNAL ( badClient ( TempConnection* ) ),
                  this, SLOT ( badClient ( TempConnection* ) ) );
        connect ( m_server, SIGNAL ( contestantDc ( ContestantConnection* ) ),
                  this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
        m_server->listen ( 2652 );
        m_server->setRound ( 1 );
        m_server->setStatus ( CONTEST_STOPPED );
        m_server->setQData ( &m_questions );
        bool result = SqlUtil::getInstance().init ( "resources/test.db" );
        if ( !result ) {
                QMessageBox msg ( this );
                msg.setText ( "Failed to load db" );
                msg.exec();
        }
}

ServerDlg::~ServerDlg()
{
        delete m_dlg;
}

void ServerDlg::onStopBtn()
{
        m_server->setStatus ( CONTEST_STOPPED );
        writeLog ( "Contest stopped" );
}

void ServerDlg::onStartBtn()
{
        m_server->setStatus ( CONTEST_RUNNING );
        writeLog ( "Contest running" );
}

void ServerDlg::onPauseBtn()
{
        m_server->setStatus ( CONTEST_PAUSED );
        writeLog ( "Contest paused" );
}

void ServerDlg::onRoundChangeBtn()
{
        int round = m_dlg->round_line->text().toInt();
        m_server->setRound ( round );
        writeLog ( QString ( "Round changed to %1" ).arg ( round ) );
}

void ServerDlg::newContestant ( ContestantConnection* cc )
{
        writeLog ( "New Contestant connection" );
}

void ServerDlg::contestantDisconnect ( ContestantConnection* cc )
{
        writeLog ( "Contestant Disconnected" );
}

void ServerDlg::onQuitBtn()
{
        this->close();
}

void ServerDlg::badClient ( TempConnection* tc )
{
        writeLog ( "Bad client" );
}

void ServerDlg::writeLog ( const QString& s )
{
        QString buffer = m_dlg->log_tedt->toPlainText();
        buffer += s+"\n";
        m_dlg->log_tedt->setText ( buffer );
}

void ServerDlg::onQuestionBtn()
{
        ushort q = m_dlg->question_line->text().toUShort();
        writeLog ( QString ( "Question number set to: %1" ).arg ( q ) );
        m_server->setQuestion ( q );
}

void ServerDlg::onTimeBtn()
{
        ushort time = m_dlg->time_line->text().toUShort();
        writeLog ( QString ( "Contest time changed to: %1" ).arg ( time ) );
        m_server->setContestTime ( time );
}

int main ( int argc, char* argv[] )
{
        QApplication app ( argc, argv );
        ServerDlg dlg;
        dlg.show();
        return app.exec();
}
