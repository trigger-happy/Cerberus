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
#include <algorithm>
#include <cassert>
#include <iostream>
#include "net/server_net.h"
#include "net/temp_connection.h"
#include "net/contestant_connection.h"
#include "net/admin_connection.h"
#include "net/projector_connection.h"

using namespace std;

// TODO: Reconsider placing this in protocol.cpp if ever
bool is_proto_current ( const p_version& ver ) {
	bool result = true;
	result &= ( ver.major == PROTOCOL_MAJOR );
	result &= ( ver.minor == PROTOCOL_MINOR );
	result &= ( ver.patch == PROTOCOL_PATCH );
	return true;
}

// servernetwork implementation
ServerNetwork::ServerNetwork ( QObject* parent ) : QObject ( parent ) {
	//initialize stuff here
	m_server = new QTcpServer ( this );
	connect ( m_server, SIGNAL ( newConnection() ), this, SLOT ( newConnection() ) );
	// default for /IT
	m_numrounds = 4;
}

ServerNetwork::~ServerNetwork() {
}

void ServerNetwork::listen ( quint16 port ) {
	m_server->listen ( QHostAddress::Any, port );
}

void ServerNetwork::newConnection() {
	// shove it into a tempconnection
	TempConnection* tc = new TempConnection ( this, m_server->nextPendingConnection() );
	connect ( tc, SIGNAL ( newClient ( TempConnection*, CLIENT_ID ) ),
	          this, SLOT ( newClient ( TempConnection*, CLIENT_ID ) ) );
	connect ( tc, SIGNAL ( invalidClient ( TempConnection* ) ),
	          this, SLOT ( invalidClient ( TempConnection* ) ) );
	connect ( tc, SIGNAL ( invalidClient ( TempConnection* ) ), this, SIGNAL ( badClient ( TempConnection* ) ) );
	m_tempconnections.insert ( m_tempconnections.end(), tc );
}

void ServerNetwork::contestantDisconnect ( ContestantConnection* c ) {
	emit contestantDc ( c );
	//remove it from the list
	contestant_list::iterator i = m_contestants.begin();

	while ( i != m_contestants.end() ) {
		if ( *i == c ) {
			break;
		}

		i++;
	}

	if ( *i == c ) {
		delete *i;
		m_contestants.erase ( i );
	}
}

void ServerNetwork::projectorDisconnect( ProjectorConnection* p ) {
	emit projectorDc( p );
	projector_list::iterator i = m_projectors.begin();

	while ( i != m_projectors.end() ) {
		if ( *i == p ) {
			delete *i;
			m_projectors.erase( i );
			break;
		}

		i++;
	}
}

void ServerNetwork::newClient ( TempConnection* con, CLIENT_ID id ) {
	QTcpSocket* temp_sock = con->getSocket();

	tmpcon_list::iterator i = m_tempconnections.begin();

	while ( i != m_tempconnections.end() ) {
		if ( *i == con ) {
			break;
		}

		i++;
	}

	if ( *i == con ) {
		delete *i;
		m_tempconnections.erase ( i );
	}

	switch ( id ) {

		case CLIENT_CONTESTANT: {
				ContestantConnection* cc = new ContestantConnection ( this, temp_sock );
				cc->setStatus ( m_con_status );
				cc->setRound ( m_round );
				cc->setQData ( m_questiondata );
				cc->setContestTime( m_contime );
				connect ( cc, SIGNAL ( contestantDisconnect ( ContestantConnection* ) ),
				          this, SLOT ( contestantDisconnect ( ContestantConnection* ) ) );
				connect( cc, SIGNAL( onContestTimeRequest( ushort& ) ),
				         this, SLOT( contestTimeResponse( ushort& ) ) );
				m_contestants.insert ( m_contestants.end(), cc );
				emit newContestant ( cc );
			}

			break;

		case CLIENT_ADMIN:
			// TODO: add code here for creating a new admin connection
			break;

		case CLIENT_PROJECTOR:
			// TODO: add code here for creating a new presenter connection
			{
				ProjectorConnection* pc = new ProjectorConnection( this, temp_sock, m_con_status, m_round, m_numrounds, m_contime );
				pc->setStageData( m_questiondata );
				connect( pc, SIGNAL( projectorDisconnect( ProjectorConnection* ) ),
				         this, SLOT( projectorDisconnect( ProjectorConnection* ) ) );
				connect( pc, SIGNAL( onContestTimeRequest( ushort& ) ) ,
				         this, SLOT( contestTimeResponse( ushort& ) ) );
				m_projectors.insert( m_projectors.end(), pc );
				emit newProjector( pc );
			}

			break;
	}
}

