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

void SqlTest::getUserTest()
{
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

void SqlTest::authenticateTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.authenticate ( "user", "pass" ) );
        QVERIFY ( !sql.authenticate ( "user", "notpass" ) );
}

QTEST_MAIN ( SqlTest )
