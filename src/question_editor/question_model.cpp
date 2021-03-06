/*
Copyright (C) 2009 Ken Lee

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

#include "iostream"
#include "question_model.h"

#define CHOICES 4
#define ENTRIES 5

using namespace std;

QuestionModel::QuestionModel(int round) : QStandardItemModel(0,10)
{
	this->round=round;
	//super(9,2);
}

void QuestionModel::addNewQuestion()
{
	/*questionmodel order
	question ansa ansb ansc ansd anse anskey score time type 
	*/
	QList<QStandardItem *> temp;
	temp.append(new QStandardItem("new question")); //question
	temp.append(new QStandardItem("")); // choice a
	temp.append(new QStandardItem("")); // choice b
	temp.append(new QStandardItem("")); // choice c
	temp.append(new QStandardItem("")); // choice d
	temp.append(new QStandardItem("")); // choice e
	if (round>1)
		temp.append(new QStandardItem("00000")); // answerkey
	else
		temp.append(new QStandardItem("10000")); 
	temp.append(new QStandardItem("1")); // score
	temp.append(new QStandardItem("0")); // time
	temp.append(new QStandardItem("0")); // isIdentification
	
	QStandardItemModel::appendRow(temp);
	
}

void QuestionModel::removeQuestion(int index)
{
	QStandardItemModel::removeRow(index);
}

void QuestionModel::updateQuestion(int index,QString question,QString a,QString b,QString c,QString d,QString e,QString anskey,QString score,QString time,QString type)
{
	QStandardItemModel::item(index,0)->setText(question.trimmed());
	QStandardItemModel::item(index,1)->setText(a.trimmed());
	QStandardItemModel::item(index,2)->setText(b.trimmed());
	QStandardItemModel::item(index,3)->setText(c.trimmed());
	QStandardItemModel::item(index,4)->setText(d.trimmed());
	QStandardItemModel::item(index,5)->setText(e.trimmed());
	QStandardItemModel::item(index,6)->setText(anskey.trimmed());
	QStandardItemModel::item(index,7)->setText(score.trimmed());
	QStandardItemModel::item(index,8)->setText(time.trimmed());
	QStandardItemModel::item(index,9)->setText(type.trimmed());
}

void QuestionModel::swapOrder(int q1,int q2)
{
	QString question=getQuestion(q1);
	QString a=getA(q1);
	QString b=getB(q1);
	QString c=getC(q1);
	QString d=getD(q1);
	QString e=getE(q1);
	QString anskey=QStandardItemModel::item(q1,6)->text();
	QString score=QStandardItemModel::item(q1,7)->text();
	QString time=QStandardItemModel::item(q1,8)->text();
	QString type=QStandardItemModel::item(q1,9)->text();
	
	QStandardItemModel::item(q1,0)->setText(getQuestion(q2));
	QStandardItemModel::item(q1,1)->setText(getA(q2));
	QStandardItemModel::item(q1,2)->setText(getB(q2));
	QStandardItemModel::item(q1,3)->setText(getC(q2));
	QStandardItemModel::item(q1,4)->setText(getD(q2));
	QStandardItemModel::item(q1,5)->setText(getE(q2));
	QStandardItemModel::item(q1,6)->setText(QStandardItemModel::item(q2,6)->text());
	QStandardItemModel::item(q1,7)->setText(QStandardItemModel::item(q2,7)->text());
	QStandardItemModel::item(q1,8)->setText(QStandardItemModel::item(q2,8)->text());
	QStandardItemModel::item(q1,9)->setText(QStandardItemModel::item(q2,9)->text());
	
	QStandardItemModel::item(q2,0)->setText(question);
	QStandardItemModel::item(q2,1)->setText(a);
	QStandardItemModel::item(q2,2)->setText(b);
	QStandardItemModel::item(q2,3)->setText(c);
	QStandardItemModel::item(q2,4)->setText(d);
	QStandardItemModel::item(q2,5)->setText(e);
	QStandardItemModel::item(q2,6)->setText(anskey);
	QStandardItemModel::item(q2,7)->setText(score);
	QStandardItemModel::item(q2,8)->setText(time);
	QStandardItemModel::item(q2,9)->setText(type);
}

