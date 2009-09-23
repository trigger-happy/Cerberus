#include "tests/sql_test.h"

void SqlTest::initTestCase()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.init ( "resources/server.db" ) );
}

void SqlTest::addTeamTest()
{
        SqlUtil& sql = SqlUtil::getInstance();
        QVERIFY ( sql.addTeam ( "test_team", "test_team" ) );
}

void SqlTest::addUserTest()
{
}

void SqlTest::getUserTest()
{
}

void SqlTest::setScoreTest()
{
}

void SqlTest::authenticateTest()
{
}

QTEST_MAIN ( SqlTest );
