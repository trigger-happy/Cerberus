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
#include "admin.h"
#include <iostream>

using namespace std;

Admin::Admin( QWidget* parent ) : QDialog( parent ), m_server( this ),
		m_dlg( new Ui::server_dlg ) {
	m_dlg->setupUi( this );
	// connect dialog signals and slots here
	connect(m_dlg->round_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(onRoundSelection(int)));
	connect(m_dlg->stop_btn, SIGNAL (clicked()), this, SLOT (onStopBtn()));
	connect(m_dlg->start_btn, SIGNAL (clicked()), this, SLOT (onStartBtn()));
	connect(m_dlg->pause_btn, SIGNAL (clicked()), this, SLOT (onPauseBtn()));
	// connect server signals and slots here
}

Admin::~Admin() {
}

// contest control
void Admin::onApplyBtn(){
}


void Admin::onStopBtn() {
	m_server.stopContest();
}

void Admin::onStartBtn() {
	m_server.startContest();
}

void Admin::onPauseBtn() {
	m_server.pauseContest();
}

void Admin::onRoundSelection(int index){
}

// contestant control
void Admin::onContestantListClick(const QModelIndex& index){
}

void Admin::onDropContestant(){
}

void Admin::onViewAnswers(){
}

void Admin::onChangeScore(){
}


// projector control
void Admin::onShowTimeLeft(){
}

void Admin::onShowRankings(){
}

void Admin::onShowNothing(){
}

void Admin::onQuestionListClick(const QModelIndex& index){
}

void Admin::onPreviousQuestion(){
}

void Admin::onNextQuestion(){
}

void Admin::onShowQuestion(){
}

void Admin::onShowAnswer(){
}

void Admin::onShowQuestionTime(){
}

int main ( int argc, char* argv[] )
{
	QApplication a ( argc, argv );
	string cmd;
	Admin admin;
	admin.show();
	return a.exec();
}
