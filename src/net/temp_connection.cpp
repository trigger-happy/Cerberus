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
        //read the socket data
	quint16 blocksize = 0;
        QDataStream in ( m_socket );
        in.setVersion ( QDataStream::Qt_4_5 );
        if ( blocksize == 0 ) {
                if ( m_socket->bytesAvailable() < ( int ) sizeof ( quint16 ) ) {
                        return;
                }
                in >> blocksize;
        }

        if ( m_socket->bytesAvailable() < blocksize ) {
                return;
        }

        // check what command was sent by the server and react accordingly
        quint16 command;
	in >> command;
        if ( command == NET_INITIATE_CONNECTION ) {
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
