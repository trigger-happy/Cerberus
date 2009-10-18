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
#include "net/contestant_connection.h"
#include "util/sql_util.h"
#include "error_defs.h"

ContestantConnection::ContestantConnection ( QObject* parent, QTcpSocket* socket ) : QObject ( parent ), m_socket ( socket )
{
        m_blocksize = 0;
        m_authenticated = false;
        connect ( m_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
                  this, SLOT ( error ( QAbstractSocket::SocketError ) ) );
        connect ( m_socket, SIGNAL ( readyRead() ), this, SLOT ( ready() ) );
        connect ( m_socket, SIGNAL ( disconnected() ), this, SLOT ( disconnected() ) );
        // TODO: write a reply to the client here
}

void ContestantConnection::error ( const QAbstractSocket::SocketError& err )
{
        //do something about the error here
}

void ContestantConnection::ready()
{
        //read the socket data
        QDataStream in ( m_socket );
        in.setVersion ( QDataStream::Qt_4_5 );
        if ( m_blocksize == 0 ) {
                if ( m_socket->bytesAvailable() < ( int ) sizeof ( quint16 ) ) {
                        return;
                }
                in >> m_blocksize;
        }

        if ( m_socket->bytesAvailable() < m_blocksize ) {
                return;
        }

        // check what command was sent by the server and react accordingly
        quint16 command;
        in >> command;
        switch ( command ) {
        case QRY_CONTEST_STATE:
                //contestant is asking for the contest state.
                if ( m_authenticated ) {
                        //send a reply
                } else {
                        //send an error
                        errorReply ( ERR_NOTAUTHORIZED );
                }
                break;
        case QRY_AUTHENTICATE:
                //contestant wants to authenticate
                if ( !m_authenticated ) {
                        QString buffer, user, pass;
                        in >> buffer;
                        user = buffer.section ( ",", 0, 0 );
                        pass = buffer.section ( ",", 1, 1 );
                        bool result = SqlUtil::getInstance().authenticate ( user, pass );
                        authenticationReply ( result );
                } else {
                        //TODO: what happens here?
                        authenticationReply ( false );
                }
                break;
        case QRY_QUESTION_REQUEST:
                //contestant is asking for question data
                if ( m_authenticated ) {
                        // TODO: check for what round and send the relevant data here
                } else {
                        //send an error
                        errorReply ( ERR_NOTAUTHORIZED );
                }
                break;
        case QRY_ANSWER_SUBMIT:
                //contestant has submitted their answers
                if ( m_authenticated ) {
                        QString buffer;
                        in >> buffer;
                        //TODO: have the buffer contents sent and checked
                        //let's reply with something for now
                        sendR1AReply ( true );
                } else {
                        errorReply ( ERR_NOTAUTHORIZED );
                }
                break;
        default:
                ;
                // invalid command, do something here
        }
        m_blocksize = 0;
}

void ContestantConnection::errorReply ( ERROR_MESSAGES err )
{
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        out << ( quint16 ) 0 << ( quint16 ) 100;
        out << err;
        out.device()->seek ( 0 );
        out << ( quint16 ) ( block.size()-sizeof ( quint16 ) );
        m_socket->write ( block );
}

void ContestantConnection::disconnected()
{
        //do something about our disconnection here
}

void ContestantConnection::authenticationReply ( bool res )
{
        //construct the packet and send it
        m_authenticated = res;
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        out << ( quint16 ) 0 << ( quint16 ) INF_AUTHENTICATE;
        out << res;
        out.device()->seek ( 0 );
        out << ( quint16 ) ( block.size()-sizeof ( quint16 ) );
        m_socket->write ( block );
}

void ContestantConnection::sendR1QData ( const QString& xml )
{
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        out << ( quint16 ) 0 << ( quint16 ) INF_QUESTION_DATA;
        // TODO: insert round number here
        out << xml;
        out.device()->seek ( 0 );
        out << ( quint16 ) ( block.size()-sizeof ( quint16 ) );
        m_socket->write ( block );
}

void ContestantConnection::sendR1AReply ( bool res )
{
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        out << ( quint16 ) 0 << ( quint16 ) INF_ANSWER_REPLY;
        out << res;
        out.device()->seek ( 0 );
        out << ( quint16 ) ( block.size()-sizeof ( quint16 ) );
        m_socket->write ( block );
}

void ContestantConnection::sendContestState ( quint16 state )
{
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        out << ( quint16 ) 0 << ( quint16 ) INF_CONTEST_STATE;
        out << state;
        out.device()->seek ( 0 );
        out << ( quint16 ) ( block.size()-sizeof ( quint16 ) );
        m_socket->write ( block );
}

void ContestantConnection::setR1QData ( const QString* xml )
{
        m_r1qdata = xml;
}
