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
#include "util/xml_util.h"

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
private slots:
	void list_focus(int round);
	void update_question(int round);
	void cancel_update(int round);
	void add_question(int round);
	void changed_general(int round);
	void changed_details(int round);
	void remove_question(int round);
	void move_up(int round);
	void move_down(int round);
	void disableDuplicates(int round);
	void newfile();
	void import();
	void load();
	void save();
	void exit();
	void saveAs();
	
private:
	Ui::q_editor *q_ui;
	void control_components(int round,bool enable);
	
	QuestionModel* roundmodel[ROUNDS];
	QListView* question_list[ROUNDS];
	QLabel* question_num[ROUNDS];
	QPlainTextEdit* welcome[ROUNDS];
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
	QToolButton* button_update[ROUNDS];
	QToolButton* button_cancel[ROUNDS];
	QToolButton* button_add[ROUNDS];
	QToolButton* button_remove[ROUNDS];
	QToolButton* button_up[ROUNDS];
	QToolButton* button_down[ROUNDS];
	QSpinBox* duration[ROUNDS];
	
	QSignalMapper* sigToList;
	QSignalMapper* sigToAdd;
	QSignalMapper* sigToRemove;
	QSignalMapper* sigToUpdate;
	QSignalMapper* sigToCancel;
	QSignalMapper* sigToGeneralUpdate;
	QSignalMapper* sigToDetailUpdate;
	QSignalMapper* sigToUp;
	QSignalMapper* sigToDown;
	QSignalMapper* sigToChoices;
	
	XmlUtil xml_util;
	QString file_prefix;
	
	bool qmod[ROUNDS];
	int history[ROUNDS];
	bool fully_updated[ROUNDS];
protected:
	void closeEvent(QCloseEvent*);
};

#endif

