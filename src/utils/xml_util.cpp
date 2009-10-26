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
#include "util/xml_util.h"
#include "data_types.h"
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;

const char * const XmlUtil::CONFIG_ROOT_TAG = "config";

void XmlUtil::readQuestionData ( int round, const QString& xml, QuestionData& qd )
{
    switch (round) {
    case 1:
        readR1QData(xml, qd);
        break;
    case 2:
        readR2QData(xml, qd);
        break;
    case 3: // round 3 and 4 are identical anyway.
    case 4:
        readR3QData(xml, qd);
        break;
    default:
        // throw
        ;
    }
}

void XmlUtil::writeQuestionData(int round, const QuestionData& qd, QString& xml) {
    switch (round) {
    case 1:
        writeR1QData(qd, xml);
        break;
    case 2:
        writeR2QData(qd, xml);
        break;
    case 3:
    case 4:
        writeR3QData(qd, xml);
        break;
    default:
        //throw
        ;
    }
}

void XmlUtil::readAnswerData(int round, const QString& xml, AnswerData& ad) {
    switch (round) {
    case 1:
        readR1AData(xml, ad);
        break;
    case 2:
        readR2AData(xml, ad);
        break;
    case 3:
    case 4:
        readR3AData(xml, ad);
        break;
    default:
        // throw
        ;
    }
}

void XmlUtil::writeAnswerData(int round, const AnswerData& ad, QString& xml) {
    switch (round) {
    case 1:
        writeR1AData(ad, xml);
        break;
    case 2:
        writeR2AData(ad, xml);
        break;
    case 3:
    case 4:
        writeR3AData(ad, xml);
        break;
    default:
        // throw
        ;
    }
}

void XmlUtil::readR1QData ( const QString& xml, QuestionData& data )
{
    QXmlStreamReader reader ( xml );

    while ( !reader.atEnd() ) {
        QXmlStreamReader::TokenType token = reader.readNext();

        if ( token == QXmlStreamReader::StartElement ) {
            if ( reader.name() == "welcome_msg" ) { // reads the welcome_msg tag
                token = reader.readNext();
                if ( token == QXmlStreamReader::Characters ) {
                    data.welcome_msg = reader.text().toString(); // reads the characters within the question_msg tag
                } else {
                    // throw
                }
                token = reader.readNext();
            } else if ( reader.name() == "question" ) { // reads the question tag
                QXmlStreamAttributes attributes = reader.attributes();
                Question temp;

                // reads the number attribute from question
                if ( attributes.hasAttribute ( "number" ) ) {
                    temp.number =  attributes.value ( "number" ).toString().toInt();
                } else {
                    // throw
                }

                // reads from the score attribute from question
                if ( attributes.hasAttribute ( "score" ) ) {
                    temp.score =  attributes.value ( "score" ).toString().toInt();
                } else {
                    // throw
                }

                for ( int i = 0; i < 5; i++ ) {
                    token = reader.readNext();
                    if ( token == QXmlStreamReader::StartElement ) {
                        if ( reader.name() == "q" ) {
                            token = reader.readNext();
                            //error check here
                            temp.question = reader.text().toString();
                            token = reader.readNext();
                        } else if ( reader.name() == "choice" ) {
                            int number = reader.attributes().value ( "id" ).toString().toInt();
                            token = reader.readNext();
                            QString choice = reader.text().toString();
                            temp.choices[number] = choice;
                            token = reader.readNext();
                        }
                    } else {
                        // don't ask, it's a work around
                        i--;
                    }
                }

                token = reader.readNext();
                data.questions.push_back ( temp );
            } else if ( reader.name() == "stage1" ) {
                QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute("contest_time")) {
                    data.contest_time = attributes.value("contest_time").toString().toInt();
                }
            }
        }
    }
}

void XmlUtil::readR2QData(const QString& xml, QuestionData& data)
{
    QXmlStreamReader reader ( xml );

    while ( !reader.atEnd() ) {
        QXmlStreamReader::TokenType token = reader.readNext();

        if ( token == QXmlStreamReader::StartElement ) {
            if ( reader.name() == "welcome_msg" ) { // reads the welcome_msg tag
                token = reader.readNext();
                if ( token == QXmlStreamReader::Characters ) {
                    data.welcome_msg = reader.text().toString(); // reads the characters within the question_msg tag
                } else {
                    // throw
                }
                token = reader.readNext();
            } else if ( reader.name() == "question" ) { // reads the question tag
                QXmlStreamAttributes attributes = reader.attributes();
                Question temp;

                // reads the number attribute from question
                if ( attributes.hasAttribute ( "number" ) ) {
                    temp.number =  attributes.value ( "number" ).toString().toInt();
                } else {
                    // throw
                }

                // reads from the score attribute from question
                if ( attributes.hasAttribute ( "score" ) ) {
                    temp.score =  attributes.value ( "score" ).toString().toInt();
                } else {
                    // throw
                }

                for ( int i = 0; i < 5; i++ ) {
                    token = reader.readNext();
                    if ( token == QXmlStreamReader::StartElement ) {
                        if ( reader.name() == "q" ) {
                            token = reader.readNext();
                            //error check here
                            temp.question = reader.text().toString();
                            token = reader.readNext();
                        } else if ( reader.name() == "choice" ) {
                            int number = reader.attributes().value ( "id" ).toString().toInt();
                            token = reader.readNext();
                            QString choice = reader.text().toString();
                            temp.choices[number] = choice;
                            token = reader.readNext();
                        }
                    } else {
                        // don't ask, it's a work around
                        i--;
                    }
                }

                token = reader.readNext();
                data.questions.push_back ( temp );
            } else if ( reader.name() == "stage2" ) {
                QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute("contest_time")) {
                    data.contest_time = attributes.value("contest_time").toString().toInt();
                }
            }
        }
    }
}

