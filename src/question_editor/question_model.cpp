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

QuestionModel::QuestionModel(int column) : QStandardItemModel(0,column)
{
	//super(9,2);
}

void QuestionModel::addNewQuestion()
{
	/**questionmodel order
	question score time ansa ansb ansc ansd boola boolb boolc boold boole
	*/
	QList<QStandardItem *> temp;
	temp.append(new QStandardItem("new question"));
	temp.append(new QStandardItem("my a"));
	temp.append(new QStandardItem("b"));
	temp.append(new QStandardItem("c"));
	temp.append(new QStandardItem("d"));
	
	QStandardItemModel::appendRow(temp);
	
}

void QuestionModel::updateQuestion(int index,QString question,QString a,QString b,QString c,QString d)
{
	QStandardItemModel::item(index,0)->setText(question);
	QStandardItemModel::item(index,1)->setText(a);
}

void QuestionModel::swapOrder(int q1,int q2)
{
	//swapping goes here
}