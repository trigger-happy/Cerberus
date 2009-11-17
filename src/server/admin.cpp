/*
Copyright (C) 2009 James Choa
Copyright (C) 2009 Joshua Cheng

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
#include "util/ContestTimer.h"
#include <iostream>

using namespace std;

static const unsigned int PRECISE_UPDATE_INTERVAL = 100;

Admin::Admin( QWidget* parent ) : QDialog( parent ), /*m_server( this ),*/
		m_dlg( new Ui::server_dlg ) {

	//TODO: Set up the view_answers dialog
	m_answers_dlg = new Ui::view_answers_dlg;
	m_answers_w = new QDialog( this );
	m_answers_dlg->setupUi( m_answers_w );
	m_answers_w->hide();
	m_dlg->setupUi( this );

	m_questions3 = new QStandardItemModel ( this );
	m_questions4 = new QStandardItemModel ( this );
	m_contestants = new QStandardItemModel( this );
	m_dlg->contestants_listv->setModel( m_contestants );
	m_dlg->contestants_listv->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_dlg->p_q_listv->setModel( NULL );
	m_dlg->p_q_listv->setEditTriggers( QAbstractItemView::NoEditTriggers );

	// connect dialog signals and slots here
	connect( m_dlg->round_combo, SIGNAL( currentIndexChanged( int ) ),
	         this, SLOT( onRoundSelection( int ) ) );
	connect( m_dlg->apply_btn, SIGNAL ( clicked() ), this, SLOT ( onApplyBtn() ) );
	connect( m_dlg->stop_btn, SIGNAL ( clicked() ), this, SLOT ( onStopBtn() ) );
	connect( m_dlg->start_btn, SIGNAL ( clicked() ), this, SLOT ( onStartBtn() ) );
	connect( m_dlg->pause_btn, SIGNAL ( clicked() ), this, SLOT ( onPauseBtn() ) );
	connect( m_dlg->contestants_listv, SIGNAL ( clicked( QModelIndex ) ),
	         this, SLOT ( onContestantListClick( QModelIndex ) ) );
	connect( m_dlg->contestants_listv, SIGNAL ( activated( QModelIndex ) ),
	         this, SLOT ( onContestantListClick( QModelIndex ) ) );
	connect( m_dlg->change_score_btn, SIGNAL ( clicked() ), this, SLOT ( onChangeScore() ) );
	connect( m_dlg->drop_con_btn, SIGNAL ( clicked() ), this, SLOT ( onDropContestant() ) );
	connect( m_dlg->view_ans_btn, SIGNAL ( clicked() ), this, SLOT ( onViewAnswers() ) );
	connect( m_answers_dlg->view_answers_rounds, SIGNAL ( currentIndexChanged( int ) ),
	         this, SLOT ( onAnswersRoundSelection( int ) ) );
	connect( m_answers_dlg->view_answers_ok, SIGNAL ( clicked() ), this, SLOT ( onAnswersOk() ) );
	connect( m_dlg->p_show_a_btn, SIGNAL ( clicked() ), this, SLOT ( onShowAnswer() ) );
	connect( m_dlg->p_hide_a_btn, SIGNAL ( clicked() ), this, SLOT ( onHideAnswer() ) );
	connect( m_dlg->p_con_time_show_btn, SIGNAL ( clicked() ), this, SLOT ( onShowTimeLeft() ) );
	connect( m_dlg->p_show_ranks_btn, SIGNAL ( clicked() ), this, SLOT ( onShowRankings() ) );
	connect( m_dlg->p_show_main_btn, SIGNAL( clicked() ), this, SLOT( onShowMainScreen() ) );
	connect( m_dlg->p_q_listv, SIGNAL ( clicked( QModelIndex ) ),
	         this, SLOT ( onQuestionListClick( QModelIndex ) ) );
	connect( m_dlg->p_q_listv, SIGNAL ( activated( QModelIndex ) ),
	         this, SLOT ( onQuestionListClick( QModelIndex ) ) );
	connect( m_dlg->qstart_btn, SIGNAL ( clicked() ), this, SLOT ( onStartQuestionTime() ) );
	connect( m_dlg->qstop_btn, SIGNAL ( clicked() ), this, SLOT ( onStopQuestionTime() ) );
	connect( m_dlg->qpause_btn, SIGNAL ( clicked() ), this, SLOT ( onPauseQuestionTime() ) );
	connect( m_dlg->reset_score_btn, SIGNAL( clicked() ), this, SLOT( onScoreReset() ) );


	// connect server signals and slots here
	m_server = new Server();
	connect( m_server, SIGNAL ( contestantC( QString ) ), this, SLOT ( addContestant( QString ) ) );
	connect( m_server, SIGNAL ( contestantDc( QString ) ), this, SLOT ( removeContestant( QString ) ) );
	connect( m_server, SIGNAL( onContestTimeRequest( ushort& ) ), this, SLOT( onContestTimeRequest( ushort& ) ) );
	connect( m_server, SIGNAL( newRankModel( QStandardItemModel* ) ), this, SLOT( onNewRankModel( QStandardItemModel* ) ) );
	connect( m_server, SIGNAL( newTeamModel( QStandardItemModel* ) ), this, SLOT( onNewTeamModel( QStandardItemModel* ) ) );

	m_preciseTimer = new ContestTimer( PRECISE_UPDATE_INTERVAL );
	connect( m_preciseTimer, SIGNAL( timeUpdate( uint ) ), m_server, SLOT( onPreciseTimerTick( uint ) ) );

	m_selectedRound = 1;

	m_q3_v = m_server->questions3;
	m_q4_v = m_server->questions4;
	QStandardItem* item;
	int size = m_q3_v.size();

	for ( int i = 0; i < size; i++ ) {
		item = new QStandardItem( m_q3_v.at( i ).question );
		m_questions3->appendRow( item );
	}

	size = m_q4_v.size();

	for ( int i = 0; i < size; i++ ) {
		item = new QStandardItem( m_q4_v.at( i ).question );
		m_questions4->appendRow( item );
	}

	m_selected_question = 0;

	onRoundSelection( 0 );
	onApplyBtn();
	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL( timeout() ), this, SLOT( onTimeUpdate() ) );
	connect( m_timer, SIGNAL( timeout() ), m_server, SLOT( timerTick() ) );

	m_dlg->score_tbv->setModel( m_server->getRankModel() );
	m_dlg->team_tbv->setModel( m_server->getTeamModel() );
}

