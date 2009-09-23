#ifndef TESTER_H
#define TESTER_H
#include <QtTest/QTest>
#include <QtNetwork>
#include <QObject>

class Tester : public QObject
{
        Q_OBJECT;
private slots:
        void tcp_test();
private:
        QTcpSocket* m_socket;
};

#endif //TESTER_H
