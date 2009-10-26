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
#ifndef XML_TEST_H
#define XML_TEST_H
#include <QtTest/QtTest>
#include <QObject>
#include "util/xml_util.h"

class XmlTest : public QObject
{
    Q_OBJECT;
private slots:
    // question test
    void q1ReadTest();
    void q1WriteTest();
    void q2ReadTest();
    void q2WriteTest();
    void q3ReadTest();
    void q3WriteTest();
    // answer test
    void a1ReadTest();
    void a1WriteTest();
    void a2ReadTest();
    void a2WriteTest();
    void a3ReadTest();
    void a3WriteTest();
    // configuration test
    void netConfReadTest();
    void netConfWriteTest();
    void serverConfReadTest();
private:
    XmlUtil m_util;
};

#endif //XML_TEST_H
