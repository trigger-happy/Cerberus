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
	temp.append(new QStandardItem("00000")); // answerkey
	temp.append(new QStandardItem("1")); // score
	temp.append(new QStandardItem("0")); // time
	//temp.append(new QStandardItem)
	
	QStandardItemModel::appendRow(temp);
	
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
	//swapping goes here
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
bool* QuestionModel::getAnskey(int index)
{
	bool* temp=new bool[5];
	
	QString cheat=QStandardItemModel::item(index,6)->text();
	for (int ctr=0;ctr<cheat.size();ctr++)
	{
		temp[ctr]=(cheat[ctr]==QChar('1'));
	}
	return temp;
}
int QuestionModel::getScore(int index)
{
	return QStandardItemModel::item(index,7)->text().toInt();
}
int QuestionModel::getTime(int index)
{
	return QStandardItemModel::item(index,8)->text().toInt();
}