QString QuestionModel::getQuestion(int index)
{
	return QStandardItemModel::item(index,0)->text();
}
QString QuestionModel::getA(int index)
{
	return QStandardItemModel::item(index,1)->text();
}
QString QuestionModel::getB(int index)
{
	return QStandardItemModel::item(index,2)->text();
}
QString QuestionModel::getC(int index)
{
	return QStandardItemModel::item(index,3)->text();
}
QString QuestionModel::getD(int index)
{
	return QStandardItemModel::item(index,4)->text();
}
QString QuestionModel::getE(int index)
{
	return QStandardItemModel::item(index,5)->text();
}
void QuestionModel::getAnskey(int index,bool* temp)
{
	//bool* temp=new bool[5];
	
	QString cheat=QStandardItemModel::item(index,6)->text();
	for (int ctr=0;ctr<cheat.size();ctr++)
	{
		temp[ctr]=(cheat[ctr]==QChar('1'));
	}
	//return temp;
}
int QuestionModel::getScore(int index)
{
	return QStandardItemModel::item(index,7)->text().toInt();
}
int QuestionModel::getTime(int index)
{
	return QStandardItemModel::item(index,8)->text().toInt();
}

bool QuestionModel::isIdentification(int index)
{
	return (QStandardItemModel::item(index,9)->text().toInt() != 0);
}

void QuestionModel::getFullQuestion(int index, Question* q)
{
	q->id=QString::number(index+1);
	q->question=getQuestion(index);
	
	QString ans[ENTRIES];
	ans[0]=getA(index);
	ans[1]=getB(index);
	ans[2]=getC(index);
	ans[3]=getD(index);
	ans[4]=getE(index);
	
	bool* cheat=new bool[CHOICES];
	getAnskey(index,cheat);
	
	//determine type
	if (round==1)
		q->type=Question::CHOOSE_ONE;
	else if (round==2)
		q->type=Question::CHOOSE_ANY;
	else
	{
		if(isIdentification(index))
		{
			q->type=Question::IDENTIFICATION;
		}
		else
			q->type=Question::CHOOSE_ANY;
	}
	//for answerkey
	
	if (q->type==Question::IDENTIFICATION)
	{
		for(int ctr=0;ctr<ENTRIES;ctr++)
		{
			Question::AnswerKeyEntry akey(ans[ctr],true);
			if (ans[ctr]!="")
				q->answer_key.push_back(akey);
		}
	}
	else
	{ 
		for (int ctr=0;ctr<CHOICES;ctr++)
		{
			Question::AnswerKeyEntry akey(ans[ctr],cheat[ctr]);
			q->answer_key.push_back(akey);
		}
	}
	
	q->score=getScore(index);
	q->time_limit=getTime(index);
	
	delete cheat;
}

void QuestionModel::feedData(StageData sd)
{
	for(int ctr=0;ctr<sd.questions.size();ctr++)
	{
		Question q=sd.questions[ctr];
		QList<QStandardItem*> temp;
		temp.append(new QStandardItem(q.question)); //question
		if (q.type==Question::IDENTIFICATION)
		{
			
			for (int cctr=0;cctr<q.answer_key.size();cctr++)
			{
				temp.append(new QStandardItem(q.answer_key[cctr].c));
			}
			while (temp.size()!=6)
				temp.append(new QStandardItem(""));
			temp.append(new QStandardItem("00000"));
			temp.append(new QStandardItem(QString::number(q.score)));
			temp.append(new QStandardItem(QString::number(q.time_limit)));
			temp.append(new QStandardItem("1")); //type
		}
		else
		{
			for(int cctr=0;cctr<q.answer_key.size();cctr++)
				temp.append(new QStandardItem(q.answer_key[cctr].c));
			while (temp.size()!=6)
				temp.append(new QStandardItem(""));
			QString cheat="";
			for(int cctr=0;cctr<q.answer_key.size();cctr++)
			{
				if (q.answer_key[cctr].is_answer)
					cheat.append("1");
				else
					cheat.append("0");
			}
			//while (cheat.length()!=5)
			//	cheat.append("0");
			temp.append(new QStandardItem(cheat));
			temp.append(new QStandardItem(QString::number(q.score)));
			temp.append(new QStandardItem(QString::number(q.time_limit)));
			temp.append(new QStandardItem("0"));
		}
		
		QStandardItemModel::appendRow(temp);
	}
}
