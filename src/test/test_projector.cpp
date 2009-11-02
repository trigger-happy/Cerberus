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
#include "ui_projector.h"
#include "test_projector.h"

TestProjector::TestProjector( QDialog* parent ) : QDialog( parent ),
		m_dlg( new Ui::projector_dlg() ) {
	m_dlg->setupUi( this );
	m_network = new ProjectorNet( this );
	connect( m_dlg->connect_btn, SIGNAL( clicked() ),
	         this, SLOT( onConnectBtn() ) );
	connect( m_dlg->quit_btn, SIGNAL( clicked() ),
	         this, SLOT( onQuitBtn() ) );
	connect( m_network, SIGNAL( onConnect() ),
	         this, SLOT( onConnect() ) );
	connect( m_network, SIGNAL( onDisconnect() ),
	         this, SLOT( onDisconnect() ) );
	connect( m_network, SIGNAL( onContestState( ushort, CONTEST_STATUS ) ),
	         this, SLOT( onContestState( ushort, CONTEST_STATUS ) ) );
	connect( m_network, SIGNAL( onContestTime( ushort ) ),
	         this, SLOT( onContestTime( ushort ) ) );
}

void TestProjector::onConnectBtn() {
	writeLog( "Connecting..." );
	m_network->connectToHost( "localhost", 2652 );
}

void TestProjector::onQuitBtn() {
	this->close();
}

void TestProjector::writeLog( const QString& log ) {
	QString buffer = m_dlg->log_tedt->toPlainText();
	buffer += log + "\n";
	m_dlg->log_tedt->setText ( buffer );
}

void TestProjector::onContestState( ushort round, CONTEST_STATUS status ) {
	writeLog( QString( "CState: round %1, status %2" ).arg( round ).arg( status ) );
	writeLog( "Getting contest time..." );
	m_network->getContestTime();
}

void TestProjector::onContestTime( ushort time ) {
	writeLog( QString( "Contest time: %1" ).arg( time ) );
}

void TestProjector::onShowContestTime() {
	writeLog( "Command received: show contest time" );
}

void TestProjector::onShowContestRanks() {
	writeLog( "Command received: show ranks" );
}

void TestProjector::onQuestionTime() {
	writeLog( "Command received: show question time" );
}

void TestProjector::onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status ) {
	writeLog( QString( "QState: qnum = %1, qtime = %2, qstatus = %3" ).arg( qnum ).arg( time ).arg( status ) );
}

void TestProjector::onShowAnswer() {
	writeLog( "Command received: show answer" );
}

void TestProjector::onConnect() {
	writeLog( "Connected to server" );
	writeLog( "Requesting contest state" );
	m_network->getContestState();
}

void TestProjector::onDisconnect() {
	writeLog( "Disconnected from server" );
}

int main( int argc, char* argv[] ) {
	QApplication app( argc, argv );
	TestProjector proj;
	proj.show();
	return app.exec();
}