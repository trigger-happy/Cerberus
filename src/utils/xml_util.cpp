#include "xml_util.h"
#include "data_types.h"

bool XmlUtil::readR1QData(const QString& xml, R1QData& data){
        QXmlStreamReader reader(xml);

	return true;
}

bool XmlUtil::writeR1QData(const R1QData& data, QString& xml){
        int counter = data.questions.size(); // stores number of elements left
        vector<R1Question> questions = data.questions;

        QXmlStreamWriter writer(&xml);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("firstroundquestions");

        while(counter != 0){            
            R1Question q = data.questions.at(counter - 1);
            writer.writeStartElement("questions");
            writer.writeStartElement("text");
            writer.writeText(q.question);// writes the question
            writer.writeEndElement();// closes the text tag
            writer.writeStartElement("answers");

            int choiceCounter = q.choices.size();
            while(choiceCounter != 0){
                writer.writeCharacters(q.choices.find(choiceCounter));
                choiceCounter--;
            }

            writer.writeEndElement();// closes the answers tag
            writer.writeEndElement();// closes the questions tag
            counter--;
        }

        writer.writeEndElement(); // closes the firstroundquestions tag

	return true;
}

bool XmlUtil::readR1AData(const QString& xml, R1Answers& data){
	return true;
}

bool XmlUtil::writeR1AData(const R1Answers& data, QString& xml){
        int counter = data.questions.size(); // stores number of elements left
        vector<R1Question> questions = data.questions;

        QXmlStreamWriter writer(&xml);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("firstroundquestions");

        while(counter != 0){
            R1Question q = data.questions.at(counter - 1);
            writer.writeStartElement("questions");
            writer.writeStartElement("text");
            writer.writeText(q.question);// writes the question
            writer.writeEndElement();// closes the text tag
            writer.writeStartElement("answers");

            int choiceCounter = q.choices.size();
            while(choiceCounter != 0){
                writer.writeCharacters(q.choices.find(choiceCounter));
                choiceCounter--;
            }

            writer.writeEndElement();// closes the answers tag
            writer.writeEndElement();// closes the questions tag
            counter--;
        }

        writer.writeEndElement(); // closes the firstroundquestions tag
}

bool XmlUtil::readClientConfig(const QString& xml, ClientConfig& conf){
	return true;
}

bool XmlUtil::readServerConfig(const QString& xml, ServerConfig& conf){
	return true;
}
