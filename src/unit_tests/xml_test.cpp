#include <QFile>
#include <QString>
#include "xml_test.h"

void XmlTest::q1ReadTest(){
	QFile file("resources/stage1_q.xml");
	QString xml = file.readAll();
	QVERIFY(xml.size() != 0);
	R1QData qd;
	R1QData td;
	qd.welcome_msg = "Insert welcome message here to be shown in the 1st screen.";
	R1Question qtemp;
	qtemp.question = "What is the bla bla bla?";
	qtemp.choices.insert(pair<int, string>(1, "Bla"));
	qtemp.choices.insert(pair<int, string>(2, "BLA"));
	qtemp.choices.insert(pair<int, string>(3, "bla"));
	qtemp.choices.insert(pair<int, string>(4, "blarg"));
	qd.questions.push_back(qtemp);
	qtemp.choices.clear();
	qtemp.question = "Who is Mr. Shires";
	qtemp.choices.insert(pair<int, string>(1, "Sir Diy"));
	qtemp.choices.insert(pair<int, string>(2, "Doc Mana"));
	qtemp.choices.insert(pair<int, string>(3, "Fr. David"));
	qtemp.choices.insert(pair<int, string>(4, "Michael Jackson"));
	qd.questions.push_back(qtemp);
	m_util.readR1QData(xml, td);
	QVERIFY(td == qd);
}

void XmlTest::q1WriteTest(){
}

void XmlTest::a1ReadTest(){
}

void XmlTest::a1WriteTest(){
}

void XmlTest::clientConfReadTest(){
}

void XmlTest::serverConfReadTest(){
}

QTEST_MAIN(XmlTest);