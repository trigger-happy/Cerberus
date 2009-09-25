#include "util/xml_util.h"
#include "data_types.h"

bool XmlUtil::readR1QData(const QString& xml, R1QData& data){
        QXmlStreamReader reader(xml);

        while(!reader.atEnd()){
            QXmlStreamReader::TokenType token = reader.readNext();

            if(token == QXmlStreamReader::StartElement){
                if(reader.name() == "question_msg"){ // reads the question_msg tag
                    token = reader.readNext();
                    if(token == QXmlStreamReader::Characters){
                        data.welcome_msg = reader.text().toString(); // reads the characters within the question_msg tag
                    }else{
                        return false;
                    }
                    token = reader.readNext();
                }else if(reader.name() == "question"){ // reads the question tag
                    QXmlStreamAttributes attributes = reader.attributes();
                    R1Question temp;

                    // reads the number attribute from question
                    if(attributes.hasAttribute("number")){
                        temp.number =  attributes.value("number").toString().toInt();
                    }else{
                        return false;
                    }

                    // reads from the difficulty attribute from question
                    if(attributes.hasAttribute("difficulty")){
                        temp.number =  attributes.value("difficulty").toString().toInt();
                    }else{
                        return false;
                    }

                    for(int i = 0; i < 5; i++){
                        token = reader.readNext();
                        if(token == QXmlStreamReader::StartElement){
                            if(reader.name() == "q"){
                                token = reader.readNext();
                                //error check here
                                temp.question = reader.text().toString();
                                token = reader.readNext();
                            }else if(reader.name() == "choice"){
                                int number = reader.attributes().value("id").toString().toInt();
                                token = reader.readNext();
                                QString choice = reader.text().toString();
                                temp.choices[number] = choice;
                                token = reader.readNext();
                            }
                        }
                    }

                    token = reader.readNext();
                    data.questions.push_back(temp);
                }
            }
        }

	return true;
}

bool XmlUtil::writeR1QData(const R1QData& data, QString& xml){
        int counter = data.questions.size() - 1; // stores number of elements left
        int offset = 1;
        vector<R1Question> questions = data.questions;

        QXmlStreamWriter writer(&xml);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("stage1");
        writer.writeTextElement("welcome_msg", QString(data.welcome_msg));

        // this writes the questions into the xml
        while(counter != 0){
            QString questionNum = QString("%1").arg(data.questions.at(counter).number);
            QString difficultyNum = QString("%1").arg(data.questions.at(counter).difficulty);
            writer.writeStartElement("question");
                writer.writeAttribute("number", questionNum);
                writer.writeAttribute("difficulty", difficultyNum);

                writer.writeTextElement("q", QString(data.questions.at(counter).question));

                int choiceCounter = data.questions.at(counter).choices.size() - 1;
                while(choiceCounter != 0){
                    QString qAnswer(data.questions.at(counter).choices.find(choiceCounter)->second);
                    writer.writeTextElement("choice", qAnswer);
                    choiceCounter--;
                }
            writer.writeEndElement();// closes the question tag
            counter--;
        }

        writer.writeEndElement(); // closes the stage1 tag
        writer.writeEndDocument();

	return true;
}

bool XmlUtil::readR1AData(const QString& xml, R1Answers& data){
	return true;
}

bool XmlUtil::writeR1AData(const R1Answers& data, QString& xml){                
        int counter = data.size() - 1;
        QXmlStreamWriter writer(&xml);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        writer.writeStartElement("stage1_ans");
        for(int i = 0; i < counter; counter++){
            writer.writeTextElement("", "answer", QString(data.find(i)->second));
            writer.writeAttribute(QXmlStreamAttribute(QString("number"), QString("%1").arg(data.find(i)->first)));
        }

        writer.writeEndElement(); // closes the stage1_ans tag
        writer.writeEndDocument();

        return true;
}

bool XmlUtil::readClientConfig(const QString& xml, ClientConfig& conf){
	return true;
}

bool XmlUtil::readServerConfig(const QString& xml, ServerConfig& conf){
	return true;
}
