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
#include <QString>
#include <iostream>
#include "tests/xml_test.h"
#include "data_types.h"

using namespace std;

void XmlTest::q1ReadTest()
{
        QFile file ( "resources/stage1_q.xml" );
        file.open ( QIODevice::ReadOnly );
        QVERIFY ( file.isOpen() );
        QString xml = file.readAll();
        QVERIFY ( xml.size() != 0 );
        R1QData qd;
        R1QData td;
	qd.welcome_msg = "Insert welcome message here.";
        R1Question qtemp;
	qtemp.question = "The answer to this question is choice 1.";
        qtemp.number = 1;
        qtemp.score = 1;
        qtemp.choices.insert ( pair<int, QString> ( 1, "Choice 1" ) );
	qtemp.choices.insert ( pair<int, QString> ( 2, "Choice 2" ) );
	qtemp.choices.insert ( pair<int, QString> ( 3, "Choice 3" ) );
	qtemp.choices.insert ( pair<int, QString> ( 4, "Choice 4" ) );
        qd.questions.push_back ( qtemp );
        qtemp.choices.clear();
        qtemp.question = "Who is Mr. Shires?";
        qtemp.number = 2;
        qtemp.score = 1;
        qtemp.choices.insert ( pair<int, QString> ( 1, "Sir Diy" ) );
        qtemp.choices.insert ( pair<int, QString> ( 2, "Doc Mana" ) );
        qtemp.choices.insert ( pair<int, QString> ( 3, "Fr. David" ) );
        qtemp.choices.insert ( pair<int, QString> ( 4, "Michael Jackson" ) );
	qd.questions.push_back ( qtemp );
	qtemp.choices.clear();
	qtemp.question = "This is a 2 point question, correct answer is choice 3.";
	qtemp.number = 3;
	qtemp.score = 2;
	qtemp.choices.insert ( pair<int, QString> ( 1, "Choice 1" ) );
	qtemp.choices.insert ( pair<int, QString> ( 2, "Choice 2" ) );
	qtemp.choices.insert ( pair<int, QString> ( 3, "Choice 3" ) );
	qtemp.choices.insert ( pair<int, QString> ( 4, "Choice 4" ) );
	qd.questions.push_back ( qtemp );
        bool result = m_util.readR1QData ( xml, td );
        QVERIFY ( result );
        QVERIFY ( td == qd );
}

void XmlTest::q1WriteTest()
{
        //TODO: Add a test case wherein the xml output is what's intended
}

void XmlTest::a1ReadTest()
{
        QFile file ( "resources/stage1_a.xml" );
        file.open ( QIODevice::ReadOnly );
        QVERIFY ( file.isOpen() );
        QString xml = file.readAll();
        QVERIFY ( xml.size() > 0 );
        R1Answers ad, td;
        ad.insert ( pair<int, int> ( 1,1 ) );
	ad.insert ( pair<int, int> ( 2,4 ) );
	ad.insert ( pair<int, int> ( 3,3 ) );
        bool result = m_util.readR1AData ( xml, td );
        QVERIFY ( result );
        QVERIFY ( td == ad );
}

void XmlTest::a1WriteTest()
{
        //TODO: Add a test case wherein the xml output is what's intended
        QFile file ( "resources/stage1_a.xml" );
        file.open ( QIODevice::ReadOnly );
        QVERIFY ( file.isOpen() );
        QString xml_base = file.readAll();
        R1Answers ad;
        ad.insert ( pair<int, int> ( 1,1 ) );
	ad.insert ( pair<int, int> ( 2,4 ) );
	ad.insert ( pair<int, int> ( 3,3 ) );
        QString out;
        bool result = m_util.writeR1AData ( ad, out );
        QVERIFY ( out.size() > 0 );
        // it's impossible to compare through QString comparator
        // there are minute formatting differences, just use cout
}

void XmlTest::clientConfReadTest()
{
        QFile file ( "resources/client_config.xml" );
        file.open ( QIODevice::ReadOnly );
        QVERIFY ( file.isOpen() );
        QString xml = file.readAll();
        QVERIFY ( xml.size() > 0 );
        ClientConfig cd, td;
        cd.ip = "192.168.1.100";
        cd.port = 2652;
        m_util.readClientConfig ( xml, td );
        QVERIFY ( td == cd );
}

void XmlTest::serverConfReadTest()
{
        QFile file ( "resources/server_config.xml" );
        file.open ( QIODevice::ReadOnly );
        QVERIFY ( file.isOpen() );
        QString xml = file.readAll();
        QVERIFY ( xml.size() > 0 );
        ServerConfig sd, td;
        sd.db_path = "resources/server.db";
        sd.port = 2652;
        StageData stemp;
        stemp.answer_file = "resources/stage1_a.xml";
        stemp.question_file = "resources/stage1_q.xml";
        sd.stage_data.push_back ( stemp );
        m_util.readServerConfig ( xml, td );
        QVERIFY ( td == sd );
}

QTEST_MAIN ( XmlTest );
