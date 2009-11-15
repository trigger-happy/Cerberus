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
#include "net/projector_connection.h"
#include "net/protocol.h"
#include "data_types.h"

using std::vector;

ProjectorConnection::ProjectorConnection( QObject* parent, QTcpSocket* socket,
        CONTEST_STATUS cstatus, int round, ushort ctime ) : QObject( parent ), m_socket( socket ) {
	m_ready = false;
	m_con_status = cstatus;
	m_round = round;
	m_contime = ctime;
	connect ( m_socket, SIGNAL ( disconnected() ), this, SLOT ( disconnected() ) );
	connect ( m_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
	          this, SLOT ( error ( QAbstractSocket::SocketError ) ) );
	connect ( m_socket, SIGNAL ( readyRead() ), this, SLOT ( ready() ) );
	m_hdr = NULL;
	// reply to client of the now established connection
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.length = sizeof ( ushort );
	hdr.command = NET_CONNECTION_RESULT;

	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	out << ( ushort ) true;

	m_socket->write ( block );
}

void ProjectorConnection::error( const QAbstractSocket::SocketError& err ) {
	emit onError( m_socket->errorString() );
}

void ProjectorConnection::ready() {
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

		case QRY_QUESTION_REQUEST:
			//contestant is asking for question data
			ushort round;
			in >> round;
			sendQData ( m_qdata->at ( round - 1 ) );

			break;

		case QRY_CONTEST_STATE:
			//contestant is asking for the contest state.

			sendContestState();
			break;

		case QRY_CONTEST_TIME:
			emit onContestTimeRequest( m_contime );
			sendContestTime();
			break;

		case QRY_PROJECTOR_READY:
			m_ready = true;
			emit projectorReady( this );
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

void ProjectorConnection::disconnected() {
	emit projectorDisconnect( this );
}

void ProjectorConnection::setQuestionState( ushort qnum, ushort time, QUESTION_STATUS state ) {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = INF_QUESTION_STATE;
	hdr.length = sizeof ( ushort ) * 3;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	out << ( ushort ) qnum << ( ushort ) time << ( ushort ) state;
	m_socket->write ( block );
}

void ProjectorConnection::showContestTime() {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = PJR_SHOW_TIME;
	hdr.length = 0;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}

void ProjectorConnection::showQuestionTime() {
	showContestTime();
}

void ProjectorConnection::showAnswer() {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = PJR_SHOW_ANSWER;
	hdr.length = 0;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}

void ProjectorConnection::hideAnswer() {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = PJR_HIDE_ANSWER;
	hdr.length = 0;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}

void ProjectorConnection::sendContestState() {
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.length = sizeof ( ushort ) + sizeof ( uchar );
	hdr.command = INF_CONTEST_STATE;

	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	out << ( ushort ) m_round << ( uchar ) m_con_status;

	m_socket->write ( block );
}

void ProjectorConnection::sendContestTime() {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = INF_CONTEST_TIME;
	hdr.length = sizeof ( ushort );
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	out << ( ushort ) m_contime;
	m_socket->write ( block );
}

void ProjectorConnection::showContestRanks( const vector<RankData>& rd ) {

	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = PJR_SHOW_RANKS;
	out.writeRawData( ( const char* )&hdr, sizeof( p_header ) );

	out << ( ushort ) rd.size();

	for ( int i = 0; i < rd.size(); i++ ) {
		out << ( ushort ) rd[i].rank;
		out << ( double ) rd[i].score;
		out << ( ushort ) rd[i].time;
		out << ( ushort ) rd[i].fullname.size();
		out.writeRawData( rd[i].fullname.toAscii().data(), rd[i].fullname.size() );
		out << ( ushort ) rd[i].teamname.size();
		out.writeRawData( rd[i].teamname.toAscii().data(), rd[i].teamname.size() );
	}

	//write the data
	hdr.length = block.size() - sizeof( p_header );

	out.device()->seek( 0 );

	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );

	m_socket->write ( block );

	m_socket->write ( block );
}

void ProjectorConnection::sendQData( const QString& xml ) {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = INF_QUESTION_DATA;
	QByteArray hash = QCryptographicHash::hash ( xml.toAscii(), QCryptographicHash::Sha1 );
	hdr.length = hash.size() + xml.size();
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	out.writeRawData ( hash.data(), hash.size() );
	out << xml;
	m_socket->write ( block );
}

void ProjectorConnection::showMainScreen() {
	//construct the packet and send it
	QByteArray block;
	QDataStream out ( &block, QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_5 );
	// construct the header
	p_header hdr;
	hdr.command = PJR_SHOW_MAINSCREEN;
	hdr.length = 0;
	out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
	m_socket->write ( block );
}
