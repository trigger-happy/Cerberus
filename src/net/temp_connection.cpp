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
#include "net/temp_connection.h"
#include "net/protocol.h"

TempConnection::TempConnection ( QObject* parent, QTcpSocket* socket ) : QObject ( parent ), m_socket ( socket )
{
        connect ( m_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
                  this, SLOT ( error ( QAbstractSocket::SocketError ) ) );
        connect ( m_socket, SIGNAL ( readyRead() ), this, SLOT ( ready() ) );
        connect ( m_socket, SIGNAL ( disconnected() ), this, SLOT ( disconnected() ) );
}

void TempConnection::error ( const QAbstractSocket::SocketError& error )
{
        // TODO: handle the error here
}

void TempConnection::ready()
{
        p_header hdr;
        if ( m_socket->bytesAvailable() < ( int ) sizeof ( hdr ) ) {
                return;
        }
        QDataStream in ( m_socket );
        in.setVersion ( QDataStream::Qt_4_5 );
        in.readRawData ( ( char* ) &hdr, sizeof ( p_header ) );
        //check the packet
        if ( strcmp ( ( const char* ) &hdr.ident.data, "CERB" ) != 0 ) {
                // bad packet, do something here
                return;
        }
        //check the version
        if ( !is_proto_current ( hdr.ver ) ) {
                // the version is not the same, do something here
        }
        if ( m_socket->bytesAvailable() < hdr.length ) {
                return;
        }

        if ( hdr.command == NET_INITIATE_CONNECTION ) {
                quint16 client_type;
                in >> client_type;
                emit newClient ( this, static_cast<CLIENT_ID> ( client_type ) );
        } else {
                // drop the connection
                //m_socket->disconnectFromHost();
                emit invalidClient ( this );
        }
}

void TempConnection::disconnected()
{
        emit invalidClient ( this );
}
