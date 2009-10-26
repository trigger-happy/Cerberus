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
#ifndef SERVER_APP_H
#define SERVER_APP_H
#include <QtGui/QtGui>
#include <QtNetwork>
#include "net/server_net.h"

namespace Ui
{
class server_dlg;
}

class ServerNetwork;

class ServerApp : public QDialog
{
        Q_OBJECT;
public:
        ServerApp ( QWidget* parent = 0 );
        ~ServerApp();
private slots:
        /*!
        Called when there is a new connection
        */
		void newContestant( ContestantConnection* cc );

		/*
		Called when a client connects but is not a Cerberus client.
		  */
		void badClient ( TempConnection* tc );
		/*
		Called when there is a disconnection
		*/
		void contestantDisconnect( ContestantConnection* cc );

        /*
        Called when the stop button is pressed
        */
        void stop();
private:
		QString log;
        ServerNetwork* m_network;
        Ui::server_dlg* m_dlg;
		vector<QString> m_questions;

};

#endif //SERVER_APP_H
