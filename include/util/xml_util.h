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
#include <stdexcept>

/*!
\brief Handles the reading/writing of Xml data.
The XmlUtil class provides a utility interface for dealing with Xml data.
\note It does not actually deal with files, it simply handles xml data in QStrings.
*/
class XmlUtil : public Singleton<XmlUtil>
{
public:

	class XmlException : public std::runtime_error {
		public:
		const QString message;
		const qint64 lineNumber, columnNumber, characterOffset;
		XmlException(const QString &message, qint64 lineNumber, qint64 columnNumber, qint64 characterOffset )
				: runtime_error(message.toStdString()), message(message), lineNumber(lineNumber), columnNumber(columnNumber), characterOffset(characterOffset)
		{}
		const char* what() const throw() {
			static const QByteArray ret = QString("Error: %1 at line %2 column %3").
										  arg(message, QString::number(lineNumber), QString::number(columnNumber)).
										  toUtf8();
			return ret.data();
		}
		~XmlException() throw(){}
	};

	class IllFormedXmlException : public XmlException {
		IllFormedXmlException(const QString &message, qint64 lineNumber, qint64 columnNumber, qint64 characterOffset )
				: XmlException(message, lineNumber, columnNumber, characterOffset)
		{}
	};

	class InvalidXmlException : public XmlException {
		InvalidXmlException(const QString &message, qint64 lineNumber, qint64 columnNumber, qint64 characterOffset )
				: XmlException(message, lineNumber, columnNumber, characterOffset)
		{}
	};

	static const char * const CONFIG_ROOT_TAG;

        /*!
        */
        bool readQuestionData ( int round, const QString& xml, QuestionData& qd );

        /*!
        */
        bool writeQuestionData ( int round, const QuestionData& qd, QString& xml );

        /*!
        Read the client config from xml.
        \param xml Xml data in a QString.
        \param conf Reference to a ClientConfig struct to fill up.
        \return true on succes, false on failure.
        */
        bool readClientConfig ( const QString& xml, ClientConfig& conf );

        /*!
        Write the client config to xml.
        \param conf The ClientConfig to save to xml.
        \param xml Reference to QString where the xml data will be saved.
        \return true on success, false on failure.
        */
        bool writeClientConfig ( const ClientConfig& conf, QString& xml );

        /*!
		Read the network config from an xml string.
        \param xml Xml data in a QString.
        \param conf Reference to a NetworkConfig struct to fill up.
		\throws IllFormedXmlException
        */
		void readNetConfig ( const QString& xml, NetworkConfig& conf );

		/*!
		Read the network config from xml stream.
		\param xml The xml stream, should be positioned to the 'config' element.
		\param conf Reference to a NetworkConfig struct to fill up.
		\throws std::invalid_argument The xml stream is not positioned to the 'config' element.
		\throws IllFormedXmlException
		*/
		void readNetConfig( QXmlStreamReader& xml, NetworkConfig& config);
        /*!
        Write the network config to xml.
        \param conf The NetworkConfig struct to save to xml.
        \param xml Reference to QString where the xml data will be saved.
        \return true on success, false on failure.
        */
        bool writeNetConfig ( const NetworkConfig& conf, QString& xml );

        /*!
        Read the server config from xml.
        \param xml Xml data in a QString.
        \param conf Reference to a ServerConfig struct to fill up.
        \return true on success, false on failure.
        */
        bool readServerConfig ( const QString& xml, ServerConfig& conf );

        /*!
        Write the server config to xml.
        \param conf The ServerConfig struct to save to xml.
        \param xml Reference to QString where the xml data will be saved.
        \return true on success, false on failure.
        */
        bool writeServerConfig ( const ServerConfig& conf, QString& xml );
private:
        // round 1 related stuff
        bool readR1QData ( const QString& xml, QuestionData& data );
        bool writeR1QData ( const QuestionData& data, QString& xml );
        bool readR1AData ( const QString& xml, AnswerData& data );
        bool writeR1AData ( const AnswerData& data, QString& xml );

        // round 2 related stuff
        bool readR2QData ( const QString& xml, QuestionData& data );
        bool writeR2QData ( const QuestionData& data, QString& xml );
        bool readR2AData ( const QString& xml, AnswerData& data );
        bool writeR2AData ( const AnswerData& data, QString& xml );

        // round 3 and tie-breaker related stuff
        bool readR3QData ( const QString& xml, QuestionData& data );
        bool writeR3QData ( const QuestionData& data, QString& xml );
        bool readR3AData ( const QString& xml, AnswerData& data );
        bool writeR3AData ( const AnswerData& data, QString& xml );
protected:
};

#endif //XML_UTIL_H
