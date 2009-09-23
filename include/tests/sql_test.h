#ifndef SQL_TEST_H
#define SQL_TEST_H
#include <QtTest/QTest>
#include "util/sql_util.h"

class SqlTest : public QObject{
	Q_OBJECT;
private slots:
	void initTestCase();
	void addTeamTest();
	void addUserTest();
	void getUserTest();
	void setScoreTest();
	void authenticateTest();
private:
	SqlUtil m_sqlutil;
};

#endif //SQL_TEST_H