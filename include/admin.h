/*
Copyright (C) 2009 James Choa

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
#ifndef ADMIN_H
#define ADMIN_H
#include <QtGui>
#include "server.h"

namespace Ui {

	class server_dlg;
	class view_answers_dlg;
}

class Admin : public QDialog {
	Q_OBJECT

public:
	Admin( QWidget* parent = 0 );
	~Admin();

public slots:
	// contest control tab
	void onApplyBtn();
	void onStartBtn();
	void onPauseBtn();
	void onStopBtn();
	void onRoundSelection(int index);
	
	// contestant control
	void addContestant(const QString& c_user);
	void removeContestant(const QString& c_user);
	void onContestantListClick(const QModelIndex& index);
	void onDropContestant();
	void onViewAnswers();
	void onChangeScore();

	// view Answers
	void onAnswersRoundSelection(int index);
	void onAnswersOk();
	
	// projector control
	void onShowTimeLeft();
	void onShowRankings();
	void onShowNothing();
	void onQuestionListClick(const QModelIndex& index);
	void onPreviousQuestion();
	void onNextQuestion();
	void onShowQuestion();
	void onShowAnswer();
	void onShowQuestionTime();
	void onStartQuestionTime();
	void onPauseQuestionTime();
	void onStopQuestionTime();

private:
	Ui::server_dlg* m_dlg;
	Ui::view_answers_dlg* m_answers_dlg;
	QDialog* m_answers_w;
	Server* m_server;
	QStandardItemModel* contestants;
	QStandardItemModel* questions3;
	QStandardItemModel* questions4;
	vector<Question> q3_v;
	vector<Question> q4_v;
	QString selected_user;
	int selected_question;
	int selectedRound;
	int currentRound;
};

#endif //ADMIN_H
