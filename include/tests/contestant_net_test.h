#ifndef CONTESTANT_NET_TEST_H
#define CONTESTANT_NET_TEST_H
#include <QtTest/QtTest>
#include "net/contestant_net.h"

class CCNetTest : public QObject{
	Q_OBJECT;
public:
	CCNetTest(QObject* parent = 0);
private slots:
	void connectionTest();
	void authenticationTest();
	void R1QDataTest();
	void R1ADataTest();
private:
	ContestantNetwork m_network;
};

#endif //CONTESTANT_NET_TEST_H