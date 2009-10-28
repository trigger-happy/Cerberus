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
#include <cassert>
#include <iostream>
#include "net/contestant_net.h"

using namespace std;

ContestantNetwork::ContestantNetwork ( QObject* parent ) : QObject ( parent )
{
        m_socket = new QTcpSocket ( this );
        connect ( m_socket, SIGNAL ( connected() ), this, SLOT ( connected() ) );
        connect ( m_socket, SIGNAL ( disconnected() ), this, SLOT ( disconnected() ) );
        connect ( m_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
                  this, SLOT ( error ( QAbstractSocket::SocketError ) ) );
        connect ( m_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
                  this, SIGNAL ( onError ( QAbstractSocket::SocketError ) ) );
        connect ( m_socket, SIGNAL ( readyRead() ), this, SLOT ( ready() ) );
        connect ( m_socket, SIGNAL ( disconnected() ), this, SIGNAL ( onDisconnect() ) );
        m_authenticated = false;
        m_hdr = NULL;
}

ContestantNetwork::~ContestantNetwork()
{
        delete m_socket;
}

void ContestantNetwork::connectToHost ( const QString& ip, quint16 port )
{
        m_socket->connectToHost ( ip, port );
}

void ContestantNetwork::disconnectFromHost()
{
        m_socket->disconnectFromHost();
}

bool ContestantNetwork::authenticate ( const QString& user_name, const QString& pw )
{
        if ( !m_socket->isWritable() ) {
                return false;
        }
        if ( m_authenticated ) {
                return false;
        }
        // construct an authentication packet
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        p_header hdr;
        hdr.command = QRY_AUTHENTICATE;

        // construct the payload
        QString data = user_name + "," + QCryptographicHash::hash ( pw.toAscii(), QCryptographicHash::Sha1 );
        hdr.length = data.size();
        // write it
        out.writeRawData ( ( const char* ) &hdr, sizeof ( hdr ) );
        out << data;

        // send it
        m_socket->write ( block );
        return true;
}

bool ContestantNetwork::qDataRequest ( int round )
{
        if ( !m_socket->isWritable() ) {
                return false;
        }

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
        return true;
}

bool ContestantNetwork::aDataSend ( const QString& xml )
{
        if ( !m_socket->isWritable() ) {
                return false;
        }
        // construct the packet
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        p_header hdr;
        hdr.command = QRY_ANSWER_SUBMIT;

        // construct the payload
        QByteArray hash = QCryptographicHash::hash ( xml.toAscii(), QCryptographicHash::Sha1 );
        hdr.length = hash.size() + xml.size();
        // write it
        out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
        out.writeRawData ( hash.data(), hash.size() );
        out << xml;

        // send it
        m_socket->write ( block );
        return true;
}

void ContestantNetwork::connected()
{
        // identify ourselves
        QByteArray block;
        QDataStream out ( &block, QIODevice::WriteOnly );
        out.setVersion ( QDataStream::Qt_4_5 );
        // construct the header
        p_header hdr;
        hdr.length = sizeof ( uchar );
        hdr.command = NET_INITIATE_CONNECTION;

        out.writeRawData ( ( const char* ) &hdr, sizeof ( p_header ) );
        out << ( uchar ) CLIENT_CONTESTANT;

        m_socket->write ( block );
}

void ContestantNetwork::getContestState()
{
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

void ContestantNetwork::disconnected()
{
        m_authenticated = false;
}

void ContestantNetwork::error ( const QAbstractSocket::SocketError& err )
{
        // TODO: do something better here
        cerr << "Error " << err << endl;
}

void ContestantNetwork::ready()
{
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
                emit onContestStateChange ( round, ( CONTEST_STATUS ) status );
        }
        break;
        case INF_AUTHENTICATE:
                //it's a reply to the authentication
        {
                uchar result;
                in >> result;
                emit onAuthenticate ( result );
                m_authenticated = result;
        }
        break;
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
                        emit onQData ( xml );
                } else {
                        // TODO: act on invalid data
                }
        }
        break;
        case INF_ANSWER_REPLY:
                //a reply on our submission
        {
                uchar result;
                in >> result;
                emit onAData ( result );
        }
        break;
        case INF_ERROR:
                // server said there's an error
        {
                uchar err;
                in >> err;
                emit onContestError ( ( ERROR_MESSAGES ) err );
        }
        break;
        case INF_CONTEST_TIME:
                // the contest time has changed
        {
                ushort time;
                in >> time;
                emit onContestTime ( time );
        }
        break;
        case INF_QUESTION_STATE:
                // change in round 3/4 question state
        {
                ushort qnum;
                ushort time;
                ushort status;
                in >> qnum >> time >> status;
                emit onQuestionStateChange ( qnum, time, ( QUESTION_STATUS ) status );
        }
        break;
        default:
                // TODO: we need to handle this more gracefully
                cout << m_hdr->command << endl;
                assert ( false );
        }
        delete m_hdr;
        m_hdr = NULL;
        if ( m_socket->bytesAvailable() > 0 ) {
                ready();
        }
}

bool ContestantNetwork::getContestTime()
{
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
