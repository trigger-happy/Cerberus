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
#ifndef SQL_TEST_H
#define SQL_TEST_H
#include <QtTest/QTest>
#include "util/sql_util.h"

class SqlTest : public QObject
{
        Q_OBJECT;
private slots:
        void initTestCase();
        void verifyDBTest();
        void addTeamTest();
        void addUserTest();
        void getUsersTest();
        void setScoreTest();
        void getScoreTest();
        void addAdminTest();
        void getTeamsTest();
        void getScoresTest();
        void getAdminsTest();
        void authenticateTest();
private:
        SqlUtil m_sqlutil;
};

#endif //SQL_TEST_H
