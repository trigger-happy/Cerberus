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
#include "sql_test.h"

void SqlTest::initTestCase()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.init ( "resources/test.db" ) );
}

void SqlTest::addTeamTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.addTeam ( "test_team", "test_school" ) );
}

void SqlTest::addUserTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        UserData ud;
        ud.user_name = "user";
        ud.password = "pass";
        ud.firstname = "fname";
        ud.lastname = "lname";
        ud.teamname = "test_team";
        QVERIFY ( sql.addUser ( ud ) );
}

void SqlTest::getUsersTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        UserData ud;
        ud.user_name = "user";
        ud.password = QCryptographicHash::hash("pass", QCryptographicHash::Sha1);
        ud.firstname = "fname";
        ud.lastname = "lname";
        ud.teamname = "test_team";
        vector<UserData> v_ud;
        bool result = sql.getUsers ( v_ud );
        QVERIFY ( result );
        result = false;
        for ( int i = 0; i < v_ud.size(); i++ ) {
                if ( ud.user_name == v_ud[i].user_name
                     && ud.password == v_ud[i].password
                     && ud.firstname == v_ud[i].firstname
                     && ud.lastname == v_ud[i].lastname
                     && ud.teamname == v_ud[i].teamname ) {
                        result = true;
                }
        }
        QVERIFY ( result );
}

void SqlTest::setScoreTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        sql.setScore ( "user", 100 );
}

void SqlTest::getScoreTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.getScore ( "user" ) == 100 );
}

void SqlTest::addAdminTest()
{
}

void SqlTest::getTeamsTest()
{
}

void SqlTest::getScoresTest()
{
}

void SqlTest::getAdminsTest()
{
}

void SqlTest::verifyDBTest()
{
}

void SqlTest::authenticateTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.authenticate ( "user",
                                     QCryptographicHash::hash ( "pass", QCryptographicHash::Sha1 ) ) );
        QVERIFY ( !sql.authenticate ( "user",
                                      QCryptographicHash::hash ( "notpass", QCryptographicHash::Sha1 ) ) );
}

QTEST_MAIN ( SqlTest )
