#include "tests/sql_test.h"

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
        QVERIFY ( sql.getScore ( "user" ) );
}

void SqlTest::authenticateTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
	QVERIFY ( sql.authenticate ( "user", "pass" ) );
	QVERIFY ( !sql.authenticate ( "user", "notpass" ) );
}

QTEST_MAIN ( SqlTest )