Admin::~Admin() {
}

void Admin::onTimeUpdate() {
	m_timeleft--;
	m_dlg->time_lcd->display( m_timeleft );
	m_dlg->time2_lcd->display( m_timeleft );

	if ( m_timeleft <= 0 ) {
		if ( m_selectedRound < 3 ) {
			onStopBtn();
		} else {
			onStopQuestionTime();
		}
	}
}

void Admin::onContestTimeRequest( ushort& contime ) {
	contime = m_timeleft;
}

// contest control
void Admin::onApplyBtn() {
	m_currentRound = m_selectedRound;
	m_timeleft = m_dlg->timer_spin->value();
	m_preciseTimer->setDuration( m_timeleft * 1000u );
	m_dlg->time_lcd->display( m_timeleft );
	m_dlg->time2_lcd->display( m_timeleft );
	m_server->setRound( m_selectedRound );
	m_server->setContestTime( m_timeleft );

	if ( m_selectedRound == 3 )
		m_dlg->p_q_listv->setModel( m_questions3 );
	else if ( m_selectedRound == 4 )
		m_dlg->p_q_listv->setModel( m_questions4 );
	else
		m_dlg->p_q_listv->setModel( NULL );
}


void Admin::onStopBtn() {
	if ( m_selectedRound < 3 ) {
		m_timer->stop();
		m_preciseTimer->stop();
	}

	m_dlg->time_lcd->display( 0 );
	m_dlg->time2_lcd->display( 0 );

	m_server->stopContest();

	m_dlg->status_lbl->setText( "Stopped" );
}

void Admin::onStartBtn() {
	if ( m_selectedRound < 3 ) {
		m_timer->start( 1000 );
		m_preciseTimer->start();
	}

	m_server->startContest();

	m_dlg->status_lbl->setText( "Running" );
}

void Admin::onPauseBtn() {
	if ( m_selectedRound < 3 ) {
		m_timer->stop();
		m_preciseTimer->pause();
	}

	m_server->pauseContest();

	m_dlg->status_lbl->setText( "Paused" );
}

void Admin::onRoundSelection( int index ) {
	m_selectedRound = index + 1;
	m_selected_question = 65535;
	int time = 0;

	if ( m_selectedRound < 3 ) {
		time = m_server->getRoundTime( m_selectedRound );
	} else {
		time = m_server->getQuestionTime( m_selectedRound, 0 );
	}

	m_dlg->timer_spin->setValue( time );

	//m_dlg->time_lcd->display(time);
	//m_dlg->time2_lcd->display(time);
}

// contestant control
void Admin::addContestant( const QString& c_user ) {

	QStandardItem* item;

	//if(!users.contains(c_user)){
	item = new QStandardItem( c_user );
	m_users[c_user] = item;
	/*}
	else
		item = users[c_user];*/
	m_contestants->appendRow( item );
	m_contestants->sort( 0, Qt::AscendingOrder );
}

void Admin::removeContestant( const QString& c_user ) {
	QModelIndex index = m_contestants->indexFromItem( m_users[c_user] );

	if ( index.row() != -1 ) {
		cout << ( QString( "Removing contestant %1 from index %2" ).arg( c_user ).arg( index.row() ) ).toStdString() << endl;
		bool removed = m_contestants->removeRow( index.row(), QModelIndex() );

		if ( removed ) cout << "Contestant successfully removed.\n";
		else cout << "Remove failed.\n";
	}
}