void ServerNetwork::invalidClient ( TempConnection* con ) {
	tmpcon_list::iterator i = m_tempconnections.begin();

	while ( i != m_tempconnections.end() ) {
		if ( *i == con ) {
			break;
		}

		i++;
	}

	if ( *i == con ) {
		delete *i;
		m_tempconnections.erase ( i );
	}
}

void ServerNetwork::setQData ( const vector<QString>* qdata ) {
	m_questiondata = qdata;
}

void ServerNetwork::setRound ( int round ) {
	m_round = round;
	contestant_list::iterator i = m_contestants.begin();

	for ( ; i != m_contestants.end(); i++ ) {
		( *i )->setRound ( m_round );
		( *i )->sendContestState();
	}

	projector_list::iterator j = m_projectors.begin();

	for ( ; j != m_projectors.end(); j++ ) {
		( *j )->setRound( m_round );
		( *j )->sendContestState();
	}
}

void ServerNetwork::setStatus ( CONTEST_STATUS s ) {
	m_con_status = s;
	contestant_list::iterator i = m_contestants.begin();

	for ( ; i != m_contestants.end(); i++ ) {
		( *i )->setStatus ( s );
		( *i )->sendContestState();
	}

	projector_list::iterator j = m_projectors.begin();

	for ( ; j != m_projectors.end(); j++ ) {
		( *j )->setStatus( s );
		( *j )->sendContestState();
	}
}

void ServerNetwork::setContestTime ( ushort time ) {
	m_contime = time;
	/*
	contestant_list::iterator i = m_contestants.begin();

	for ( ;i != m_contestants.end(); i++ ) {
		( *i )->setContestTime ( time );
	}
	*/
	projector_list::iterator j = m_projectors.begin();

	for ( ; j != m_projectors.end(); j++ ) {
		( *j )->setContestTime( time );
	}

}

void ServerNetwork::setQuestionState ( ushort qnum, ushort& time, QUESTION_STATUS state ) {
	m_qnumber = qnum;
	m_qtime = &time;
	m_qstatus = state;

	if ( m_round >= 3 ) {
		contestant_list::iterator i = m_contestants.begin();

		for ( ; i != m_contestants.end(); i++ ) {
			( *i )->setQuestionState ( qnum, time, state );
		}

		projector_list::iterator j = m_projectors.begin();

		for ( ; j != m_projectors.end(); j++ ) {
			( *j )->setQuestionState( qnum, time, state );
		}
	}
}

void ServerNetwork::showContestTime() {
	projector_list::iterator i = m_projectors.begin();

	for ( ; i != m_projectors.end(); i++ ) {
		( *i )->showContestTime();
	}
}

void ServerNetwork::showContestRanks( const std::vector< RankData >& rd ) {
	projector_list::iterator i = m_projectors.begin();

	for ( ; i != m_projectors.end(); i++ ) {
		( *i )->showContestRanks( rd );
	}
}

void ServerNetwork::showQuestionTime() {
	projector_list::iterator i = m_projectors.begin();

	for ( ; i != m_projectors.end(); i++ ) {
		( *i )->showQuestionTime();
	}
}

void ServerNetwork::showAnswer() {
	projector_list::iterator i = m_projectors.begin();

	for ( ; i != m_projectors.end(); i++ ) {
		( *i )->showAnswer();
	}
}

void ServerNetwork::hideAnswer() {
	projector_list::iterator i = m_projectors.begin();

	for ( ; i != m_projectors.end(); i++ ) {
		( *i )->hideAnswer();
	}
}

void ServerNetwork::showMainScreen() {
	projector_list::iterator i = m_projectors.begin();

	for ( ; i != m_projectors.end(); i++ ) {
		( *i )->showMainScreen();
	}
}

void ServerNetwork::contestTimeResponse( ushort& contime ) {
	contime = m_contime;
}

void ServerNetwork::enableContestSubmission( bool e ) {
	contestant_list::iterator i = m_contestants.begin();

	while ( i != m_contestants.end() ) {
		( *i )->enableAnswerSubmission( e );
		i++;
	}
}
