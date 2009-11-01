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
#ifndef PROJECTOR_CONNECTION_H
#define PROJECTOR_CONNECTION_H
#include <QtNetwork>

class ProjectorConnection : public QObject {
	Q_OBJECT;

public:
	ProjectorConnection( QObject* parent = 0, QTcpSocket* socket = 0 );

public slots:
	/*!
	Called when there's a socket error.
	\param err The error
	*/
	void error ( const QAbstractSocket::SocketError& err );

	/*!
	Called when there's data ready in the socket for reading.
	*/
	void ready();

	/*!
	Called when the connection is disconnected by the client.
	*/
	void disconnected();

signals:
	/*!
	Emitted when the projector disconnects
	\param p Pointer to the ProjectorConnection that disconnected.
	*/
	void projectorDisconnect( ProjectorConnection* p );

private:
	QTcpSocket* m_socket;
};

#endif