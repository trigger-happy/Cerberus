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
#ifndef CONTESTANT_NET_TEST_H
#define CONTESTANT_NET_TEST_H
#include <QtTest/QtTest>
#include "net/server_net.h"
#include "net/contestant_net.h"

class CCNetTest : public QObject
{
        Q_OBJECT;
public:
        CCNetTest ( QObject* parent = 0 );
private slots:
        void connectionTest();
        void authenticationTest();
        void R1QDataTest();
        void R1ADataTest();
private:
        ContestantNetwork m_contestant;
        ServerNetwork m_server;
};

#endif //CONTESTANT_NET_TEST_H
