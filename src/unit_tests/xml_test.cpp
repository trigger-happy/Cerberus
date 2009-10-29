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
	QFile file("resources/stage1.xml");
	QVERIFY(file.open(QIODevice::ReadOnly));
	QString xml = file.readAll();
	StageData qd, td;
	td.contest_time = 3600;
	td.welcome_msg = "Insert welcome message here.";
	Question q;
	q.type = Question::CHOOSE_ONE;

	q.score = 1;
	q.question = "The answer to this question is choice 1.";
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 1", true));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 2"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 3"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 4"));
	td.questions.push_back(q);

	q.score = 1;
	q.question = "Who is Mr. Shires?";
	q.answer_key.clear();
	q.answer_key.push_back(Question::AnswerKeyEntry("Sir Diy"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Doc Mana"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Fr. David"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Michael Jackson", true));
	q.type = Question::CHOOSE_ANY;
	td.questions.push_back(q);

	q.score = 2;
	q.question = "This is a 2 point question, correct answer is choice 3.";
	q.answer_key.clear();
	q.answer_key.push_back(Question::AnswerKeyEntry("3", true));
	q.type = Question::IDENTIFICATION;
	td.questions.push_back(q);
	xu.readStageData(xml, qd);
	QVERIFY(td == qd);
}

void XmlTest::q1WriteTest()
{
	XmlUtil& xu = XmlUtil::getInstance();
	QFile file("resources/stage1.xml");
	QVERIFY(file.open(QIODevice::ReadOnly));

	StageData qd;
	xu.readStageData(file.readAll(), qd);

	QString test_xml;
	xu.writeStageData(qd, test_xml);

	StageData td;
	xu.readStageData(test_xml, td);

	QVERIFY(qd == td);
}

void XmlTest::q2ReadTest()
{
	XmlUtil& xu = XmlUtil::getInstance();
	QFile file("resources/stage2.xml");
	QVERIFY(file.open(QIODevice::ReadOnly));
	QString xml = file.readAll();
	StageData qd, td;
	td.contest_time = 3600;
	td.welcome_msg = "Insert welcome message here.";
	Question q;

	q.type = Question::CHOOSE_ONE;
	q.score = 1;
	q.question = "The answer to this question is choice 1.";
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 1", true));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 2"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 3"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 4"));
	td.questions.push_back(q);

	q.score = 1;
	q.question = "Who is Mr. Shires?";
	q.answer_key.clear();
	q.answer_key.push_back(Question::AnswerKeyEntry("Sir Diy"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Doc Mana"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Fr. David"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Michael Jackson", true));
	td.questions.push_back(q);

	q.score = 2;
	q.question = "This is a 2 point question, correct answers are choice 3 and 4.";
	q.answer_key.clear();
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 1"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 2"));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 3", true));
	q.answer_key.push_back(Question::AnswerKeyEntry("Choice 4", true));
	q.type = Question::CHOOSE_ANY;
	td.questions.push_back(q);
	xu.readStageData(xml, qd);
	QVERIFY(qd == td);
}

void XmlTest::netConfReadTest()
{
	XmlUtil& xu = XmlUtil::getInstance();
	NetworkConfig tc, nc;
	tc.ip = "127.0.0.1";
	tc.port = 2652;
	QFile file("resources/net_config.xml");
	QVERIFY(file.open(QIODevice::ReadOnly));
	QString xml = file.readAll();
	try {
		xu.readNetConfig(xml, nc);
	} catch (XmlUtil::XmlException e) {
		cout << e.what() << endl;
	}
	QVERIFY(nc == tc);
}

void XmlTest::netConfWriteTest()
{
}

void XmlTest::serverConfReadTest()
{
}

QTEST_MAIN ( XmlTest );
