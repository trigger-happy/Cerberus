#ifndef XML_TEST_H
#define XML_TEST_H
#include <QtTest/QtTest>
#include <QObject>
#include "util/xml_util.h"

class XmlTest : public QObject
{
        Q_OBJECT;
private slots:
        void q1ReadTest();
        void q1WriteTest();
        void a1ReadTest();
        void a1WriteTest();
        void clientConfReadTest();
        void serverConfReadTest();
private:
        XmlUtil m_util;
};

#endif //XML_TEST_H
