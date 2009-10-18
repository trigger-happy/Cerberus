/*
Copyright (C) 2009 Michael Ybanez

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
#ifndef CLIENT_H
#define CLIENT_H
#include <QtGui>
#include <QtNetwork>
#include "net/contestant_net.h"

namespace Ui
{
class client_dlg;
}

class ClientDlg : public QDialog
{
        Q_OBJECT;
public:
        ClientDlg ( QWidget* parent = 0 );
        ~ClientDlg();
private slots:
        void onAuthReply ( bool result );
        void onConnectBtn();
        void onConnect();
        void onError ( QAbstractSocket::SocketError error );
        void onR1QData ( const QString& xml );
        void onR1AData ( bool result );
        void onContestStateChange ( int c );
private:
        ContestantNetwork* m_net;
        Ui::client_dlg* m_dlg;
};

#endif
