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
#include "server_app.h"
#include "ui_server_app.h"
#include "net/server_net.h"

ServerApp::ServerApp ( QWidget* parent ) : QDialog ( parent ), m_dlg ( new Ui::server_dlg )
{
        m_dlg->setupUi ( this );
        m_network = new ServerNetwork ( this );
}

ServerApp::~ServerApp()
{
        delete m_network;
}

void ServerApp::netNewConnection()
{
        //TODO: Stuff here for when a new connection is made
}

int main ( int argc, char* argv[] )
{
        QApplication a ( argc, argv );
        ServerApp server;
        server.show();
        return a.exec();
}
