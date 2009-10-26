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
#include "xml_test.h"
#include "data_types.h"

using namespace std;

void XmlTest::q1ReadTest()
{
    XmlUtil& xu = XmlUtil::getInstance();
    QFile file("resources/stage1_q.xml");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString xml = file.readAll();
    QuestionData qd, td;
    td.contest_time = 3600;
    td.welcome_msg = "Insert welcome message here.";
    Question q;
    
    q.number = 1;
    q.score = 1;
    q.question = "The answer to this question is choice 1.";
    q.choices.insert(pair<int,QString>(1, "Choice 1"));
    q.choices.insert(pair<int,QString>(2, "Choice 2"));
    q.choices.insert(pair<int,QString>(3, "Choice 3"));
    q.choices.insert(pair<int,QString>(4, "Choice 4"));
    td.questions.push_back(q);
    
    q.number = 2;
    q.score = 1;
    q.question = "Who is Mr. Shires?";
    q.choices.insert(pair<int,QString>(1, "Sir Diy"));
    q.choices.insert(pair<int,QString>(2, "Doc Mana"));
    q.choices.insert(pair<int,QString>(3, "Fr. David"));
    q.choices.insert(pair<int,QString>(4, "Michael Jackson"));
    td.questions.push_back(q);
    
    q.number = 3;
    q.score = 2;
    q.question = "This is a 2 point question, correct answer is choice 3.";
    q.choices.insert(pair<int,QString>(1, "Choice 1"));
    q.choices.insert(pair<int,QString>(2, "Choice 2"));
    q.choices.insert(pair<int,QString>(3, "Choice 3"));
    q.choices.insert(pair<int,QString>(4, "Choice 4"));
    td.questions.push_back(q);
    xu.readQuestionData(1, xml, qd);
    QVERIFY(td == qd);
}

void XmlTest::q1WriteTest()
{
}

void XmlTest::q2ReadTest()
{
}

void XmlTest::q2WriteTest() {
}

void XmlTest::q3ReadTest() {
}

void XmlTest::q3WriteTest() {
}

void XmlTest::a1ReadTest()
{
    XmlUtil& xu = XmlUtil::getInstance();
    AnswerData ad, td;
    QFile file("resources/stage1_a.xml");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QString xml = file.readAll();
    td.insert(pair<int, QString>(1, "1"));
    td.insert(pair<int, QString>(2, "4"));
    td.insert(pair<int, QString>(3, "3"));
    xu.readAnswerData(1, xml, ad);
    QVERIFY(ad == td);
}

void XmlTest::a1WriteTest()
{
}

void XmlTest::a2ReadTest() {
}

void XmlTest::a2WriteTest() {
}

void XmlTest::a3ReadTest() {
}

void XmlTest::a3WriteTest() {
}

void XmlTest::netConfReadTest()
{
}

void XmlTest::serverConfReadTest()
{
}

QTEST_MAIN ( XmlTest );
