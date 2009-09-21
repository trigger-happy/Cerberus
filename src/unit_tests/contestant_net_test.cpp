#include <QFile>
#include "tests/contestant_net_test.h"

CCNetTest::CCNetTest(QObject* parent) : QObject(parent), m_server(this){
	QVERIFY(m_contestant.getState() == CCS_DISCONNECTED);
}

void CCNetTest::connectionTest(){
	QString ip = "127.0.0.1";
	int port = 2652;
	QSignalSpy spy(&m_contestant, SIGNAL(connected()));
	m_contestant.connectToHost(ip, port);
	QVERIFY(spy.count() == 1);
	QVERIFY(m_contestant.getState() == CCS_STANDBY);
}

void CCNetTest::authenticationTest(){
	QString user = "trigger-happy";
	QString pw = "the_password";
	QSignalSpy spy(&m_contestant, SIGNAL(onAuthenticate(bool)));
	m_contestant.authenticate(user, pw);
	QVERIFY(spy.count() == 1);
	QList<QVariant> args = spy.takeFirst();
	QVERIFY(args.at(0).toBool() == true);
}

void CCNetTest::R1QDataTest(){
	QString xml;
	QSignalSpy spy(&m_contestant, SIGNAL(onR1QData(QString)));
	m_contestant.r1QDataRequest();
	QVERIFY(spy.count() == 1);
	QList<QVariant> args = spy.takeFirst();
	xml = args.at(0).toString();
	QVERIFY(xml.size() > 0);
}

void CCNetTest::R1ADataTest(){
	QFile f("resources/stage1_a.xml");
	QString xml = f.readAll();
	QSignalSpy spy(&m_contestant, SIGNAL(onR1AData(bool)));
	m_contestant.r1ADataSend(xml);
	QVERIFY(spy.count() == 1);
	QList<QVariant> args = spy.takeFirst();
	bool result = args.at(0).toBool();
	QVERIFY(result);
}

QTEST_MAIN(CCNetTest);