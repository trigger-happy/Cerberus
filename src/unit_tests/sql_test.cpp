#include "tests/sql_test.h"

void SqlTest::initTestCase()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.init ( "resources/server.db" ) );
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
        ud.user_name = 'user';
        ud.password = 'pass';
        ud.firstname = 'fname';
        ud.lastname = 'lname';
        ud.teamname = 'teamname';
        QVERIFY ( sql.addUser ( ud ) );
}

void SqlTest::getUserTest()
{
}

void SqlTest::setScoreTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        sql.setScore ( "mynameismanny", 100 );
}

void SqlTest::getScoreTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.getScore ( "mynameismanny" ) );
}

void SqlTest::authenticateTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.authenticate ( "mynameismanny", "money" ) );
}

QTEST_MAIN ( SqlTest )
