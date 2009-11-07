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
#include "ui_server.h"
#include "ui_view_answers.h"
#include "admin.h"
#include <iostream>

using namespace std;

Admin::Admin( QWidget* parent ) : QDialog( parent ), /*m_server( this ),*/
		m_dlg( new Ui::server_dlg ) {

	//TODO: Set up the view_answers dialog
	m_answers_dlg = new Ui::view_answers_dlg;
	m_answers_w = new QDialog( this );
	m_answers_dlg->setupUi( m_answers_w );
	m_answers_w->hide();
	m_dlg->setupUi( this );

	questions3 = new QStandardItemModel ( this );
	questions4 = new QStandardItemModel ( this );
	contestants = new QStandardItemModel( this );
	m_dlg->contestants_listv->setModel( contestants );
	m_dlg->contestants_listv->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_dlg->p_q_listv->setModel( questions3 );
	m_dlg->p_q_listv->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// connect dialog signals and slots here
	connect(m_dlg->round_combo, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onRoundSelection(int)));
	connect(m_dlg->apply_btn, SIGNAL (clicked()), this, SLOT (onApplyBtn()));
	connect(m_dlg->stop_btn, SIGNAL (clicked()), this, SLOT (onStopBtn()));
	connect(m_dlg->start_btn, SIGNAL (clicked()), this, SLOT (onStartBtn()));
	connect(m_dlg->pause_btn, SIGNAL (clicked()), this, SLOT (onPauseBtn()));
	connect(m_dlg->contestants_listv, SIGNAL (clicked(QModelIndex)),
			this, SLOT (onContestantListClick(QModelIndex)));
	connect(m_dlg->contestants_listv, SIGNAL (activated(QModelIndex)),
			this, SLOT (onContestantListClick(QModelIndex)));
	connect(m_dlg->change_score_btn, SIGNAL (clicked()), this, SLOT (onChangeScore()));
	connect(m_dlg->drop_con_btn, SIGNAL (clicked()), this, SLOT (onDropContestant()));
	connect(m_dlg->view_ans_btn, SIGNAL (clicked()), this, SLOT (onViewAnswers()));
	connect(m_answers_dlg->view_answers_rounds, SIGNAL (currentIndexChanged(int)),
			this, SLOT (onAnswersRoundSelection(int)));
	connect(m_answers_dlg->view_answers_ok, SIGNAL (clicked()), this, SLOT (onAnswersOk()));
	connect(m_dlg->p_show_qtime_btn, SIGNAL (clicked()), this, SLOT (onShowQuestionTime()));
	connect(m_dlg->p_show_a_btn, SIGNAL (clicked()), this, SLOT (onShowAnswer()));
	connect(m_dlg->p_show_q_btn, SIGNAL (clicked()), this, SLOT (onShowQuestion()));
	connect(m_dlg->p_con_time_show_btn, SIGNAL (clicked()), this, SLOT (onShowAnswer()));
	connect(m_dlg->p_show_ranks_btn, SIGNAL (clicked()), this, SLOT (onShowRankings()));
	connect(m_dlg->p_q_listv, SIGNAL (clicked(QModelIndex)),
			this, SLOT (onQuestionListClick(QModelIndex)));
	connect(m_dlg->p_q_listv, SIGNAL (activated(QModelIndex)),
			this, SLOT (onQuestionListClick(QModelIndex)));
	connect(m_dlg->qstart_btn, SIGNAL (clicked()), this, SLOT (onStartQuestionTime()));
	connect(m_dlg->qstop_btn, SIGNAL (clicked()), this, SLOT (onStopQuestionTime()));
	connect(m_dlg->qpause_btn, SIGNAL (clicked()), this, SLOT (onPauseQuestionTime()));


	// connect server signals and slots here
	m_server = new Server();
	connect(m_server, SIGNAL (contestantC(QString)), this, SLOT (addContestant(QString)));
	connect(m_server, SIGNAL (contestantDc(QString)), this, SLOT (removeContestant(QString)));
	selectedRound = 1;

	q3_v = m_server->questions3;
	q4_v = m_server->questions4;
	QStandardItem* item;
	int size = q3_v.size();
	for(int i=0; i<size; i++){
		item = new QStandardItem(q3_v.at(i).question);
		questions3->appendRow(item);
	}

	size = q4_v.size();
	for(int i=0; i<size; i++){
		item = new QStandardItem(q4_v.at(i).question);
		questions4->appendRow(item);
	}

	selected_question = 0;

}

Admin::~Admin() {
}

