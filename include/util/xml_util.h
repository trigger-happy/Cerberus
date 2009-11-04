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
		XmlException(const QString& message, const QXmlStreamReader& stream) :
				runtime_error(message.toStdString()), message(message),
				lineNumber(stream.lineNumber()), columnNumber(stream.columnNumber()), characterOffset(stream.characterOffset()) {}

		const char* what() const throw() {
			static const QByteArray ret = QString("Error: %1 at line %2 column %3").
										  arg(message, QString::number(lineNumber), QString::number(columnNumber)).
										  toUtf8();
			return ret.data();
		}
		~XmlException() throw() {}
	};

	class IllFormedXmlException : public XmlException {
	public:
		IllFormedXmlException(const QString &message, qint64 lineNumber, qint64 columnNumber, qint64 characterOffset ) :
				XmlException(message, lineNumber, columnNumber, characterOffset)
		{}
		IllFormedXmlException(const QString& message, const QXmlStreamReader& stream) :
				XmlException(message, stream) {}
	};

	class InvalidXmlException : public XmlException {
	public:
		InvalidXmlException(const QString &message, qint64 lineNumber, qint64 columnNumber, qint64 characterOffset )
				: XmlException(message, lineNumber, columnNumber, characterOffset)
		{}
		InvalidXmlException(const QString& message, const QXmlStreamReader& stream) :
				XmlException(message, stream) {}
	};

	static const char * const CONFIG_ROOT_TAG;
	static const char * const XML_NS;

	/*!
	Parse a stage XML data and fill the QuestionData struct with the question and answer data.
	\param xml QString containing the xml data.
	\param qd Reference to a QuestionData struct to fill the question data with.
			  The caller is responsible for clearing the structure first before passing it.
	\throws XmlException
	*/
	void readStageData(const QString &xml, StageData &qd);

	/*!

	*/
	void writeStageData(const StageData &qd, QString &output);

	/*!
	Strips the answers from the stage XML data. Use for sending question data to clients.
	\param input The stage data for input
	\return The same stage data with its answers stripped.
	*/
	QString stripAnswers(const QString &input);

	/*!
	Read the client config from xml.
	\param xml Xml data in a QString.
	\param conf Reference to a ClientConfig struct to fill up.
	\throws XmlException
	*/
	void readClientConfig ( const QString& xml, ClientConfig& conf );

	/*!
	Write the client config to xml.
	\param conf The ClientConfig to save to xml.
	\param xml Reference to QString where the xml data will be saved.
	*/
	void writeClientConfig ( const ClientConfig& conf, QString& xml );

	/*!
	Read the network config from an xml string.
	\param xml Xml data in a QString.
	\param conf Reference to a NetworkConfig struct to fill up.
	\throws XmlException
	*/
	void readNetConfig ( const QString& xml, NetworkConfig& conf );

	/*!
	Write the network config to xml.
	\param conf The NetworkConfig struct to save to xml.
	\param xml Reference to QString where the xml data will be saved.
	*/
	void writeNetConfig ( const NetworkConfig& conf, QString& xml );

	/*!
	Read the server config from xml.
	\param xml Xml data in a QString.
	\param conf Reference to a ServerConfig struct to fill up.
	\throws XmlException
	*/
	void readServerConfig ( const QString& xml, ServerConfig& conf );

	/*!
	Write the server config to xml.
	\param conf The ServerConfig struct to save to xml.
	\param xml Reference to QString where the xml data will be appended.
	*/
	void writeServerConfig ( const ServerConfig& conf, QString& xml );

	void readAdminConfig(const QString &xml, AdminConfig &conf);
	void writeAdminConfig(const AdminConfig &conf, QString &xml);

	void readProjectorConfig(const QString &xml, ProjectorConfig &conf);
private:
	static Question parseChooseQuestion(QXmlStreamReader &reader);
	static Question parseIdentificationQuestion(QXmlStreamReader &reader);
	static StageData readStageData(QXmlStreamReader& stream);
	static void readNetConfig ( QXmlStreamReader& reader, NetworkConfig& conf );
	static void readNetConfigElement(QXmlStreamReader &reader, NetworkConfig &conf);
	static void readAuthorModeScoreEntry(QXmlStreamReader &reader, ProjectorConfig::AuthorMode &am);
	static ProjectorConfig::AuthorMode* readAuthorMode(QXmlStreamReader &reader);
protected:
};

#endif //XML_UTIL_H
