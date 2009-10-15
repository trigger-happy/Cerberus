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

using namespace std;

bool XmlUtil::readR1QData ( const QString& xml, R1QData& data )
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
                                        return false;
                                }
                                token = reader.readNext();
                        } else if ( reader.name() == "question" ) { // reads the question tag
                                QXmlStreamAttributes attributes = reader.attributes();
                                R1Question temp;

                                // reads the number attribute from question
                                if ( attributes.hasAttribute ( "number" ) ) {
                                        temp.number =  attributes.value ( "number" ).toString().toInt();
                                } else {
                                        return false;
                                }

                                // reads from the score attribute from question
                                if ( attributes.hasAttribute ( "score" ) ) {
                                        temp.score =  attributes.value ( "score" ).toString().toInt();
                                } else {
                                        return false;
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
                                        }
                                }

                                token = reader.readNext();
                                data.questions.push_back ( temp );
                        }
                }
        }

        return true;
}

bool XmlUtil::writeR1QData ( const R1QData& data, QString& xml )
{
        int counter = data.questions.size() - 1; // stores number of elements left
        int offset = 1;
        vector<R1Question> questions = data.questions;

        QXmlStreamWriter writer ( &xml );
        writer.setAutoFormatting ( true );
        writer.writeStartDocument();
        writer.writeStartElement ( "stage1" );
        writer.writeTextElement ( "welcome_msg", QString ( data.welcome_msg ) );

        // this writes the questions into the xml
        while ( counter != 0 ) {
                QString questionNum = QString ( "%1" ).arg ( data.questions.at ( counter ).number );
                QString scoreNum = QString ( "%1" ).arg ( data.questions.at ( counter ).score );
                writer.writeStartElement ( "question" );
                writer.writeAttribute ( "number", questionNum );
                writer.writeAttribute ( "score", scoreNum );

                writer.writeTextElement ( "q", QString ( data.questions.at ( counter ).question ) );

                int choiceCounter = data.questions.at ( counter ).choices.size() - 1;
                while ( choiceCounter != 0 ) {
                        QString qAnswer ( data.questions.at ( counter ).choices.find ( choiceCounter )->second );
                        writer.writeTextElement ( "choice", qAnswer );
                        choiceCounter--;
                }
                writer.writeEndElement();// closes the question tag
                counter--;
        }

        writer.writeEndElement(); // closes the stage1 tag
        writer.writeEndDocument();

        return true;
}

bool XmlUtil::readR1AData ( const QString& xml, R1Answers& data )
{
        QXmlStreamReader reader ( xml );
        QXmlStreamReader::TokenType token = reader.readNext();

        while ( !reader.atEnd() ) {
                if ( token == QXmlStreamReader::StartElement ) {
                        if ( reader.name() == "answer" ) { // checks for the answer tag
                                QXmlStreamAttributes attribute = reader.attributes(); // extracts the attributes
                                int number = attribute.value ( "number" ).toString().toInt(); // gets the number attribute
                                token = reader.readNext();
                                int answer = reader.text().toString().toInt(); // gets the number of the answer
                                data[number] = answer; // stores the number and answer into a map
                        }

                }
                token = reader.readNext();
        }
        return true;
}

bool XmlUtil::writeR1AData ( const R1Answers& data, QString& xml )
{
        int counter = data.size();
        QXmlStreamWriter writer ( &xml );
        writer.setAutoFormatting ( true );
        writer.writeStartDocument();

        writer.writeStartElement ( "stage1_ans" );
        R1Answers::const_iterator iter = data.begin();
        for ( ; iter != data.end(); iter++ ) {
                writer.writeStartElement ( "answer" );
                writer.writeAttribute ( QXmlStreamAttribute ( QString ( "number" ), QString ( "%1" ).arg ( iter->first ) ) );
                writer.writeCharacters ( QString ( "%1" ).arg ( iter->second ) );
                writer.writeEndElement();
        }

        writer.writeEndElement(); // closes the stage1_ans tag
        writer.writeEndDocument();

        return true;
}

bool XmlUtil::readClientConfig ( const QString& xml, ClientConfig& conf )
{
        QXmlStreamReader reader ( xml );
        while ( !reader.atEnd() ) {
                QXmlStreamReader::TokenType token = reader.readNext();
                if ( token == QXmlStreamReader::StartElement ) {
                        if ( reader.name() == "config" ) { // checks for the config tag
                                token = reader.readNext();
                                token = reader.readNext();// moves to the ip tag
                                token = reader.readNext();
                                QString ip = reader.text().toString(); // reads the ip address
                                token = reader.readNext();
                                token = reader.readNext(); // moves to the port tag
                                token = reader.readNext();
                                token = reader.readNext(); // moves to the port text
                                int port = reader.text().toString().toInt(); // reads the text from the port
                                conf.ip = ip; // stores ip into data
                                conf.port = port; // stores port into data port
                        }
                }

        }
        return true;
}

bool XmlUtil::readServerConfig ( const QString& xml, ServerConfig& conf )
{
        QXmlStreamReader reader ( xml );
        while ( !reader.atEnd() ) {
                QXmlStreamReader::TokenType token = reader.readNext();
                if ( token == QXmlStreamReader::StartElement ) {
                        if ( reader.name() == "config" ) {
                                token = reader.readNext();
                                token = reader.readNext(); // move token to the port tag
                                token = reader.readNext();
                                int port = reader.text().toString().toInt(); // read the port value
                                token = reader.readNext();
                                token = reader.readNext();// move token to the db tag
                                token = reader.readNext();
                                token = reader.readNext();
                                QString db = reader.text().toString(); // read the db value
                                token = reader.readNext(); // exit the db tag
                                token = reader.readNext();
                                token = reader.readNext(); // move token to stage_data tag

                                QXmlStreamAttributes attributes = reader.attributes(); // get the attributes
                                int round = attributes.value ( "round" ).toString().toInt(); // gets the attribute round from stage_data

                                token = reader.readNext(); // move token to question_data
                                token = reader.readNext();
                                token = reader.readNext(); // move token to question_data value
                                QString questions = reader.text().toString(); // read the questions value
                                token = reader.readNext(); // move token to answer_data tag
                                token = reader.readNext();
                                token = reader.readNext();
                                token = reader.readNext(); // move token to answer_data value
                                QString answers = reader.text().toString(); //read the answers value

                                StageData data;
                                data.question_file = questions;
                                data.answer_file = answers;

                                conf.db_path = db;
                                conf.port = port;

                                conf.stage_data.push_back ( data );
                        }
                }
        }
        return true;
}
