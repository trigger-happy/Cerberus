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

#ifndef question_model_h
#define question_model_h

#include <QtGui>

class QuestionModel : public QStandardItemModel
{
	Q_OBJECT
private:
	int round;
public:
	QuestionModel(int round);
	int getRound();
	void addNewQuestion();
	void swapOrder(int q1,int q2);
	QString getQuestion(int index);
	QString getA(int index);
	QString getB(int index);
	QString getC(int index);
	QString getD(int index);
	QString getE(int index);
	bool* getAnskey(int index);
	int getTime(int index);
	int getScore(int index);
	void updateQuestion(int index,QString question,QString a,QString b,QString c,QString d,QString e,QString anskey,QString time,QString score);
};

#endif