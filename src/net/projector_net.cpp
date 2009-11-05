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
#include <vector>
#include "net/projector_net.h"
#include "data_types.h"

using std::vector;

ProjectorNet::ProjectorNet( QObject* parent ) : QObject( parent ) {
	m_socket = new QTcpSocket ( this );
	connect ( m_socket, SIGNAL ( connected() ), this, SLOT ( connected() ) );
	connect ( m_socket, SIGNAL ( disconnected() ), this, SLOT ( disconnected() ) );
	connect ( m_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
	          this, SLOT ( error ( QAbstractSocket::SocketError ) ) );
	connect ( m_socket, SIGNAL ( readyRead() ), this, SLOT ( ready() ) );
	m_hdr = NULL;
}

ProjectorNet::~ProjectorNet() {
	delete m_socket;
}

void ProjectorNet::connectToHost( const QString& ip, quint16 port ) {
	m_socket->connectToHost( ip, port );
}

void ProjectorNet::disconnectFromHost() {
	m_socket->disconnectFromHost();
}

void ProjectorNet::connected() {
	// identify ourselves
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.length = sizeof ( uchar );
	hdr.command = NET_INITIATE_CONNECTION;

	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	out << ( uchar ) CLIENT_PROJECTOR;

	m_socket->write ( block );
}

void ProjectorNet::disconnected() {
	emit onDisconnect();
}

void ProjectorNet::error( const QAbstractSocket::SocketError& error ) {
	QString msg = m_socket->errorString();
	emit onError( msg );
}

void ProjectorNet::ready() {
	QDataStream in ( m_socket );
	in.setVersion ( QDataStream::Qt_4_5 );

	if ( m_hdr == NULL ) {
		if ( m_socket->bytesAvailable() < ( int ) sizeof ( p_header ) ) {
			return;
		}

		m_hdr = new p_header;

		in.readRawData ( ( char* ) m_hdr, sizeof ( p_header ) );
		//check the packet

		if ( strcmp ( ( const char* ) m_hdr->ident.data, "CERB" ) != 0 ) {
			// bad packet, do something here
			return;
		}

		//check the version
		if ( !is_proto_current ( m_hdr->ver ) ) {
			// the version is not the same, do something here
		}
	}

	if ( m_socket->bytesAvailable() < m_hdr->length ) {
		return;
	}

	switch ( m_hdr->command ) {

		case INF_QUESTION_DATA:
			//we have our question data
			{
				QString xml;
				uchar hash[20];
				in.readRawData ( ( char* ) hash, 20 );
				in >> xml;
				QByteArray testhash = QCryptographicHash::hash ( xml.toAscii(), QCryptographicHash::Sha1 );
				QByteArray testhash2;

				for ( int i = 0; i < 20; i++ ) {
					testhash2.push_back ( hash[i] );
				}

				if ( testhash == testhash2 ) {
					emit onStageData ( xml );
				} else {
					// TODO: act on invalid data
				}
			}

			break;

		case NET_CONNECTION_RESULT:
			// check the result
			{
				ushort result;
				in >> result;

				if ( result ) {
					emit onConnect();
				}
			}

			break;

		case INF_CONTEST_STATE:
			//we got info on the contest state
			{
				ushort round;
				uchar status;
				in >> round >> status;
				emit onContestState ( round, ( CONTEST_STATUS ) status );
			}

			break;

		case INF_CONTEST_TIME:
			// info on contest time
			{
				ushort time;
				in >> time;
				emit onContestTime( time );
			}

			break;

		case PJR_SHOW_TIME:
			emit onShowContestTime();
			break;

		case PJR_SHOW_RANKS: {
				// here comes a load of insanity
				ushort num_entries;
				in >> num_entries;
				vector<RankData> rd;

				for ( int i = 0; i < num_entries; i++ ) {
					RankData temp;
					in >> temp.rank;
					in >> temp.score;
					ushort size;
					in >> size;
					QByteArray buffer;
					buffer = in.device()->read( size );
					temp.fullname = buffer;
					buffer.clear();
					in >> size;
					buffer = in.device()->read( size );
					temp.teamname = buffer;
					rd.push_back( temp );
				}

				emit onShowContestRanks( rd );
			}

			break;

		case INF_QUESTION_TIME:
			// TODO: might remove this
			break;

		case INF_QUESTION_STATE: {
				ushort qnum, time, status;
				in >> qnum >> time >> status;
				emit onQuestionState( qnum, time, ( QUESTION_STATUS )status );
			}

			break;

		case PJR_SHOW_ANSWER:
			emit onShowAnswer();
			break;

		case PJR_SHOW_QUESTION:
			emit onShowQuestion();
			break;

		default:
			;
	}

	delete m_hdr;

	m_hdr = NULL;

	if ( m_socket->bytesAvailable() > 0 ) {
		ready();
	}
}

void ProjectorNet::getContestState() {
	// ripped off from ContestantNet
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	//construct the header
	p_header hdr;
	hdr.length = 0;
	hdr.command = QRY_CONTEST_STATE;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}

void ProjectorNet::getContestTime() {
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	//construct the header
	p_header hdr;
	hdr.length = 0;
	hdr.command = QRY_CONTEST_TIME;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}

void ProjectorNet::sendReadyState() {
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	//construct the header
	p_header hdr;
	hdr.length = 0;
	hdr.command = QRY_PROJECTOR_READY;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}

void ProjectorNet::getStageData( int round ) {
	// construct the packet
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	p_header hdr;
	hdr.command = QRY_QUESTION_REQUEST;
	// construct the payload
	hdr.length = sizeof ( ushort );
	// write it
	out.writeRawData ( ( const char* ) &hdr, sizeof ( hdr ) );
	out << ( ushort ) round;
	// send it
	m_socket->write ( block );
}
