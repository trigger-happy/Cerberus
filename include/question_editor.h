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
#ifndef QUESTION_EDITOR_H
#define QUESTION_EDITOR_H

#include <QtGui>
#include "question_model.h"

#define ROUNDS 4
//#include <QtGui/QMainWindow>

namespace Ui
{
class q_editor;
}

class QEditor : public QMainWindow
{
	Q_OBJECT

public:
	QEditor(QWidget *parent = 0);
	~QEditor();
public slots:
	void list_focus(int round);
	void update_question(int round);
	void add_question(int round);
	void on_first_list();
	void on_r2_list();
	void add_question_r1();
	void add_question_r2();
	void changed_details_r2();
	void update_question_r2();
	
private:
	Ui::q_editor *q_ui;
	void disable_components(int round);
	void enable_components(int round);
	void control_components(int round,bool enable);
	
	
	bool enabled_r2;
	QStandardItemModel* model;
	QuestionModel* round1model;
	QuestionModel* round2model;
	QuestionModel* roundmodel[ROUNDS];
	
	QListView* question_list[ROUNDS];
	QLabel* question_num[ROUNDS];
	QPlainTextEdit* question_text[ROUNDS];
	QLineEdit* question_a[ROUNDS];
	QLineEdit* question_b[ROUNDS];
	QLineEdit* question_c[ROUNDS];
	QLineEdit* question_d[ROUNDS];
	QLineEdit* question_e[ROUNDS];
	QSpinBox* question_score[ROUNDS];
	QSpinBox* question_time[ROUNDS];
	QAbstractButton* question_ans_a[ROUNDS];
	QAbstractButton* question_ans_b[ROUNDS];
	QAbstractButton* question_ans_c[ROUNDS];
	QAbstractButton* question_ans_d[ROUNDS];
	QAbstractButton* question_ans_e[ROUNDS];
	QToolButton* button_update[ROUNDS+1];
	QToolButton* button_cancel[ROUNDS+1];
	QToolButton* button_add[ROUNDS];
	QToolButton* button_remove[ROUNDS];
	
	QSignalMapper* sigToList;
	QSignalMapper* sigToAdd;
	QSignalMapper* sigToUpdate;
};

#endif