// contest control
void Admin::onApplyBtn(){
	currentRound = selectedRound;
	m_server->setRound(selectedRound);
	if (selectedRound == 3)
		m_dlg->p_q_listv->setModel(questions3);
	else if (selectedRound == 4)
		m_dlg->p_q_listv->setModel(questions4);
}


void Admin::onStopBtn() {
	m_server->stopContest();
	m_dlg->status_lbl->setText("Stopped");
}

void Admin::onStartBtn() {
	m_server->startContest();
	m_dlg->status_lbl->setText("Running");
}

void Admin::onPauseBtn() {
	m_server->pauseContest();
	m_dlg->status_lbl->setText("Paused");
}

void Admin::onRoundSelection(int index){
	selectedRound = index + 1;
}

// contestant control
void Admin::addContestant(const QString& c_user){
	QStandardItem* item = new QStandardItem(c_user);
	contestants->appendRow(item);
	contestants->sort( 0, Qt::AscendingOrder);
}

void Admin::removeContestant(const QString& c_user){
	QStandardItem* item = new QStandardItem(c_user);
	contestants->removeRow(0,contestants->indexFromItem(item));
}

void Admin::onContestantListClick(const QModelIndex& index){
	QString c_user = contestants->itemFromIndex(index)->text();
	m_dlg->user_lbl->setText(c_user);
	m_dlg->score_lbl->setText(QString("%1").arg(m_server->getScore(c_user)));
	selected_user = c_user;
}

void Admin::onDropContestant(){
	if( !selected_user.isEmpty() ){
		this->removeContestant( selected_user );
		m_server->dropConnection( selected_user );
		selected_user = "";
	}
}

void Admin::onViewAnswers(){
	int round = m_answers_dlg->view_answers_rounds->currentIndex();
	if( !selected_user.isEmpty() ){
		QString answers = m_server->viewSubmittedAnswers( selected_user, round );
		m_answers_dlg->view_answers_browser->setText( answers );
		m_answers_w->show();
	}
}

void Admin::onAnswersRoundSelection(int index){
	QString answers = m_server->viewSubmittedAnswers( selected_user, index );
	m_answers_dlg->view_answers_browser->setText( answers );
}

void Admin::onAnswersOk(){
	m_answers_w->hide();
}

void Admin::onChangeScore(){
	if( !selected_user.isEmpty() ){
		bool ok;
		double score = QInputDialog::getDouble( this, tr("Change score"), tr("Enter the new score: "),
												m_server->getScore( selected_user ), 0, 10000, 0, &ok);
		if( ok ){
			m_server->setScore( selected_user, score );
			m_dlg->score_lbl->setText(QString("%1").arg(score));
		}
	}
}


// projector control
void Admin::onShowTimeLeft(){
	m_server->showTimeLeft();
}

void Admin::onShowRankings(){
	//m_server->showRankings();
}

void Admin::onShowNothing(){
}

void Admin::onQuestionListClick(const QModelIndex& index){
	QString question_text;
	QString time_limit;
	Question question;
	selected_question = index.row();

	if (currentRound == 4)
		question = q4_v.at(selected_question);
	else
		question = q3_v.at(selected_question);

	question_text = question.question;
	time_limit = question.time_limit;

	m_dlg->p_question_line->setText( question_text );
	m_dlg->p_time_line->setText( time_limit );
}

void Admin::onPreviousQuestion(){
}

void Admin::onNextQuestion(){
}

void Admin::onShowQuestion(){
	m_server->showQuestion();
}

void Admin::onShowAnswer(){
	m_server->showAnswer();
}

void Admin::onShowQuestionTime(){
	m_server->showQuestionTime();
}

void Admin::onStartQuestionTime(){
	if(currentRound == 3){
		m_server->startQuestionTime(selected_question, q3_v.at(selected_question).time_limit);
	}
	else if(currentRound == 4){
		m_server->startQuestionTime(selected_question, q4_v.at(selected_question).time_limit);
	}
}

void Admin::onPauseQuestionTime(){
	if(currentRound == 3){
		m_server->pauseQuestionTime(selected_question, q3_v.at(selected_question).time_limit);
	}
	else if(currentRound == 4){
		m_server->pauseQuestionTime(selected_question, q4_v.at(selected_question).time_limit);
	}
}

void Admin::onStopQuestionTime(){
	if(currentRound == 3){
		m_server->stopQuestionTime(selected_question, q3_v.at(selected_question).time_limit);
	}
	else if(currentRound == 4){
		m_server->stopQuestionTime(selected_question, q4_v.at(selected_question).time_limit);
	}
}


int main ( int argc, char* argv[] )
{
	QApplication a ( argc, argv );
	string cmd;
	Admin admin;
	admin.show();
	return a.exec();
}