void Admin::onContestantListClick( const QModelIndex& index ) {
	QString c_user = m_contestants->itemFromIndex( index )->text();
	m_dlg->user_lbl->setText( c_user );
	m_dlg->score_lbl->setText( QString( "%1" ).arg( m_server->getScore( c_user ) ) );

	UserData ud;
	SqlUtil::getInstance().getSpecificUser( c_user, ud );
	m_dlg->team_lbl->setText( QString( "%1" ).arg( ud.teamname ) );
	m_selected_user = c_user;
}

void Admin::onDropContestant() {
	if ( !m_selected_user.isEmpty() ) {
		m_server->dropConnection( m_selected_user );
		m_selected_user = "";
	}
}

void Admin::onViewAnswers() {
	int round = m_answers_dlg->view_answers_rounds->currentIndex();

	if ( !m_selected_user.isEmpty() ) {
		QString answers = m_server->viewSubmittedAnswers( m_selected_user, round );
		m_answers_dlg->view_answers_browser->setText( answers );
		m_answers_w->show();
	}
}

void Admin::onAnswersRoundSelection( int index ) {
	QString answers = m_server->viewSubmittedAnswers( m_selected_user, index );
	m_answers_dlg->view_answers_browser->setText( answers );
}

void Admin::onAnswersOk() {
	m_answers_w->hide();
}

void Admin::onChangeScore() {
	if ( !m_selected_user.isEmpty() ) {
		bool ok;
		double score = QInputDialog::getDouble( this, tr( "Change score" ), tr( "Enter the new score: " ),
		                                        m_server->getScore( m_selected_user ), 0, 10000, 0, &ok );

		if ( ok ) {
			m_server->setScore( m_selected_user, score );
			m_dlg->score_lbl->setText( QString( "%1" ).arg( score ) );
			m_server->updateRankData();
		}
	}
}


// projector control
void Admin::onShowTimeLeft() {
	m_server->showTimeLeft();
}

void Admin::onShowRankings() {
	m_server->showRankings();
}

void Admin::onShowMainScreen() {
	m_server->showMainScreen();
}

void Admin::onQuestionListClick( const QModelIndex& index ) {
	QString question_text;
	QString time_limit;
	Question question;
	m_selected_question = index.row();

	if ( m_currentRound == 4 )
		question = m_q4_v.at( m_selected_question );
	else
		question = m_q3_v.at( m_selected_question );

	question_text = question.question;

	time_limit = QString( "%1" ).arg( question.time_limit );

	m_dlg->p_question_line->setText( question_text );

	m_dlg->p_time_line->setText( time_limit );
}

void Admin::onHideAnswer() {
	m_server->hideAnswer();
}

void Admin::onShowAnswer() {
	m_server->showAnswer();
}

void Admin::onShowQuestionTime() {
	m_server->showQuestionTime();
}

void Admin::onStartQuestionTime() {
	//m_server->showQuestionTime();
	m_timeleft = m_dlg->p_time_line->text().toInt();
	m_preciseTimer->setDuration( m_timeleft * 1000u );
	m_dlg->time_lcd->display( m_timeleft );
	m_dlg->time2_lcd->display( m_timeleft );
	m_timer->start( 1000 );
	m_preciseTimer->start();


	if ( m_currentRound == 3 ) {
		m_server->startQuestionTime( m_selected_question, m_q3_v.at( m_selected_question ).time_limit );
	} else if ( m_currentRound == 4 ) {
		m_server->startQuestionTime( m_selected_question, m_q4_v.at( m_selected_question ).time_limit );
	}
}

void Admin::onPauseQuestionTime() {
	m_timer->stop();
	m_preciseTimer->pause();

	if ( m_currentRound == 3 ) {
		m_server->pauseQuestionTime( m_selected_question, m_q3_v.at( m_selected_question ).time_limit );
	} else if ( m_currentRound == 4 ) {
		m_server->pauseQuestionTime( m_selected_question, m_q4_v.at( m_selected_question ).time_limit );
	}
}

void Admin::onStopQuestionTime() {
	m_timer->stop();
	m_preciseTimer->stop();
	m_dlg->time_lcd->display( 0 );
	m_dlg->time2_lcd->display( 0 );

	if ( m_currentRound == 3 ) {
		m_server->stopQuestionTime( m_selected_question, m_q3_v.at( m_selected_question ).time_limit );
	} else if ( m_currentRound == 4 ) {
		m_server->stopQuestionTime( m_selected_question, m_q4_v.at( m_selected_question ).time_limit );
	}
}

void Admin::onScoreReset() {
	m_server->scoreReset();
}

void Admin::onNewRankModel( QStandardItemModel* model ) {
	m_dlg->score_tbv->setModel( model );
}

void Admin::onNewTeamModel( QStandardItemModel* model ) {
	m_dlg->team_tbv->setModel( model );
}

int main ( int argc, char* argv[] ) {
	QApplication a ( argc, argv );
	string cmd;
	Admin admin;
	admin.show();
	return a.exec();
}
