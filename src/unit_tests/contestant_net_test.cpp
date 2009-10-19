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
#include <QFile>
#include "contestant_net_test.h"

CCNetTest::CCNetTest ( QObject* parent ) : QObject ( parent ), m_server ( this ), m_contestant ( this )
{
        QVERIFY ( m_contestant.getState() == CCS_DISCONNECTED );
}

void CCNetTest::connectionTest()
{
        QString ip = "localhost";
        quint16 port = 2652;
        //have the server listen
        m_server.listen ( port );
        //attempt to connect
        QSignalSpy spy ( &m_contestant, SIGNAL ( onConnect() ) );
        m_contestant.connectToHost ( ip, port );
        QVERIFY ( m_contestant.getState() == CCS_STANDBY );
        QVERIFY ( spy.count() == 1 );
}

void CCNetTest::authenticationTest()
{
        QString user = "trigger-happy";
        QString pw = "the_password";
        QSignalSpy spy ( &m_contestant, SIGNAL ( onAuthenticate ( bool ) ) );
        m_contestant.authenticate ( user, pw );
        QVERIFY ( spy.count() == 1 );
        QList<QVariant> args = spy.takeFirst();
        QVERIFY ( args.at ( 0 ).toBool() == true );
}

void CCNetTest::R1QDataTest()
{
        QString xml;
        QSignalSpy spy ( &m_contestant, SIGNAL ( onR1QData ( QString ) ) );
        m_contestant.QDataRequest();
        QVERIFY ( spy.count() == 1 );
        QList<QVariant> args = spy.takeFirst();
        xml = args.at ( 0 ).toString();
        QVERIFY ( xml.size() > 0 );
}

void CCNetTest::R1ADataTest()
{
        QFile f ( "resources/stage1_a.xml" );
        QString xml = f.readAll();
        QSignalSpy spy ( &m_contestant, SIGNAL ( onR1AData ( bool ) ) );
        m_contestant.r1ADataSend ( xml );
        QVERIFY ( spy.count() == 1 );
        QList<QVariant> args = spy.takeFirst();
        bool result = args.at ( 0 ).toBool();
        QVERIFY ( result );
}

QTEST_MAIN ( CCNetTest );
