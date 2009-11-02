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
#include "net/projector_connection.h"
#include "net/protocol.h"

ProjectorConnection::ProjectorConnection( QObject* parent, QTcpSocket* socket ) : QObject( parent ), m_socket( socket ) {
	m_socket = new QTcpSocket ( this );
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
}

void ProjectorConnection::showContestTime() {
}

void ProjectorConnection::showContestRanks() {
}

void ProjectorConnection::showQuestionTime() {
}

void ProjectorConnection::showAnswer() {
}
