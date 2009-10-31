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
#ifndef SERVER_H
#define SERVER_H
#include <QtGui/QtGui>
#include <QtNetwork>
#include "net/server_net.h"

namespace Ui
{
class server_dlg;
}

class ServerNetwork;

class Server : public QObject
{
        Q_OBJECT;
public:
		Server ( QWidget* parent = 0 );
		~Server();
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

		void onAuthentication( ContestantConnection* cc, const QString& c_username );

		//Manual methods to be used by admin.
		/*
        Called when the stop button is pressed
        */
		void stopContest();

		void startContest();

		void pauseContest();

		void viewSubmittedAnswers();

		void checkAnswersManually ();

		void dropConnection( ContestantConnection* cc );



private:
		QString log, m_db_path;
		quint16 m_port;
		ServerNetwork* m_network;
		ServerConfig m_config;
		vector<QString> m_questions;
		vector<QString> m_answers;

};

#endif //SERVER_H
