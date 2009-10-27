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


#include "question_model.h"

QuestionModel::QuestionModel(int round) : QStandardItemModel(0,9)
{
	this->round=round;
	//super(9,2);
}

void QuestionModel::addNewQuestion()
{
	/*questionmodel order
	question ansa ansb ansc ansd anse anskey score time 
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
	
	QStandardItemModel::appendRow(temp);
	
}

void QuestionModel::removeQuestion(int index)
{
	QStandardItemModel::removeRow(index);
}

void QuestionModel::updateQuestion(int index,QString question,QString a,QString b,QString c,QString d,QString e,QString anskey,QString score,QString time)
{
	QStandardItemModel::item(index,0)->setText(question);
	QStandardItemModel::item(index,1)->setText(a);
	QStandardItemModel::item(index,2)->setText(b);
	QStandardItemModel::item(index,3)->setText(c);
	QStandardItemModel::item(index,4)->setText(d);
	QStandardItemModel::item(index,5)->setText(e);
	QStandardItemModel::item(index,6)->setText(anskey);
	QStandardItemModel::item(index,7)->setText(score);
	QStandardItemModel::item(index,8)->setText(time);
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
	
	QStandardItemModel::item(q1,0)->setText(getQuestion(q2));
	QStandardItemModel::item(q1,1)->setText(getA(q2));
	QStandardItemModel::item(q1,2)->setText(getB(q2));
	QStandardItemModel::item(q1,3)->setText(getC(q2));
	QStandardItemModel::item(q1,4)->setText(getD(q2));
	QStandardItemModel::item(q1,5)->setText(getE(q2));
	QStandardItemModel::item(q1,6)->setText(QStandardItemModel::item(q2,6)->text());
	QStandardItemModel::item(q1,7)->setText(QStandardItemModel::item(q2,7)->text());
	QStandardItemModel::item(q1,8)->setText(QStandardItemModel::item(q2,8)->text());
	
	QStandardItemModel::item(q2,0)->setText(question);
	QStandardItemModel::item(q2,1)->setText(a);
	QStandardItemModel::item(q2,2)->setText(b);
	QStandardItemModel::item(q2,3)->setText(c);
	QStandardItemModel::item(q2,4)->setText(d);
	QStandardItemModel::item(q2,5)->setText(e);
	QStandardItemModel::item(q2,6)->setText(anskey);
	QStandardItemModel::item(q2,7)->setText(score);
	QStandardItemModel::item(q2,8)->setText(time);
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

void QuestionModel::getFullQuestion(int index, Question* q)
{
	q->number=index+1;
	q->question=getQuestion(index);
	q->choices.push_back(getA(index));
	q->choices.push_back(getB(index));
	q->choices.push_back(getC(index));
	q->choices.push_back(getD(index));
	//q->choices[1]=getA(index);
	//q->choices[2]=getB(index);
	//q->choices[3]=getC(index);
	//q->choices[4]=getD(index);
	q->score=getScore(index);
	q->time=getTime(index);
	
	if (round>2)
	{
		q->choices.push_back(getC(index));
		//q->choices[5]=getE(index);
	}
}

void QuestionModel::feedData(QuestionData qd,AnswerData ad)
{
	for(int ctr=0;ctr<qd.questions.size();ctr++)
	{
		Question q=qd.questions[ctr];
		QList<QStandardItem *> temp;
		temp.append(new QStandardItem(q.question)); //question
		temp.append(new QStandardItem(q.choices[0])); // choice a
		temp.append(new QStandardItem(q.choices[1])); // choice b
		temp.append(new QStandardItem(q.choices[2])); // choice c
		temp.append(new QStandardItem(q.choices[3])); // choice d
		if (round>2)
		{
			temp.append(new QStandardItem(q.choices[4]));
		}
		else
		{
			temp.append(new QStandardItem("")); // choice e
		}
		
		pair<AnswerData::iterator,AnswerData::iterator> ret;
		AnswerData::iterator it;
		ret=ad.equal_range(ctr+1);
		QString cheat="00000";
		for (it=ret.first;it!=ret.second;it++)
		{
			int index;
			index=(*it).second.toInt();
			if (index==0) index=5;
//			cheat.replace(index-1,1,"1");
		}
		
		temp.append(new QStandardItem(cheat));
		
		temp.append(new QStandardItem(QString::number(q.score))); // score
		temp.append(new QStandardItem(QString::number(q.time))); // time
		
		QStandardItemModel::appendRow(temp);
	}
}
