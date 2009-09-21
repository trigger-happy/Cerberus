#ifndef CONTESTANT_NET_TEST_H
#define CONTESTANT_NET_TEST_H
#include <QtTest/QtTest>
#include "net/contestant_net.h"

class CCNetTest : public QObject{
	Q_OBJECT;
private slots:
	void init();
	void connectionTest();
	void authenticationTest();
	void R1QDataTest();
	void R1ADataTest();
private:
	ContestantNetwork m_network;
};

#endif //CONTESTANT_NET_TEST_H