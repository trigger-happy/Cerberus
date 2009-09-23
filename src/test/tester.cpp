#include "tester.h"

void Tester::tcp_test()
{
        m_socket = new QTcpSocket ( this );
        QVERIFY ( !m_socket->isOpen() );
        m_socket->connectToHost ( QString ( "localhost" ), 21 );
        m_socket->waitForConnected();
        QVERIFY ( m_socket->isOpen() );
        delete m_socket;
}

QTEST_MAIN ( Tester )
