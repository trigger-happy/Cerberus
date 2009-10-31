/*
Copyright (C) 2009 Wilhansen Li

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
#include "util/xml_util.h"
#include "data_types.h"
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;

const char * const XmlUtil::CONFIG_ROOT_TAG = "config";
const char * const XmlUtil::XML_NS = "http://cerberus.compsat.org";

template <typename T>
T checked_convert(const QString &str, const char *key, const QXmlStreamReader &reader) {
	throw std::logic_error("Unimplemented type passed to checked_convert.");
}

template<>
bool checked_convert(const QString &str, const char *key, const QXmlStreamReader &reader) {
	const QString &trm = str.trimmed();

	if ( trm.compare("true", Qt::CaseInsensitive) == 0 || trm == "1" )
		return true;
	else if ( trm.compare("false", Qt::CaseInsensitive) == 0 || trm == "0")
		return false;

	throw XmlUtil::InvalidXmlException(
			QString("Invalid value for '%1' (expected 'true' or 'false')").arg(key),
			reader);
}

template<>
int checked_convert(const QString &str, const char *key, const QXmlStreamReader &reader) {
	bool ok = true;
	const int val = str.toInt(&ok);
	if ( !ok )
		throw XmlUtil::InvalidXmlException(
				QString("Invalid value for '%1' (expected integer)").arg(key),
				reader);
	return val;
}

template<>
unsigned int checked_convert(const QString &str, const char *key, const QXmlStreamReader &reader) {
	bool ok = true;
	const unsigned int val = str.toUInt(&ok);
	if ( !ok )
		throw XmlUtil::InvalidXmlException(
				QString("Invalid value for '%1' (expected non-negative integer)").arg(key),
				reader);
	return val;
}

template<>
QString checked_convert(const QString &str, const char *key, const QXmlStreamReader &reader) {
	return str;
}

template <typename T>
bool optional_assign(const QXmlStreamAttributes &attr, const char *key, T &v, const QXmlStreamReader &reader) {
	if ( attr.hasAttribute(XmlUtil::XML_NS, key) ) {
		v = checked_convert<T>(attr.value(XmlUtil::XML_NS, key).toString(), key, reader);
		return true;
	} else if ( attr.hasAttribute("", key) ) {
		v = checked_convert<T>(attr.value("", key).toString(), key, reader);
		return true;
	}
	return false;
}

template <typename T>
T checked_assign(const QXmlStreamAttributes &attr, const char * key, const QXmlStreamReader &reader) {
	T tmp;
	if ( optional_assign<T>(attr, key, &tmp) )
		return tmp;

	throw XmlUtil::InvalidXmlException(
			QString("Required attribute '%1' missing in element %2.").
			arg(key, reader.name().toString()),
			reader);
}

Question XmlUtil::parseChooseQuestion(QXmlStreamReader &reader) {
	assert(reader.name() == "choose");

	const QXmlStreamAttributes &attrs = reader.attributes();
	Question q;
	q.type = Question::CHOOSE_ONE;
	optional_assign(attrs, "score", q.score, reader);
	optional_assign(attrs, "time_limit", q.time_limit, reader);
	optional_assign(attrs, "id", q.id, reader);

	{
		bool multiple = false;
		optional_assign(attrs, "multiple", multiple, reader);
		if ( multiple )
			q.type = Question::CHOOSE_ANY;
	}

	while ( !reader.atEnd() ) {
		reader.readNext();
		{
			const QStringRef &ns = reader.namespaceUri();
			if ( !ns.isEmpty() && ns != XML_NS ) continue;
		}
		if ( reader.isStartElement() ) {
			if ( reader.name() == "q" ) {
				q.question = reader.readElementText();
			} else if ( reader.name() == "choice" )  {
				bool is_answer = false;
				optional_assign(reader.attributes(), "answer", is_answer, reader);
				q.answer_key.push_back(Question::AnswerKeyEntry(reader.readElementText(), is_answer));
			} else if ( reader.name() == "choose" ) {
				throw InvalidXmlException("'choose' tag found inside 'choose'.", reader);
			} else {
				//barf?
			}
		} else if ( reader.isEndElement() && reader.name() == "choose" )
			break;
	}
	return q;
}

Question XmlUtil::parseIdentificationQuestion(QXmlStreamReader &reader) {
	assert(reader.name() == "identification");
	const QXmlStreamAttributes &attrs = reader.attributes();

	Question q;
	q.type = Question::IDENTIFICATION;
	optional_assign(attrs, "score", q.score, reader);
	optional_assign(attrs, "time_limit", q.time_limit, reader);
	optional_assign(attrs, "id", q.id, reader);

	while ( !reader.atEnd() ) {
		reader.readNext();
		{
			const QStringRef &ns = reader.namespaceUri();
			if ( !ns.isEmpty() && ns != XML_NS ) continue;
		}
		if ( reader.isStartElement() ) {
			if ( reader.name() == "q" ) {
				q.question = reader.readElementText();
			} else if ( reader.name() == "a" )  {
				q.answer_key.push_back(Question::AnswerKeyEntry(reader.readElementText(), true));
			} else if ( reader.name() == "identification" ) {
				throw InvalidXmlException("'identification' tag found inside 'identification'.", reader);
			} else {
				//barf?
			}
		} else if ( reader.isEndElement() && reader.name() == "identification" )
			break;
	}
	return q;
}

void XmlUtil::readStageData(const QString &xml, StageData &qd) {
	QXmlStreamReader reader(xml);

	while ( !reader.atEnd() ) {
		if ( reader.readNext() == QXmlStreamReader::StartElement ) {
			if ( !reader.namespaceUri().isEmpty() && reader.namespaceUri() != XML_NS ) continue;

			if ( reader.name() == "stage" ) {
				const QXmlStreamAttributes &attrs = reader.attributes();
				optional_assign(attrs, "time_limit", qd.contest_time, reader);
			} else if ( reader.name() == "welcome_msg" ) {
				qd.welcome_msg = reader.readElementText();
			} else if ( reader.name() == "choose" ) {
				qd.questions.push_back(parseChooseQuestion(reader));
			} else if ( reader.name() == "identification" ) {
				qd.questions.push_back(parseIdentificationQuestion(reader));
			}
		}
	}

	if ( reader.hasError() )
		throw IllFormedXmlException(reader.errorString(), reader);
}

void XmlUtil::writeStageData(const StageData &qd, QString &output) {
	QXmlStreamWriter writer(&output);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("stage");
	writer.writeDefaultNamespace(XML_NS);
	writer.writeNamespace("http://www.w3.org/2001/XMLSchema-instance", "xsi");
	writer.writeAttribute("http://www.w3.org/2001/XMLSchema-instance", "schemaLocation",
						  "http://cerberus.compsat.org stage.xsd");

	if ( qd.hasContestTime() )
		writer.writeAttribute("time_limit", QString::number(qd.contest_time));


	writer.writeTextElement(XML_NS, "welcome_msg", qd.welcome_msg);
	for ( size_t i = 0; i < qd.questions.size(); ++i ) {
		const Question &q = qd.questions[i];
		switch ( q.type ) {
			case Question::IDENTIFICATION:
				writer.writeStartElement(XML_NS, "identification");
				break;
			case Question::CHOOSE_ANY:
			case Question::CHOOSE_ONE:
				writer.writeStartElement(XML_NS, "choose");
				if ( q.type == Question::CHOOSE_ANY )
					writer.writeAttribute("multiple", "true");
				break;
		}
		writer.writeAttribute("score", QString::number(q.score));
		if ( q.hasTimeLimit() )
			writer.writeAttribute("time_limit", QString::number(q.time_limit));
		if ( q.hasId() )
			writer.writeAttribute("id", q.id);
		writer.writeTextElement(XML_NS, "q", q.question);

		switch ( q.type ) {
			case Question::IDENTIFICATION:
				for ( size_t j = 0; j < q.answer_key.size(); ++j ) {
					if ( !q.answer_key[j].is_answer ) continue;
					writer.writeTextElement(XML_NS, "a", q.answer_key[j].c);
				}
				break;
			case Question::CHOOSE_ANY:
			case Question::CHOOSE_ONE:
				for ( size_t j = 0; j < q.answer_key.size(); ++j ) {
					writer.writeStartElement(XML_NS, "choice");
					if ( q.answer_key[j].is_answer )
						writer.writeAttribute("answer", "true");
					writer.writeCharacters(q.answer_key[j].c);
					writer.writeEndElement();
				}
				break;
		}

		writer.writeEndElement();
	}

	writer.writeEndElement();
	writer.writeEndDocument();
}

#include <QDomDocument>
QString XmlUtil::stripAnswers(const QString &input) {
	QDomDocument doc;
	doc.setContent(input);
	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	while ( !n.isNull() ) {
		QDomElement e = n.toElement();
		if ( !e.isNull() && (e.namespaceURI().isEmpty() || e.namespaceURI() == XML_NS) ) {
			if ( e.nodeName() == "choose" ) {
				QDomElement c = e.firstChildElement("choice");
				while ( !c.isNull() ) {
					c.removeAttribute("answer");
					c = c.nextSiblingElement("choice");
				}
			} else if ( e.nodeName() == "identification" ) {
				QDomElement a = e.firstChildElement("a");
				while ( !a.isNull() ) {
					e.removeChild(a);
					a = e.firstChildElement("a");
				}
			}
		}
		n = n.nextSibling();
	}
	return doc.toString(2);
}

void XmlUtil::readProjectorConfig(const QString &xml, ProjectorConfig &conf) {
	QXmlStreamReader reader( xml );
	while ( !reader.atEnd() ) {
		if ( reader.readNext() == QXmlStreamReader::StartElement) {
			if ( reader.name() == CONFIG_ROOT_TAG ) {
				break;
			}
		}
	}

	if ( reader.name() != CONFIG_ROOT_TAG )
		throw InvalidXmlException("No config root tag found.", reader);

	while ( !reader.atEnd() ) {
		reader.readNext();
		if ( reader.isStartElement() ) {
			if ( reader.name() == "theme_path" ) {
				conf.theme_path = reader.readElementText();
			} else if ( reader.name() == "contest_name" ) {
				conf.contest_name = reader.readElementText();
			} else {
				readNetConfigElement(reader, conf);
			}
		} else if ( reader.isEndElement() && reader.name() == CONFIG_ROOT_TAG )
			break;
	}

	if ( reader.hasError() )
		throw IllFormedXmlException(reader.errorString(), reader);
}

void XmlUtil::readClientConfig ( const QString& xml, ClientConfig& conf )
{
	readNetConfig(xml, conf);
}

void XmlUtil::writeClientConfig ( const ClientConfig& conf, QString& xml ) {
	writeNetConfig(conf, xml);
}

void XmlUtil::readNetConfig( QXmlStreamReader& reader, NetworkConfig& config) {
	if ( !reader.isStartElement() || reader.name() != CONFIG_ROOT_TAG )
		throw std::invalid_argument("Passed XML does not start with 'config' element.");

	while ( !reader.atEnd() ) {
		if ( reader.readNext() == QXmlStreamReader::StartElement ) {
			readNetConfigElement(reader, config);
		} else if ( reader.isEndElement() && reader.name() == CONFIG_ROOT_TAG )
			break;
	}
}

void XmlUtil::readNetConfigElement(QXmlStreamReader &reader, NetworkConfig &config) {
	if ( !reader.isStartElement() ) return;

	if ( reader.name() == "ip" ) {
		config.ip = reader.readElementText();
	} else if ( reader.name() == "port" ) {
		config.port = reader.readElementText().toInt();
	} else {
		//Should I barf because of invalid tags in the conf or just
		//ignore this madness?
	}
}

void XmlUtil::readNetConfig ( const QString& xml, NetworkConfig& conf )
{
	QXmlStreamReader reader( xml );
	while ( !reader.atEnd() ) {
		if ( reader.readNext() == QXmlStreamReader::StartElement) {
			if ( reader.name() == CONFIG_ROOT_TAG ) {
				break;
			}
		}
	}

	if ( reader.name() != CONFIG_ROOT_TAG )
		throw InvalidXmlException("No config root tag found.",
								  reader.lineNumber(), reader.columnNumber(), reader.characterOffset());
	readNetConfig(reader, conf);

	if ( reader.hasError() )
		throw IllFormedXmlException(reader.errorString(),
									reader.lineNumber(), reader.columnNumber(), reader.characterOffset());
}

void XmlUtil::writeNetConfig(const NetworkConfig &conf, QString &xml) {
	QXmlStreamWriter writer(&xml);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("config");
	{
		writer.writeTextElement("ip", QString("%1").arg(conf.ip));
		writer.writeTextElement("port", QString("%1").arg(conf.port));
	}
	writer.writeEndElement();
	writer.writeEndDocument();
}

void XmlUtil::readServerConfig ( const QString& xml, ServerConfig& conf )
{
	QXmlStreamReader reader ( xml );

	while ( !reader.atEnd() ) {
		if ( reader.readNext() == QXmlStreamReader::StartElement )
			break;
	}

	if ( reader.name() != CONFIG_ROOT_TAG )
		throw InvalidXmlException("No config root tag found.",
								  reader.lineNumber(), reader.columnNumber(), reader.characterOffset());

	while ( !reader.atEnd() ) {
		if ( reader.readNext() == QXmlStreamReader::StartElement ) {
			if ( reader.name() == "port" ) {
				bool ok;
				conf.port = reader.readElementText().toInt(&ok);
				if ( !ok || conf.port > (1 << 16 - 1) )
					throw InvalidXmlException("Invalid server port specified.",
											  reader.lineNumber(), reader.columnNumber(), reader.characterOffset());
			} else if ( reader.name() == "db" ) {
				conf.db_path = reader.readElementText();
			} else if ( reader.name() == "stage_file" ) {
				conf.stage_files.push_back(reader.readElementText());
			} else {
				//barf?
			}
		}
	}

	if ( reader.hasError() )
		throw IllFormedXmlException(reader.errorString(), reader);
}

void XmlUtil::writeServerConfig ( const ServerConfig& conf, QString& xml ) {
	QXmlStreamWriter writer( &xml );
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement(CONFIG_ROOT_TAG);
	writer.writeTextElement("port", QString::number(conf.port));
	writer.writeTextElement("db", conf.db_path);
	for ( size_t i = 0; i < conf.stage_files.size(); ++i ) {
		writer.writeTextElement("stage_file", conf.stage_files[i]);
	}
	writer.writeEndElement();
	writer.writeEndDocument();
}
void XmlUtil::readAdminConfig(const QString &xml, AdminConfig &conf) {
	readNetConfig(xml, conf);
}

void XmlUtil::writeAdminConfig(const AdminConfig &conf, QString &xml) {
	writeNetConfig(conf, xml);
}