void XmlUtil::writeR2QData(const QuestionData& qd, QString& xml) {
}

void XmlUtil::readR3QData(const QString& xml, QuestionData& qd) {
}

void XmlUtil::writeR3QData(const QuestionData& qd, QString& xml) {
}

void XmlUtil::writeR1QData ( const QuestionData& data, QString& xml )
{
    int counter = 0; // stores number of elements left
    int offset = 1;
    vector<Question> questions = data.questions;

    QXmlStreamWriter writer ( &xml );
    writer.setAutoFormatting ( true );
    writer.writeStartDocument();
    writer.writeStartElement ( "stage1" );
    writer.writeAttribute("contest_time", QString("%1").arg(data.contest_time));
    writer.writeTextElement ( "welcome_msg", QString ( data.welcome_msg ) );

    // this writes the questions into the xml
    while ( counter < data.questions.size() ) {
        QString questionNum = QString ( "%1" ).arg ( data.questions.at ( counter ).number );
        QString scoreNum = QString ( "%1" ).arg ( data.questions.at ( counter ).score );
        writer.writeStartElement ( "question" );
        writer.writeAttribute ( "number", questionNum );
        writer.writeAttribute ( "score", scoreNum );

        writer.writeTextElement ( "q", QString ( data.questions.at ( counter ).question ) );

        map<int,QString>::const_iterator iter = data.questions[counter].choices.begin();
        while (iter != data.questions[counter].choices.end()) {
            writer.writeStartElement("choice");
            writer.writeAttribute("id", QString("%1").arg(iter->first));
            writer.writeCharacters(iter->second);
            writer.writeEndElement();
            iter++;
        }
        writer.writeEndElement();// closes the question tag
        counter++;
    }

    writer.writeEndElement(); // closes the stage1 tag
    writer.writeEndDocument();
}

void XmlUtil::readR1AData ( const QString& xml, AnswerData& data )
{
    QXmlStreamReader reader ( xml );
    QXmlStreamReader::TokenType token = reader.readNext();

    while ( !reader.atEnd() ) {
        if ( token == QXmlStreamReader::StartElement ) {
            if ( reader.name() == "answer" ) { // checks for the answer tag
                QXmlStreamAttributes attribute = reader.attributes(); // extracts the attributes
                int number = attribute.value ( "number" ).toString().toInt(); // gets the number attribute
                token = reader.readNext();
                data.insert(pair<int,QString>(number, reader.text().toString())); // stores the number and answer into a map
            }

        }
        token = reader.readNext();
    }
}

void XmlUtil::readR2AData(const QString& xml, AnswerData& data) {
    readR1AData(xml, data);
}

void XmlUtil::readR3AData(const QString& xml, AnswerData& data) {
}

void XmlUtil::writeR1AData ( const AnswerData& data, QString& xml )
{
    int counter = data.size();
    QXmlStreamWriter writer ( &xml );
    writer.setAutoFormatting ( true );
    writer.writeStartDocument();

    writer.writeStartElement ( "stage1_ans" );
    AnswerData::const_iterator iter = data.begin();
    for ( ; iter != data.end(); iter++ ) {
        writer.writeStartElement ( "answer" );
        writer.writeAttribute ( QXmlStreamAttribute ( QString ( "number" ), QString ( "%1" ).arg ( iter->first ) ) );
        writer.writeCharacters ( QString ( "%1" ).arg ( iter->second ) );
        writer.writeEndElement();
    }

    writer.writeEndElement(); // closes the stage1_ans tag
    writer.writeEndDocument();
}

void XmlUtil::writeR2AData ( const AnswerData& data, QString& xml ) {
}

void XmlUtil::writeR3AData ( const AnswerData& data, QString& xml ) {
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
            if ( reader.name() == "ip" ) {
                config.ip = reader.readElementText();
            } else if ( reader.name() == "port" ) {
                config.port = reader.readElementText().toInt();
            } else {
                //Should I barf because of invalid tags in the conf or just
                //ignore this madness?
            }
        } else if ( reader.isEndElement() && reader.name() == CONFIG_ROOT_TAG )
            break;
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

StageData XmlUtil::readStageData(QXmlStreamReader& stream) {
    assert(stream.name() == "stage_data");
    StageData ret;
    while ( !stream.atEnd() ) {
        if ( stream.readNext() == QXmlStreamReader::StartElement ) {
            if ( stream.name() == "question" ) {
                ret.question_file = stream.readElementText();
            } else if ( stream.name() == "answer" ) {
                ret.answer_file = stream.readElementText();
            } else if ( stream.name() == "stage_data" ) {
                //I found me-self again!
                throw InvalidXmlException("'stage_data' tag found inside 'stage_data'.", stream);
            } else {
                //barf?
            }
        } else if ( stream.isEndElement() && stream.name() == "stage_data" )
            break;
    }
    return ret;
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
            } else if ( reader.name() == "stage_data" ) {
                conf.stage_data.push_back( readStageData(reader) );
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
    for ( size_t i = 0; i < conf.stage_data.size(); ++i ) {
        writer.writeStartElement("stage_data");
        const StageData &sd = conf.stage_data[i];
        writer.writeTextElement("question", sd.question_file);
        writer.writeTextElement("answer", sd.answer_file);
        writer.writeEndElement();
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
