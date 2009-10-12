/*
Copyright (C) 2009 Victor Tanedo

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
#ifndef XML_UTIL_H
#define XML_UTIL_H
#include <QtXml/QtXml>
#include "data_types.h"
#include "patterns/singleton.h"

class XmlUtil : public QObject, public Singleton<XmlUtil>
{
        Q_OBJECT;
public:
        /*!
        Parse the round 1 question data.
        \param xml The xml data in a QString.
        \param data A reference to the R1QData struct to be filled up.
        \return true on success, false on failure
        */
        bool readR1QData ( const QString& xml, R1QData& data );

        /*!
        Serialize the round 1 question data into an xml format.
        \param data R1QData struct to serialize.
        \param xml A reference to a QString to contain the xml data.
        \return true on success, false on failure
        */
        bool writeR1QData ( const R1QData& data, QString& xml );

        /*!
        Parse the round 1 answer data.
        \param xml The xml data in a QString.
        \param data The R1Answers struct to fill up.
        \return true on success, false on failure.
        */
        bool readR1AData ( const QString& xml, R1Answers& data );

        /*!
        Serialize the round 1 answer data into an xml format.
        \param data R1Answers struct to serialize.
        \param xml Reference to QString to contain the xml data.
        \return true on success, false on failure.
        */
        bool writeR1AData ( const R1Answers& data, QString& xml );

        /*!
        Read the client config from xml.
        \param xml Xml data in a QString.
        \param conf Reference to a ClientConfig struct to fill up.
        \return true on succes, false on failure.
        */
        bool readClientConfig ( const QString& xml, ClientConfig& conf );

        /*!
        Read the server config from xml.
        \param xml Xml data in a QString.
        \param conf Reference to a ServerConfig struct to fill up.
        \return true on success, false on failure.
        */
        bool readServerConfig ( const QString& xml, ServerConfig& conf );
protected:
};

#endif //XML_UTIL_H
